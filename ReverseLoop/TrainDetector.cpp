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
	return true;
}

void TrainDetector::AcknowledgeTrainSeen () {
	this->trainDetectionMillis = 0;
}

void TrainDetector::Init (uint8_t Pin, volatile unsigned long * TrainDetectionTimeVariable) {
	this->reed_pin = Pin;
  this->trainDetectionMillis = TrainDetectionTimeVariable;
	pinMode (this->reed_pin, INPUT_PULLUP); 
}
