/**
 * @file webserver.h
 * @author Jordi Gauchía
 * @brief Web server functions
 * @version 0.3.1
 * @date 2024-04
 */

/**
 * @brief Current directory
 * 
 */
String oldDir;

#define APP "ESPCompuTone File Utility"

/**
 * @brief Web Server Declaration
 *
 */
AsyncWebServer server(80);

/**
 * @brief Convert bytes to Human Readable Size
 *
 * @param bytes
 * @return String
 */
String humanReadableSize(uint64_t bytes)
{
  if (bytes < 1024)
    return String(bytes) + " B";
  else if (bytes < (1024 * 1024))
    return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024))
    return String(bytes / (1024.0 * 1024.0)) + " MB";
  else
    return String(bytes / (1024.0 * 1024.0 * 1024.0)) + " GB";
}

/**
 * @brief Manage Web not found error
 *
 * @param request
 */
void webNotFound(AsyncWebServerRequest *request)
{
  String logMessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  log_i("%s", logMessage.c_str());
  request->send(404, "text/plain", "Not found");
}

/**
 * @brief Replace HTML Vars with values
 *
 * @param var
 * @return String
 */
String webParser(const String &var)
{
  if (var == "FIRMWARE")
    return String(VERSION,1) + "." + String(REVISION);
  else if (var == "FREEFS")
    return humanReadableSize((SD.totalBytes() - SD.usedBytes()));
  else if (var == "USEDFS")
    return humanReadableSize(SD.usedBytes());
  else if (var == "TOTALFS")
    return humanReadableSize(SD.cardSize());
  else if (var == "APP")
    return APP;
  else if (var == "TYPEFS")
  {
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_MMC)
    {
      return "MMC";
    }
    else if (cardType == CARD_SD)
    {
      return "SDSC";
    }
    else if (cardType == CARD_SDHC)
    {
      return "SDHC";
    }
    else
    {
      return "UNKNOWN";
    }
  }
  else
    return "";
}

/**
 * @brief Reboot ESP
 * 
 * @param message 
 */
void rebootESP()
{
  log_i("Rebooting ESP32: ");
  ESP.restart();
}

/**
 * @brief List Files in Web Page
 * 
 * @param ishtml 
 * @return String 
 */
String listFiles(bool ishtml)
{
  String returnText = "";
  File root = SD.open(oldDir.c_str());
  File foundFile = root.openNextFile();
  if (ishtml)
  {
    returnText += "<div style=\"overflow-y:scroll;\"><table><tr><th>Name</th><th style=\"text-align:center\">Size</th><th></th><th></th></tr>";
    if (oldDir != "/")
    {
      returnText += "<tr align='left'><td style=\"width:300px\">";
      returnText += "<img src=\"folder\"> <a href='#' onclick='changeDirectory(\"..\")'>..</a>";
      returnText += "</td><td style=\"text-align:center\">dir</td><td></td><td></td>";
      returnText += "</tr>";
    }
  }
  while (foundFile)
  {
    if (ishtml)
    {
      returnText += "<tr align='left'><td style=\"width:300px\">";
      if (foundFile.isDirectory())
      {
        returnText += "<img src=\"folder\"> <a href='#' onclick='changeDirectory(\"" + String(foundFile.name()) + "\")'>" + String(foundFile.name()) + "</a>";
        returnText += "</td><td style=\"text-align:center\">dir</td><td></td><td></td>";
      }
      else
      {
        returnText += "<img src=\"files\"> " + String(foundFile.name());
        returnText += "</td><td style=\"text-align:right\">" + humanReadableSize(foundFile.size()) + "</td>";
        returnText += "<td><button class=\"button\" onclick=\"downloadDeleteButton('" + String(foundFile.name()) + "', 'download')\"><img src=\"down\"> Download</button></td>";
        returnText += "<td><button class=\"button\" onclick=\"downloadDeleteButton('" + String(foundFile.name()) + "', 'delete')\"><img src=\"del\"> Delete</button></td>";
      }
      returnText += "</tr>";
    }
    else
    {
      returnText += "File: " + String(foundFile.name()) + " Size: " + humanReadableSize(foundFile.size()) + "\n";
    }
    foundFile = root.openNextFile();
  }
  if (ishtml)
  {
    returnText += "</table></div>";
  }
  root.close();
  foundFile.close();
  return returnText;
}

/**
 * @brief Upload file handle
 * 
 * @param request 
 * @param filename 
 * @param index 
 * @param data 
 * @param len 
 */
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    request->client()->setRxTimeout(15000);
    request->_tempFile = SD.open(oldDir + "/" + filename, "w");
  }
  if (len)
    request->_tempFile.write(data, len);

  if (final)
  {
    request->_tempFile.close();
    request->redirect("/");
  }
}

/**
 * @brief Configure Web Server
 * 
 */
void configureWebServer()
{
  server.onNotFound(webNotFound);
  server.onFileUpload(handleUpload);
  oldDir = "/";
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String logMessage = "Client:" + request->client()->remoteIP().toString() + +" " + request->url();
                log_i("%s", logMessage.c_str());
                request->send_P(200, "text/html", index_html, webParser); 
            });

  server.on("/files", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/file.png", "image/png"); });

  server.on("/folder", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/folder.png", "image/png"); });

  server.on("/down", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/download.png", "image/png"); });

  server.on("/up", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/upload.png", "image/png"); });

  server.on("/del", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/delete.png", "image/png"); });

  server.on("/list", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/list.png", "image/png"); });

  server.on("/reb", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/reboot.png", "image/png"); });

  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String logMessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
              request->send(200, "text/html", reboot_html);
              log_i("%s",logMessage.c_str());
              rebootESP(); });

  server.on("/listfiles", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String logMessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
              log_i("%s", logMessage.c_str());
              request->send(200, "text/plain", listFiles(true)); });

  server.on("/file", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String logMessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
              log_i("%s", logMessage.c_str());

              if (request->hasParam("name") && request->hasParam("action"))
              {
                const char *fileName = request->getParam("name")->value().c_str();
                const char *fileAction = request->getParam("action")->value().c_str();

                logMessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url() + "?name=" + String(fileName) + "&action=" + String(fileAction);

                String path = oldDir + String(fileName);
                log_i("%s",path.c_str());

                if (!SD.exists(path))
                {
                  request->send(400, "text/plain", "ERROR: file does not exist");
                }
                else
                {
                  log_i("%s", logMessage + " file exists");
                  if (strcmp(fileAction, "download") == 0)
                  {
                    logMessage += " downloaded";
                    request->send(SD, path, "application/octet-stream");
                  }
                  else if (strcmp(fileAction, "delete") == 0)
                  {
                    logMessage += " deleted";
                    SD.remove(path);
                    request->send(200, "text/plain", "Deleted File: " + String(fileName));
                  }
                  else
                  {
                    logMessage += " ERROR: invalid action param supplied";
                    request->send(400, "text/plain", "ERROR: invalid action param supplied");
                  }
                  log_i("%s", logMessage.c_str());
                }
              }
              else
              {
                request->send(400, "text/plain", "ERROR: name and action params required");
              } });

  server.on("/changedirectory", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (request->hasParam("dir"))
              {
                String newDir = request->getParam("dir")->value();
                log_i("new dir %s", newDir.c_str());
                log_i("old dir %s", oldDir.c_str());
                // UP Directory
                if (newDir == "/..")
                {
                  if (oldDir != "/..")
                  {
                    oldDir = oldDir.substring(0, oldDir.lastIndexOf("/"));
                    if (oldDir == "")
                      oldDir = "/";
                    SD.open(oldDir.c_str());
                    request->send(200, "text/plain", "Directory changed successfully");
                  }
                  else
                  {
                    request->send(400, "text/plain", "Cannot go up from root directory");
                  }
                }
                else
                // DOWN Directory
                {
                  if (oldDir != "/")
                    oldDir = oldDir + newDir;
                  else
                    oldDir = newDir;

                  if (SD.exists(oldDir))
                  {
                    SD.open(oldDir.c_str());
                    request->send(200, "text/plain", "Directory changed successfully");
                  }
                  else
                  {
                    request->send(500, "text/plain", "Failed to change directory");
                  }
                }
              }
              else
              {
                request->send(400, "text/plain", "ERROR: dir parameter required");
              }
            });
}