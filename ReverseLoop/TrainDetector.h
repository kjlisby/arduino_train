#ifndef TRAINDETECTOR_H
#define TRAINDETECTOR_H

#include <stdint.h>

class TrainDetector {
	public:
		void Init (uint8_t Pin, volatile unsigned long * TrainDetectionTimeVariable);
		bool TrainSeen ();
		void AcknowledgeTrainSeen ();
	private:
		uint8_t reed_pin;
    volatile unsigned long * trainDetectionMillis;
};

#endif
