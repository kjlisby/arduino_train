#include "SDWebServer.h"
#include "PowerSupply.h"
#include "Turnout.h"
#include "TrainDetector.h"


#define SD_CS_PIN       5
#define SD_D1_MOSI_PIN 23
#define SD_SCLK_PIN    18
#define SD_D0_MISO_PIN 19
#define TURNOUT1_PIN  16
#define TRAIN_SENSOR1 34
#define TRAIN_SENSOR2 35
#define TRAIN_SENSOR3 36
#define TRAIN_SENSOR4 39
#define PSU1_DAC_PIN 25
#define PSU1_ADC_PIN 32
#define PSU1_POL_PIN 17
#define PSU2_DAC_PIN 26
#define PSU2_ADC_PIN 33
#define PSU2_POL_PIN 21
SDWebServer *WS;
PowerSupply *PSU1;
PowerSupply *PSU2;
Turnout     *TU;
TrainDetector *TD1;
TrainDetector *TD2;
TrainDetector *TD3;
TrainDetector *TD4;

/*--------------------------------------------------------------------------------
 * ANOTHER CLASS TO CONTROL SOMETHING
 */

#include <ESP32Servo.h>

#define ledPin 2 //Built-in LED Which lights up when the output is LOW
const byte reedPin = 5; //Reed switch on GPIO5 / D1
#define servoPin 4 // Servo on GPIO4 / D2
int counter = 0;
Servo myservo;
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
		  TU->Close();
      }
      if (cmdarray[2].equals("B2")) {
		  TU->Throw();
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
}

void initHandler() {
  pinMode ( ledPin, OUTPUT );
  analogWrite ( ledPin, 0 );

  pinMode (reedPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(reedPin), handleReedInterrupt, FALLING);
}

/*
 * -----------------------------------------------------------------------------------------------------------------------
 */


void InitPowerSupplies() {
  PSU1 = new PowerSupply();
  PSU1->Init(PSU1_DAC_PIN, PSU1_ADC_PIN, PSU1_POL_PIN);
  PSU2 = new PowerSupply();
  PSU2->Init(PSU2_DAC_PIN, PSU2_ADC_PIN, PSU2_POL_PIN);
}

void SDWebServer_handleNotFound() {
  WS->loadFromSdCard(WS->getServer()->uri());
}

volatile unsigned long trainDetectionMillis1 = 0;
ICACHE_RAM_ATTR void handleTrainDetectorInterrupt1() {
	Serial.println("handleTrainDetectorInterrupt1");
	trainDetectionMillis1 = millis();
}
volatile unsigned long trainDetectionMillis2 = 0;
ICACHE_RAM_ATTR void handleTrainDetectorInterrupt2() {
	Serial.println("handleTrainDetectorInterrupt2");
	trainDetectionMillis2 = millis();
}
volatile unsigned long trainDetectionMillis3 = 0;
ICACHE_RAM_ATTR void handleTrainDetectorInterrupt3() {
	Serial.println("handleTrainDetectorInterrupt3");
	trainDetectionMillis3 = millis();
}
volatile unsigned long trainDetectionMillis4 = 0;
ICACHE_RAM_ATTR void handleTrainDetectorInterrupt4() {
	Serial.println("handleTrainDetectorInterrupt4");
	trainDetectionMillis4 = millis();
}
void InitTrainDetectors () {
	TD1 = new TrainDetector();
	TD1->Init(TRAIN_SENSOR1, &trainDetectionMillis1);
	attachInterrupt(digitalPinToInterrupt(TRAIN_SENSOR1), handleTrainDetectorInterrupt1, FALLING);
	TD2 = new TrainDetector();
	TD2->Init(TRAIN_SENSOR2, &trainDetectionMillis2);
	attachInterrupt(digitalPinToInterrupt(TRAIN_SENSOR2), handleTrainDetectorInterrupt2, FALLING);
	TD3 = new TrainDetector();
	TD3->Init(TRAIN_SENSOR3, &trainDetectionMillis3);
	attachInterrupt(digitalPinToInterrupt(TRAIN_SENSOR3), handleTrainDetectorInterrupt3, FALLING);
	TD4 = new TrainDetector();
	TD4->Init(TRAIN_SENSOR4, &trainDetectionMillis4);
	attachInterrupt(digitalPinToInterrupt(TRAIN_SENSOR4), handleTrainDetectorInterrupt4, FALLING);
}

void setup(void) {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.print("\n");
  WS = new SDWebServer();
  WS->getServer()->onNotFound(SDWebServer_handleNotFound);
  WS->getServer()->addHandler(&myHandler);
  WS->Init(SD_CS_PIN);
  InitPowerSupplies();
  TU = new Turnout();
  TU->Init(TURNOUT1_PIN);
  InitTrainDetectors();
  initHandler();
}

void loop(void) {
  WS->getServer()->handleClient();
  PSU1->PollShortcircuit();
  PSU2->PollShortcircuit();
  TU->Loop();
  loopHandler();
}
