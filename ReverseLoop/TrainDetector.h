#ifndef TRAINDETECTOR_H
#define TRAINDETECTOR_H

#include <stdint.h>
#include <Arduino.h>

class TrainDetector {
	public:
		void   Init (uint8_t Pin, volatile unsigned long * TrainDetectionTimeVariable, String BlockName);
		bool   TrainSeen ();
		void   AcknowledgeTrainSeen ();
		String BlockName();
	private:
		uint8_t reed_pin;
		volatile unsigned long * trainDetectionMillis;
		String block_name;
};

#endif
