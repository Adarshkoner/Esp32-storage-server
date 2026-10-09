/*
ESP32 Mini Storage Server
​Features:
​Connects to Wi-Fi OR creates its own Access Point ("ESP32-Storage")
​Web-based file browser (List, Upload, Download, Delete)
​Interfaced with MicroSD Card via SPI
*/
​#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <SD.h>
​// --- Configuration ---
const char* ap_ssid = "ESP32-Storage-Server";
const char* ap_password = "hacklifestorage"; // Minimum 8 characters
​// SD Card SPI Pin Definitions
#define SD_CS   5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK  18
​#define STATUS_LED 2
​WebServer server(80);
File uploadFile;
​// --- Web Interface HTML ---
const char HTML_INDEX[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 Storage Server</title>
<style>
body { font-family: Arial, sans-serif; margin: 20px; background-color: #f4f4f9; color: #333; }
h2 { color: #ec3750; }
.card { background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); margin-bottom: 20px; }
table { width: 100%; border-collapse: collapse; margin-top: 10px; }
th, td { text-align: left; padding: 10px; border-bottom: 1px solid #ddd; }
th { background-color: #ec3750; color: white; }
.btn { padding: 6px 12px; background: #ec3750; color: white; border: none; border-radius: 4px; text-decoration: none; cursor: pointer; }
.btn-del { background: #d9534f; }
input[type=file] { margin-bottom: 10px; }
</style>
</head>
<body>
<h2>📁 ESP32 File Storage Server</h2>
<div class="card">
<h3>Upload File</h3>
<form method="POST" action="/upload" enctype="multipart/form-data">
<input type="file" name="upload" required>


<input type="submit" value="Upload File" class="btn">
</form>
</div>
<div class="card">
<h3>Stored Files</h3>
<div id="file-list">Loading files...</div>
</div>
​<script>
function loadFiles() {
fetch('/list')
.then(response => response.json())
.then(data => {
let html = '<table><tr><th>Filename</th><th>Size (Bytes)</th><th>Action</th></tr>';
data.forEach(file => {
html += <tr> <td>${file.name}</td> <td>${file.size}</td> <td> <a href="/download?name=${encodeURIComponent(file.name)}" class="btn">Download</a> <a href="/delete?name=${encodeURIComponent(file.name)}" class="btn btn-del" onclick="return confirm('Delete file?')">Delete</a> </td> </tr>;
});
html += '</table>';
document.getElementById('file-list').innerHTML = html;
});
}
window.onload = loadFiles;
</script>
</body>
</html>
)rawliteral";
​// --- Helper Functions ---
String getContentType(String filename) {
if (filename.endsWith(".htm") || filename.endsWith(".html")) return "text/html";
else if (filename.endsWith(".css")) return "text/css";
else if (filename.endsWith(".js")) return "application/javascript";
else if (filename.endsWith(".png")) return "image/png";
else if (filename.endsWith(".gif")) return "image/gif";
else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
else if (filename.endsWith(".ico")) return "image/x-icon";
else if (filename.endsWith(".xml")) return "text/xml";
else if (filename.endsWith(".pdf")) return "application/x-pdf";
else if (filename.endsWith(".zip")) return "application/x-zip";
else if (filename.endsWith(".gz")) return "application/x-gzip";
return "text/plain";
}
​void handleRoot() {
server.send(200, "text/html", HTML_INDEX);
}
​void handleFileList() {
File root = SD.open("/");
String output = "[";
File file = root.openNextFile();
bool first = true;
​while (file) {
if (!file.isDirectory()) {
if (!first) output += ",";
output += "{"name":"" + String(file.name()) + "","size":" + String(file.size()) + "}";
first = false;
}
file = root.openNextFile();
}
output += "]";
server.send(200, "application/json", output);
}
​void handleFileDownload() {
if (!server.hasArg("name")) {
server.send(400, "text/plain", "Bad Request: Missing 'name' param");
return;
}
String filename = server.arg("name");
if (!filename.startsWith("/")) filename = "/" + filename;
​if (!SD.exists(filename)) {
server.send(404, "text/plain", "404: File Not Found");
return;
}
​File dataFile = SD.open(filename, FILE_READ);
String dataType = getContentType(filename);
​digitalWrite(STATUS_LED, HIGH);
server.streamFile(dataFile, dataType);
dataFile.close();
digitalWrite(STATUS_LED, LOW);
}
​void handleFileDelete() {
if (!server.hasArg("name")) {
server.send(400, "text/plain", "Bad Request");
return;
}
String filename = server.arg("name");
if (!filename.startsWith("/")) filename = "/" + filename;
​if (SD.exists(filename)) {
SD.remove(filename);
server.sendHeader("Location", "/");
server.send(303);
} else {
server.send(404, "text/plain", "File Not Found");
}
}
​void handleFileUpload() {
HTTPUpload& upload = server.upload();
if (upload.status == UPLOAD_FILE_START) {
String filename = upload.filename;
if (!filename.startsWith("/")) filename = "/" + filename;
digitalWrite(STATUS_LED, HIGH);
uploadFile = SD.open(filename, FILE_WRITE);
} else if (upload.status == UPLOAD_FILE_WRITE) {
if (uploadFile) {
uploadFile.write(upload.buf, upload.currentSize);
}
} else if (upload.status == UPLOAD_FILE_END) {
if (uploadFile) {
uploadFile.close();
}
digitalWrite(STATUS_LED, LOW);
}
}
​void setup() {
Serial.begin(115200);
pinMode(STATUS_LED, OUTPUT);
digitalWrite(STATUS_LED, LOW);
​// Initialize SPI & SD Card
SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);
if (!SD.begin(SD_CS)) {
Serial.println("Error: MicroSD Card Mount Failed!");
return;
}
Serial.println("MicroSD Card initialized successfully.");
​// Start Access Point Mode
WiFi.softAP(ap_ssid, ap_password);
IPAddress myIP = WiFi.softAPIP();
Serial.print("Access Point Created. IP Address: ");
Serial.println(myIP);
​// Define Server Routes
server.on("/", HTTP_GET, handleRoot);
server.on("/list", HTTP_GET, handleFileList);
server.on("/download", HTTP_GET, handleFileDownload);
server.on("/delete", HTTP_GET, handleFileDelete);
​// File upload handlers
server.on("/upload", HTTP_POST, 
​{
server.sendHeader("Location", "/");
server.send(303);
}, handleFileUpload);
​server.begin();
Serial.println("HTTP server started.");
}
​void loop() {
server.handleClient();
}
