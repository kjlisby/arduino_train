#include "SDWebServer.h"
#include "AjaxHandler.h"
#include "PowerSupply.h"
#include "Turnout.h"
#include "TrainDetector.h"
#include "ReverseLoop.h"


#define SD_CS_PIN       5
#define SD_D1_MOSI_PIN 23
#define SD_SCLK_PIN    18
#define SD_D0_MISO_PIN 19
#define I2C_SDA_PIN    21
#define I2C_SCL_PIN    22
#define TURNOUT1_PIN   16
#define TRAIN_SENSOR1  34 // This port has no internal pullup, so an external 10-100k is needed
#define TRAIN_SENSOR2  35 // This port has no internal pullup, so an external 10-100k is needed
#define TRAIN_SENSOR3  36 // This port has no internal pullup, so an external 10-100k is needed
#define TRAIN_SENSOR4  39 // This port has no internal pullup, so an external 10-100k is needed
#define PSU1_DAC_PIN   25
#define PSU1_ADC_PIN   32
#define PSU1_POL_PIN   17
#define PSU2_DAC_PIN   26
#define PSU2_ADC_PIN   33
#define PSU2_POL_PIN   27
SDWebServer   *WS;
AjaxHandler   *AH;
PowerSupply   *PSU1;
PowerSupply   *PSU2;
Turnout       *TU;
TrainDetector *TD1;
TrainDetector *TD2;
TrainDetector *TD3;
TrainDetector *TD4;
ReverseLoop   *RL;


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
//	Serial.println("handleTrainDetectorInterrupt2");
	trainDetectionMillis2 = millis();
}
volatile unsigned long trainDetectionMillis3 = 0;
ICACHE_RAM_ATTR void handleTrainDetectorInterrupt3() {
//	Serial.println("handleTrainDetectorInterrupt3");
	trainDetectionMillis3 = millis();
}
volatile unsigned long trainDetectionMillis4 = 0;
ICACHE_RAM_ATTR void handleTrainDetectorInterrupt4() {
//	Serial.println("handleTrainDetectorInterrupt4");
	trainDetectionMillis4 = millis();
}
void InitTrainDetectors () {
	TD1 = new TrainDetector();
	TD1->Init(TRAIN_SENSOR1, &trainDetectionMillis1);
	attachInterrupt(digitalPinToInterrupt(TRAIN_SENSOR1), handleTrainDetectorInterrupt1, FALLING);
	// TD2 = new TrainDetector();
	// TD2->Init(TRAIN_SENSOR2, &trainDetectionMillis2);
	// attachInterrupt(digitalPinToInterrupt(TRAIN_SENSOR2), handleTrainDetectorInterrupt2, FALLING);
	// TD3 = new TrainDetector();
	// TD3->Init(TRAIN_SENSOR3, &trainDetectionMillis3);
	// attachInterrupt(digitalPinToInterrupt(TRAIN_SENSOR3), handleTrainDetectorInterrupt3, FALLING);
	// TD4 = new TrainDetector();
	// TD4->Init(TRAIN_SENSOR4, &trainDetectionMillis4);
	// attachInterrupt(digitalPinToInterrupt(TRAIN_SENSOR4), handleTrainDetectorInterrupt4, FALLING);
}

bool AutoMode = true;
unsigned long NextLoopTime = 0;
void LoopDone() {
  NextLoopTime = millis()+random(10000, 50000);
}
void StartLoop() {
  NextLoopTime = 0;
  RL->Attention();
}

void setup(void) {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.print("\n");
  WS = new SDWebServer();
  WS->getServer()->onNotFound(SDWebServer_handleNotFound);
  AH = new AjaxHandler();
  AH->Init(WS->getServer());
  WS->Init(SD_CS_PIN);
  InitPowerSupplies();
  TU = new Turnout();
  TU->Init(TURNOUT1_PIN);
  InitTrainDetectors();
  RL = new ReverseLoop();
  RL->Init(PSU1, TU, TD1, LoopDone);
}

void loop(void) {
  WS->getServer()->handleClient();
  PSU1->Loop();
  PSU2->Loop();
  TU->Loop();
  AH->Loop();
  RL->Loop();
  if (NextLoopTime > 0) {
     if (AutoMode && millis() > NextLoopTime) {
       StartLoop();
     }
  }
}
