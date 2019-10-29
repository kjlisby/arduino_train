#include "SDWebServer.h"

void SDWebServer::returnOK () {
  this->server->send(200, "text/plain", "");
}

void SDWebServer::returnFail (String msg) {
  this->server->send(500, "text/plain", msg + "\r\n");
}

bool SDWebServer::loadFromSdCard (String path) {
  Serial.println("SDWebServer::loadFromSdCard");
  Serial.println(path);
  String dataType = "text/plain";
  if (path.endsWith("/")) {
    path += "index.htm";
  }
  if (path.endsWith(".src")) {
    path = path.substring(0, path.lastIndexOf("."));
  } else if (path.endsWith(".htm")) {
    dataType = "text/html";
  } else if (path.endsWith(".css")) {
    dataType = "text/css";
  } else if (path.endsWith(".js")) {
    dataType = "application/javascript";
  } else if (path.endsWith(".png")) {
    dataType = "image/png";
  } else if (path.endsWith(".gif")) {
    dataType = "image/gif";
  } else if (path.endsWith(".jpg")) {
    dataType = "image/jpeg";
  } else if (path.endsWith(".ico")) {
    dataType = "image/x-icon";
  } else if (path.endsWith(".xml")) {
    dataType = "text/xml";
  } else if (path.endsWith(".pdf")) {
    dataType = "application/pdf";
  } else if (path.endsWith(".zip")) {
    dataType = "application/zip";
  }
  Serial.println(path);
  File dataFile = SD.open(path.c_str());
  if (dataFile.isDirectory()) {
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }
  if (!dataFile) {
    Serial.println("OPENING FILE FAILED");
    return false;
  }
  if (server->hasArg("download")) {
    dataType = "application/octet-stream";
  }
  if (this->server->streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println("Sent less data than expected!");
  }
  dataFile.close();
  return true;
}

SDWebServer::SDWebServer() {
  this->server   = new WebServer(80);
  this->apIP     = new IPAddress(42, 42, 42, 42);
  this->staticIP = new IPAddress(192,168,1,200);
  this->gateway  = new IPAddress(192,168,1,1);
  this->subnet   = new IPAddress(255,255,255,0);
  WiFi.persistent(false);
//  WiFi.mode(WIFI_AP_STA);
  WiFi.config(*(this->staticIP), *(this->gateway), *(this->subnet));
//  WiFi.softAPConfig(*(this->apIP), *(this->apIP), *(this->subnet));
//  WiFi.softAP(this->ssid, this->password); 
//  IPAddress myAPIP = WiFi.softAPIP(); 
//  Serial.print("AP IP address: "); 
//  Serial.println(myAPIP); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Wait for connection
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
    delay(500);
  }
  if (i == 21) {
    Serial.print("Could not connect to");
    Serial.println(ssid);
    while (1) {
      delay(500);
      ESP.restart();
    }
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

bool INITIALIZED = false;
void SDWebServer::Init(uint8_t SDPin) {
  if (INITIALIZED) return;
  INITIALIZED = true;
  
  this->sd_pin = SDPin;
  server->begin();
  Serial.println("HTTP server started");
  if (SD.begin(this->sd_pin)) {
    Serial.println("SD Card initialized.");
  }
  loadFromSdCard("/");
}

WebServer *SDWebServer::getServer() {
  return server;
}
