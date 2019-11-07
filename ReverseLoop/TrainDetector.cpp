#include "TrainDetector.h"
#include <Arduino.h>

bool TrainDetector::TrainSeen () {
	if (*(this->trainDetectionMillis) <= 0) {
		return false;
	}
	if (millis()-*(this->trainDetectionMillis) < 10000) {
		return true;
	}
	*(this->trainDetectionMillis) = 0;
	return false;
}

void TrainDetector::AcknowledgeTrainSeen () {
	*(this->trainDetectionMillis) = 0;
}

String TrainDetector::BlockName() {
	return this->block_name;
}

void TrainDetector::Init (uint8_t Pin, volatile unsigned long * TrainDetectionTimeVariable, String BlockName) {
	this->block_name = BlockName;
	this->reed_pin = Pin;
	this->trainDetectionMillis = TrainDetectionTimeVariable;
	pinMode (this->reed_pin, INPUT_PULLUP); 
}
