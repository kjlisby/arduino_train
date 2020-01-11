#ifndef TRAINDETECTOR_H
#define TRAINDETECTOR_H

#include <stdint.h>
#include <Arduino.h>
#include "SDWebServer.h"

class TrainDetector {
	public:
		void   Init (uint8_t Pin, String BlockName);
		bool   TrainSeen ();
		void   AcknowledgeTrainSeen ();
		String BlockName();
		void   Loop();
	private:
		unsigned long delayBeforeDetectMillis;
		unsigned long trainDetectionMillis;
		uint8_t reed_pin;
		String block_name;
};

#endif
