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
    
    #include "MySecretPassword.h"
    // You must have a file named MySecretPassword.h
    // and that file must contain the SSID and the password
    // for your WIFI. Syntax as follows:
    // const char* ssid = "xxxxxx";
    // const char* password = "yyyyyyy";

    WebServer *server;
    IPAddress *apIP;
    IPAddress *staticIP;
    IPAddress *gateway;
    IPAddress *subnet;
    void returnOK();
    void returnFail (String msg);
};

#endif
