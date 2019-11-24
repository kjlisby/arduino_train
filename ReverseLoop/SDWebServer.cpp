#include "SDWebServer.h"
#include <WebSocketsServer.h>
WebSocketsServer webSocket = WebSocketsServer(81);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
   if (type == WStype_TEXT){
    for(int i = 0; i < length; i++) Serial.print((char) payload[i]);
    Serial.println();
   }
}
void SDWebServer::sendMessage (String message) {
  webSocket.broadcastTXT(message);
}

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

void SDWebServer::setupNetwork(bool AP) {
  this->server   = new WebServer(80);
  this->apIP     = new IPAddress(42, 42, 42, 42);
  this->staticIP = new IPAddress(192,168,1,200);
  this->gateway  = new IPAddress(192,168,1,1);
  this->subnet   = new IPAddress(255,255,255,0);
  WiFi.persistent(false);
  if (AP) {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(*(this->apIP), *(this->apIP), *(this->subnet));
    WiFi.softAP(APssid, APpassword);
    IPAddress myAPIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myAPIP);
  } else {
    WiFi.config(*(this->staticIP), *(this->gateway), *(this->subnet));
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
}

bool INITIALIZED = false;
void SDWebServer::Init(uint8_t SDPin) {
  if (INITIALIZED) return;
  INITIALIZED = true;
  Serial.println("SDWebServer::Init");
  this->sd_pin = SDPin;
  if (SD.begin(this->sd_pin)) {
    Serial.println("SD Card initialized.");
  }
  File dataFile = SD.open("/APmode");
  if (dataFile) {
    Serial.println("AP mode");
    dataFile.close();
    this->setupNetwork(true);
  } else {
    Serial.println("STA mode");
    this->setupNetwork(false);
  }
  server->begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("HTTP server started");
}

void SDWebServer::Loop() {
  webSocket.loop();
}

WebServer *SDWebServer::getServer() {
  return server;
}
