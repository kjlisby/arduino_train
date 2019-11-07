#ifndef REVERSELOOP_H
#define REVERSELOOP_H

#include <stdint.h>
#include "PowerSupply.h"
#include "Turnout.h"
#include "TrainDetector.h"

enum Status {Idle, Starting, LookForFirstTD, InLoop, RunToPlatform, Stopping};

class ReverseLoop {
	public:
		void Init(PowerSupply *PSU, Turnout *TU, TrainDetector *TD, TrainDetector *TD2, void (*TrainDetectedCallback)());
		void Attention(String *TrainPosition);
		void Loop();
	private:
		PowerSupply   *psu;
		Turnout       *tu;
		TrainDetector *td, *td2;
		String        *train_position;
		void (*tdcb)();
		Status        stat;
		unsigned long wait_millis;
};

#endif
