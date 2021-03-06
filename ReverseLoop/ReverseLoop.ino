#include "SDWebServer.h"
#include "OTAHandler.h"
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
#define TURNOUT1_PIN    4
#define TURNOUT2_PIN   12
#define TURNOUT3_PIN   13
#define TURNOUT4_PIN   14
#define TURNOUT5_PIN   15
#define TURNOUT6_PIN   16
#define TRAIN_SENSOR1  36 // This port has no internal pullup, so an external 10-100k is needed
#define TRAIN_SENSOR2  34 // This port has no internal pullup, so an external 10-100k is needed
#define TRAIN_SENSOR3  39 // This port has no internal pullup, so an external 10-100k is needed
#define TRAIN_SENSOR4  35 // This port has no internal pullup, so an external 10-100k is needed
#define PSU1_DAC_PIN   25
#define PSU1_ADC_PIN   32
#define PSU1_POL_PIN   17
#define PSU2_DAC_PIN   26
#define PSU2_ADC_PIN   33
#define PSU2_POL_PIN   27
SDWebServer   *WS;
OTAHandler    *OH;
AjaxHandler   *AH;
PowerSupply   *PSU1;
PowerSupply   *PSU2;
Turnout       *TU1;
Turnout       *TU2;
Turnout       *TU3;
Turnout       *TU4;
Turnout       *TU5;
Turnout       *TU6;
TrainDetector *TD1;
TrainDetector *TD2;
TrainDetector *TD3;
TrainDetector *TD4;
//ReverseLoop   *RL1;
ReverseLoop   *RL2;
bool          AutoMode = false;
String        Train1Position = "B7";
String        Train2Position = "B2";

void InitPowerSupplies() {
	PSU1 = new PowerSupply();
	PSU1->Init(PSU1_DAC_PIN, PSU1_ADC_PIN, PSU1_POL_PIN);
	PSU2 = new PowerSupply();
	PSU2->Init(PSU2_DAC_PIN, PSU2_ADC_PIN, PSU2_POL_PIN);
}

void SDWebServer_handleNotFound() {
	WS->loadFromSdCard(WS->getServer()->uri());
}

void InitTrainDetectors () {
	TD1 = new TrainDetector();
	TD1->Init(TRAIN_SENSOR1, "B8");
	TD2 = new TrainDetector();
	TD2->Init(TRAIN_SENSOR2, "B7");
	// TD3 = new TrainDetector();
	// TD3->Init(TRAIN_SENSOR3, "B?");
	// TD4 = new TrainDetector();
	// TD4->Init(TRAIN_SENSOR4, "B?");
}

void InitTurnouts() {
	TU1 = new Turnout();
	TU1->Init(TURNOUT1_PIN);
	TU2 = new Turnout();
	TU2->Init(TURNOUT2_PIN);
	TU3 = new Turnout();
	TU3->Init(TURNOUT3_PIN);
	TU4 = new Turnout();
	TU4->Init(TURNOUT4_PIN);
	TU5 = new Turnout();
	TU5->Init(TURNOUT5_PIN);
	TU6 = new Turnout();
	TU6->Init(TURNOUT6_PIN);
}


unsigned long NextLoopTime = 0;
void LoopDone() {
	Serial.println("-----------LoopDone");
	Train1Position = "B7";
	NextLoopTime = millis()+random(10000, 50000);
}
void StartLoop() {
	Serial.println("-----------StartLoop");
	NextLoopTime = 0;
	RL2->Attention(&Train1Position);
}
void ToggleAutoMode() {
	if (AutoMode) {
		analogWrite(LED_BUILTIN,0);
		AutoMode = false;
		NextLoopTime = 0;
	} else {
		analogWrite(LED_BUILTIN,100);
		AutoMode = true;
		NextLoopTime = millis()+random(10000, 50000);
	}
}

void setup(void) {
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	Serial.print("\n");
	WS = new SDWebServer();
	WS->Init(SD_CS_PIN);
	WS->getServer()->onNotFound(SDWebServer_handleNotFound);
	OH = new OTAHandler();
	OH->Init();
	AH = new AjaxHandler();
	AH->Init(WS->getServer());
	InitPowerSupplies();
	InitTurnouts();
	InitTrainDetectors();
//	RL1 = new ReverseLoop();
//	RL1->Init(PSU1, TU3, TD1, TD2, LoopDone);
	RL2 = new ReverseLoop();
	RL2->Init(WS, PSU2, TU6, TD1, TD2, LoopDone);
	pinMode ( LED_BUILTIN, OUTPUT );
	analogWrite ( LED_BUILTIN, 0 );
}

void loop(void) {
	WS->getServer()->handleClient();
	WS->Loop();
	OH->Loop();
	TD1->Loop();
	TD2->Loop();
	PSU1->Loop();
	PSU2->Loop();
	TU1->Loop();
	TU2->Loop();
	TU3->Loop();
	TU4->Loop();
	TU5->Loop();
	TU6->Loop();
	AH->Loop();
//	RL1->Loop();
	RL2->Loop();
	if (NextLoopTime > 0) {
		if (AutoMode && millis() > NextLoopTime) {
			StartLoop();
		}
	}
}
