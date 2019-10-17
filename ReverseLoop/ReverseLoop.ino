/*
  SDWebServer class

  Heavily inspired by the Example WebServer with SD Card backend for esp8266
  Which is Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Have a FAT Formatted SD Card connected to the SPI port of the ESP8266
  The web root is the SD Card root folder
  File extensions with more than 3 charecters are not supported by the SD Library
  File Names longer than 8 charecters will be truncated by the SD library, so keep filenames shorter
  index.htm is the default index (works on subfolders as well)

  upload the contents of SdRoot to the root of the SDcard and access the editor by going to http://esp8266sd.local/edit

*/
#include "SDWebServer.h"
#include "PowerSupply.h"


/*--------------------------------------------------------------------------------
 * ANOTHER CLASS TO CONTROL SOMETHING
 */

#include <ESP32Servo.h>

#define ledPin 2 //Built-in LED Which lights up when the output is LOW
const byte reedPin = 5; //Reed switch on GPIO5 / D1
#define servoPin 4 // Servo on GPIO4 / D2
#define sporskiftePin 0 // GPIO0 / D3
int counter = 0;
Servo myservo;
Servo sporskifte;
int sporskifte_act;
int sporskifte_dest;
unsigned long sporskifte_last_millis = 0;
int sporskifte_detach_timer = 0;
bool ledState = false;
int reedState = false;

class MyHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/ajax_");
  }

  bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri) {   
    Serial.println("AJAX GETINPUT");
    Serial.println(requestUri);

    String cmdarray[10];
    int index = 0;
    int startindex = 0;
    int cmd_length = 0;
    while (index < requestUri.length()) {
      if (requestUri[index] == '_') {
        cmdarray[cmd_length] = requestUri.substring(startindex, index);
        startindex = index+1;
        cmd_length++;
      }
      index++;
    }
    cmdarray[cmd_length] = requestUri.substring(startindex);
    index++;
    cmd_length++;
    
    for (int i=0; i<cmd_length; i++) {
      Serial.print(i); Serial.print(": ");
      Serial.println(cmdarray[i]);
    }

    if (cmdarray[1].equals("set")) {
      if (cmdarray[2].equals("B1")) {
        Serial.println("SETTING B1 SERVO");
        sporskifte.attach(sporskiftePin);
        sporskifte_dest = 50;
      }
      if (cmdarray[2].equals("B2")) {
        Serial.println("SETTING B2 SERVO");
        sporskifte.attach(sporskiftePin);
        sporskifte_dest = 145;
      }
    }
    server.send(200, "text/plain", "");
  }
} myHandler;


volatile unsigned long trainDetectionMillis = 0;
ICACHE_RAM_ATTR void handleReedInterrupt() {
  Serial.println("handleReedInterrupt");
  trainDetectionMillis = millis();
}
boolean trainSeen() {
//  Serial.print("trainSeen trainDetectionMillis==");
//  Serial.print(trainDetectionMillis);
//  Serial.print(" millis== ");
//  Serial.println(millis());
  if (trainDetectionMillis <= 0) {
    return false;
  }
  if (millis()-trainDetectionMillis < 10000) {
    return true;
  }
  trainDetectionMillis = 0;
  return true;
}

boolean LEDstate = false;
void loopHandler() {
    bool old_reedState = reedState;
//    reedState = !digitalRead(reedPin);
    reedState = trainSeen();
    if (old_reedState != reedState) {
      if (reedState) {
        myservo.write(90);
      } else {
        myservo.write(0);
      }
    }
    counter++;
    if (counter > 500000) {
      counter = 0;
      Serial.print("TOGGLE LED ");
//      digitalWrite (ledPin,!digitalRead(ledPin));
      if (LEDstate) {
        Serial.println("OFF");
        LEDstate = false;
        analogWrite(ledPin,2000);
      } else {
        Serial.println("ON");
        LEDstate = true;
        analogWrite(ledPin,1000);
      }
    }

    unsigned long now = millis();
    if (sporskifte_act != sporskifte_dest && now%20 == 0 && now != sporskifte_last_millis) {
      sporskifte_last_millis = now;
      if (sporskifte_act > sporskifte_dest) {
        sporskifte_act--;
      } else {
        sporskifte_act++;
      }
      sporskifte.write(sporskifte_act);
      if (sporskifte_act == sporskifte_dest) {
        sporskifte_detach_timer = millis()+1000;
      }
    }
    if (sporskifte_detach_timer > 0) {
      if (millis() > sporskifte_detach_timer) {
        sporskifte_detach_timer = 0;
        sporskifte.detach();
      }
    }
}

void initHandler() {
  pinMode ( ledPin, OUTPUT );
  analogWrite ( ledPin, 0 );

  pinMode (reedPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(reedPin), handleReedInterrupt, FALLING);
  
  myservo.attach(servoPin);
  myservo.write(0);
  sporskifte.attach(sporskiftePin);
  sporskifte.write(90);
  sporskifte_act  = 90;
  sporskifte_dest = 90;
  sporskifte_detach_timer = millis()+1000;
}

/*
 * -----------------------------------------------------------------------------------------------------------------------
 */

PowerSupply *PSU1;
PowerSupply *PSU2;
ICACHE_RAM_ATTR void handlePSUStatusInterrupt1() {
  PSU1->Disable();
}
ICACHE_RAM_ATTR void handlePSUStatusInterrupt2() {
  PSU2->Disable();
}
void InitPowerSupplies() {
  PSU1 = new PowerSupply();
  PSU1->Init(25, 27, 28);
  attachInterrupt(digitalPinToInterrupt(27), handlePSUStatusInterrupt1, RISING);
  PSU2 = new PowerSupply();
  PSU2->Init(26, 29, 30);
  attachInterrupt(digitalPinToInterrupt(29), handlePSUStatusInterrupt2, RISING);
}


SDWebServer *WS;
void setup(void) {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.print("\n");
  WS = new SDWebServer();
  WS->getServer()->addHandler(&myHandler);
  WS->Init();
  InitPowerSupplies();
  initHandler();
}

void loop(void) {
  WS->getServer()->handleClient();
  loopHandler();
}
