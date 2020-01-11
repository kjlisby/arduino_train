#include "TrainDetector.h"
#include <Arduino.h>

bool TrainDetector::TrainSeen () {
	if (this->trainDetectionMillis <= 0) {
		return false;
	}
	if (millis()-this->trainDetectionMillis < 2000) {
		return true;
	}
	this->trainDetectionMillis = 0;
	return false;
}

void TrainDetector::AcknowledgeTrainSeen () {
	this->trainDetectionMillis = 0;
}

String TrainDetector::BlockName() {
	return this->block_name;
}

void TrainDetector::Loop () {
	if (!digitalRead(this->reed_pin)) {
		if (this->delayBeforeDetectMillis == 0) {
			this->delayBeforeDetectMillis = millis();
		} else {
			if (millis() > 25+this->delayBeforeDetectMillis) {
				this->trainDetectionMillis = millis();
			}
		}
	} else {
		this->delayBeforeDetectMillis = 0;
	}
}

void TrainDetector::Init (uint8_t Pin, String BlockName) {
	this->delayBeforeDetectMillis = 0;
	this->trainDetectionMillis = 0;
	this->block_name = BlockName;
	this->reed_pin = Pin;
	pinMode (this->reed_pin, INPUT_PULLUP); 
}
