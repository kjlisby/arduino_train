#ifndef REVERSELOOP_H
#define REVERSELOOP_H

#include <stdint.h>
#include "PowerSupply.h"
#include "Turnout.h"
#include "TrainDetector.h"

class ReverseLoop {
	public:
		void Init(PowerSupply *PSU, Turnout *TU, TrainDetector *TD, void (*TrainDetectedCallback)());
		void Attention(String *TrainPosition);
		void Loop();
	private:
		PowerSupply   *psu;
		Turnout       *tu;
		TrainDetector *td;
		String        *train_position;
		void (*tdcb)();
		bool active;
		bool wait_for_train_to_stop;
		unsigned long wait_millis;
};

#endif
