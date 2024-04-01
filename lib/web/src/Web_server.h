/*
       @file       Web_server.h
       @brief      Declaración de variables y funciones usadas en el File Server

       @author     Jordi Gauchia

       @date       27/03/2022
*/

// **********************************************
//  Declaración tamaño máximo archivo upload
// **********************************************
const size_t MAX_FILESIZE = 1024 * 1024 * 15;
File UploadFile;
String dir;
String olddir;

#define FIRMWARE_VERSION "0.3"
#define APP "ESPCompuTone File Utility"

// **********************************************
//  Declaración servidor WEB
// **********************************************
AsyncWebServer server(80);
bool shouldReboot = false;

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

// **********************************************
//  Función para mensaje de error si no se
//  encuentra la Web del FileServer
// **********************************************
void WebnotFound(AsyncWebServerRequest *request)
{
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  log_i("%s", logmessage.c_str());
  request->send(404, "text/plain", "Not found");
}

// **********************************************
//  Función para reemplazar variables dentro
//  del archivo HTML de la Web
// **********************************************
String WebParser(const String &var)
{
  if (var == "FIRMWARE")
    return FIRMWARE_VERSION;
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

// **********************************************
//  Función para reiniciar ESP
// **********************************************
void rebootESP(String message)
{
  // Serial.print("Rebooting ESP32: "); Serial.println(message);
  ESP.restart();
}

// **********************************************
//  Función para listar archivos SD en la Web
// **********************************************
String listFiles(bool ishtml)
{
  String returnText = "";
  File root = SD.open(dir.c_str());
  File foundfile = root.openNextFile();
  if (ishtml)
  {
    returnText += "<div style=\"overflow-y:scroll;\"><table><tr><th>Name</th><th style=\"text-align:center\">Size</th><th></th><th></th></tr>";
    if (olddir.length() != 0)
    {
      returnText += "<tr align='left'><td style=\"width:300px\">";
      returnText += "<img src=\"folder\"> <a href='#' onclick='changeDirectory(\"..\")'>..</a>";
      returnText += "</td><td style=\"text-align:center\">dir</td><td></td><td></td>";
      returnText += "</tr>";
    }
  }
  while (foundfile)
  {
    if (ishtml)
    {
      returnText += "<tr align='left'><td style=\"width:300px\">";
      if (foundfile.isDirectory())
      {
        returnText += "<img src=\"folder\"> <a href='#' onclick='changeDirectory(\"" + String(foundfile.name()) + "\")'>" + String(foundfile.name()) + "</a>";
        returnText += "</td><td style=\"text-align:center\">dir</td><td></td><td></td>";
      }
      else
      {
        returnText += "<img src=\"file\"> " + String(foundfile.name());
        returnText += "</td><td style=\"text-align:right\">" + humanReadableSize(foundfile.size()) + "</td>";
        returnText += "<td><button class=\"button\" onclick=\"downloadDeleteButton('" + String(foundfile.name()) + "', 'download')\"><img src=\"down\"> Download</button></td>";
        returnText += "<td><button class=\"button\" onclick=\"downloadDeleteButton('" + String(foundfile.name()) + "', 'delete')\"><img src=\"del\"> Delete</button></td>";
      }
      returnText += "</tr>";
    }
    else
    {
      returnText += "File: " + String(foundfile.name()) + " Size: " + humanReadableSize(foundfile.size()) + "\n";
    }
    foundfile = root.openNextFile();
  }
  if (ishtml)
  {
    returnText += "</table></div>";
  }
  root.close();
  foundfile.close();
  return returnText;
}

// **********************************************
//  Función para subir archivos a la Web
// **********************************************
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    String logmessage = "Upload Start: " + String(filename);
    request->client()->setRxTimeout(15000);
    request->_tempFile = SD.open(dir + "/" + filename, "w");
    Serial.println(logmessage);
  }

  if (len)
    request->_tempFile.write(data, len);

  if (final)
  {
    String logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
    request->_tempFile.close();
    Serial.println(logmessage);
    request->redirect("/");
  }
}

// **********************************************
//  Función para crear servicio Web
// **********************************************
void configureWebServer()
{
  server.onNotFound(WebnotFound);
  server.onFileUpload(handleUpload);
  dir = "/";
  olddir = "";
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String logmessage = "Client:" + request->client()->remoteIP().toString() + +" " + request->url();
                log_i("%s", logmessage.c_str());
                request->send_P(200, "text/html", index_html, WebParser); });

  server.on("/file", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/file.png", "image/png");
            });

  server.on("/folder", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/folder.png", "image/png");
            });

  server.on("/down", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/download.png", "image/png");
            });

  server.on("/up", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/upload.png", "image/png");
            });

  server.on("/del", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/delete.png", "image/png");
            });

  server.on("/list", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/list.png", "image/png");
            });

  server.on("/reb", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/reboot.png", "image/png");
            });

  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();

      request->send(200, "text/html", reboot_html);
      log_i("%s",logmessage.c_str());
      shouldReboot = true; });

  server.on("/listfiles", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
              log_i("%s", logmessage.c_str());
              request->send(200, "text/plain", listFiles(true)); });

  server.on("/file", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
              log_i("%s", logmessage.c_str());

              if (request->hasParam("name") && request->hasParam("action"))
              {
                const char *fileName = request->getParam("name")->value().c_str();
                const char *fileAction = request->getParam("action")->value().c_str();

                logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url() + "?name=" + String(fileName) + "&action=" + String(fileAction);

                String path = dir + String(fileName);
                log_i("%s",path.c_str());

                if (!SD.exists(path))
                {
                  request->send(400, "text/plain", "ERROR: file does not exist");
                }
                else
                {
                  log_i("%s", logmessage + " file exists");
                  if (strcmp(fileAction, "download") == 0)
                  {
                    logmessage += " downloaded";
                    request->send(SD, path, "application/octet-stream");
                  }
                  else if (strcmp(fileAction, "delete") == 0)
                  {
                    logmessage += " deleted";
                    SD.remove(path);
                    request->send(200, "text/plain", "Deleted File: " + String(fileName));
                  }
                  else
                  {
                    logmessage += " ERROR: invalid action param supplied";
                    request->send(400, "text/plain", "ERROR: invalid action param supplied");
                  }
                  log_i("%s", logmessage.c_str());
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

                if (newDir == "/..")
                {
                  // Si intenta subir un nivel
                  if (olddir != "/")
                  {
                    // Si no estamos en la raíz, subir un nivel
                    olddir = olddir.substring(0, olddir.lastIndexOf('/'));
                    request->send(200, "text/plain", "Directory changed successfully");
                  }
                  else
                  {
                    // Si estamos en la raíz, enviar un mensaje de error
                    request->send(400, "text/plain", "Cannot go up from root directory");
                  }
                }
                else
                {
                  // Cambiar al nuevo directorio
                  olddir = newDir;

                  if (SD.exists(olddir))
                  {
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

              //  if (request->hasParam("dir"))
              //  {
              //       olddir = dir;
              //       log_i("%s",olddir.c_str());
              //       dir = request->getParam("dir")->value();
              //       log_i("%s",dir.c_str());
              //       if ( dir.equals("/..") )
              //       {
              //          dir = olddir;
              //          olddir = "";
              //       }
              //       if (SD.open(dir.c_str()))
              //       {
              //           request->send(200, "text/plain", "Directory changed successfully");
              //       }
              //       else
              //       {
              //           request->send(500, "text/plain", "Failed to change directory");
              //       }
              //   }
              //   else
              //   {
              //       request->send(400, "text/plain", "ERROR: dir parameter required");
              //   }
            });
}