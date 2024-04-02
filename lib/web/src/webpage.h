/**
 * @file webpage.h
 * @author Jordi Gauchía
 * @brief Web page
 * @version 0.3
 * @date 2024-04
 */

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">

<head>

  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">

  <style>
      h1  {
            font-family: "Lucida Console", "Courier New", monospace;
            font-weight: bold;
            font-size: 30px;
          }
      .p3 {
            font-family: "Lucida Console", "Courier New", monospace;
            font-size: 12px;
          }
      .p2 {
            font-family: "Lucida Console", "Courier New", monospace;
            font-size: 10px;
          }
      .button {
                  background-color:  #5d6d7e;
                  border: none;
                  color: white;
                  padding: 5px 25px;
                  text-align: center;
                  text-decoration: none;
                  display: inline-block;
                  font-size: 12px;
                  opacity: 0.6;
                  transition: 0.3s;
                  margin: 4px 2px;
                  cursor: pointer;
              }
      .button:hover {opacity: 1}

      a:link {
                font-weight: bold;
                font-style: italic;
                text-decoration: none;
                color: black;
             }

      th, td {
                padding: 4px;
                text-align: left;
                border-bottom: 1px solid #ddd;
              }

      td {
            font-family: "Lucida Console", "Courier New", monospace;
            font-size: 10px;
            height: 25px;
            vertical-align: middle;
          }
  </style>

</head>

<body>

  <h1>%APP%</h1>

  <p class="p3">Firmware: %FIRMWARE%</p>
  <p class="p3">Type: %TYPEFS% | Size: %TOTALFS% | Used: %USEDFS% | Free: %FREEFS%</p>

  <p>
  <button class="button" onclick="rebootButton()"><img src="reb"> Reboot</button>
  <button class="button" onclick="listFilesButton()"><img src="list"> List Files</button>
  <button class="button" onclick="showUploadButtonFancy()"><img src="up"> Upload File</button>
  </p>

  <p class="p3" id="status"></p>
  <p class="p3" id="detailsheader"></p>
  <p class="p3" id="details"></p>


<script>
function rebootButton() {
  document.getElementById("statusdetails").innerHTML = "Invoking Reboot ...";
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/reboot", true);
  xhr.send();
  window.open("/reboot","_self");
}
function listFilesButton() {
  xmlhttp=new XMLHttpRequest();
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();
  document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
  document.getElementById("details").innerHTML = xmlhttp.responseText;
}
function downloadDeleteButton(filename, action) {
  var urltocall = "/file?name=/" + filename + "&action=" + action;
  xmlhttp=new XMLHttpRequest();
  if (action == "delete") {
    xmlhttp.open("GET", urltocall, false);
    xmlhttp.send();
    document.getElementById("status").innerHTML = xmlhttp.responseText;
    xmlhttp.open("GET", "/listfiles", false);
    xmlhttp.send();
    document.getElementById("details").innerHTML = xmlhttp.responseText;
  }
  if (action == "download") {
    document.getElementById("status").innerHTML = "";
    window.open(urltocall,"_blank");
  }
}

function changeDirectory(directory) {
  var urltocall = "/changedirectory?dir=/" + directory;
  xmlhttp=new XMLHttpRequest();
  xmlhttp.open("GET", urltocall, false);
  xmlhttp.send();
  document.getElementById("status").innerHTML = xmlhttp.responseText;
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();
  document.getElementById("details").innerHTML = xmlhttp.responseText;
}

function showUploadButtonFancy() {
  document.getElementById("detailsheader").innerHTML = "<h3>Upload File<h3>"
  document.getElementById("status").innerHTML = "";
  var uploadform = "<form method = \"POST\" action = \"/\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"data\"/><input type=\"submit\" name=\"upload\" value=\"Upload\" title = \"Upload File\"></form>"
  document.getElementById("details").innerHTML = uploadform;
  var uploadform =
  "<form id=\"upload_form\" enctype=\"multipart/form-data\" method=\"post\">" +
  "<input type=\"file\" name=\"file1\" id=\"file1\" onchange=\"uploadFile()\"><br>" +
  "<progress id=\"progressBar\" value=\"0\" max=\"100\" style=\"width:300px;\"></progress>" +
  "<h3 id=\"status\"></h3>" +
  "<p id=\"loaded_n_total\"></p>" +
  "</form>";
  document.getElementById("details").innerHTML = uploadform;
}
function _(el) {
  return document.getElementById(el);
}
function uploadFile() {
  var file = _("file1").files[0];
  // alert(file.name+" | "+file.size+" | "+file.type);
  var formdata = new FormData();
  formdata.append("file1", file);
  var ajax = new XMLHttpRequest();
  ajax.upload.addEventListener("progress", progressHandler, false);
  ajax.addEventListener("load", completeHandler, false); // doesnt appear to ever get called even upon success
  ajax.addEventListener("error", errorHandler, false);
  ajax.addEventListener("abort", abortHandler, false);
  ajax.open("POST", "/");
  ajax.send(formdata);
}
function progressHandler(event) {
  //_("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes of " + event.total; // event.total doesnt show accurate total file size
  _("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes";
  var percent = (event.loaded / event.total) * 100;
  _("progressBar").value = Math.round(percent);
  _("status").innerHTML = Math.round(percent) + "% uploaded... please wait";
  if (percent >= 100) {
    _("status").innerHTML = "Please wait, writing file to filesystem";
  }
}
function completeHandler(event) {
  _("status").innerHTML = "Upload Complete";
  _("progressBar").value = 0;
  xmlhttp=new XMLHttpRequest();
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();
  document.getElementById("status").innerHTML = "File Uploaded";
  document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
  document.getElementById("details").innerHTML = xmlhttp.responseText;
}
function errorHandler(event) {
  _("status").innerHTML = "Upload Failed";
}
function abortHandler(event) {
  _("status").innerHTML = "inUpload Aborted";
}
</script>

</body>
</html>
)rawliteral";

// reboot.html base upon https://gist.github.com/Joel-James/62d98e8cb3a1b6b05102
const char reboot_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="UTF-8">
</head>
<body>
<h3>
  Rebooting, returning to main page in <span id="countdown">30</span> seconds
</h3>
<script type="text/javascript">
  var seconds = 20;
  function countdown() {
    seconds = seconds - 1;
    if (seconds < 0) {
      window.location = "/";
    } else {
      document.getElementById("countdown").innerHTML = seconds;
      window.setTimeout("countdown()", 1000);
    }
  }
  countdown();
</script>
</body>
</html>
)rawliteral";
