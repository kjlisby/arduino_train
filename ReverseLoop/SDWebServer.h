#ifndef SD_WEB_SERVER_H
#define SD_WEB_SERVER_H

#include <stdint.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SD.h>

class SDWebServer {
  public:
    SDWebServer();
    void Init(uint8_t SDPin);
    WebServer *getServer();
    bool loadFromSdCard (String path);
  private:
    uint8_t sd_pin;
    const char* ssid = "TDC-F4B8";
    const char* password = "5214CD15C";
    WebServer *server;
    IPAddress *apIP;
    IPAddress *staticIP;
    IPAddress *gateway;
    IPAddress *subnet;
    void returnOK();
    void returnFail (String msg);
};

#endif
