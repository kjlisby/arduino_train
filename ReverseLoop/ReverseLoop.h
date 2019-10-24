#ifndef REVERSELOOP_H
#define REVERSELOOP_H

#include <stdint.h>
#include "PowerSupply.h"
#include "Turnout.h"
#include "TrainDetector.h"

class ReverseLoop {
	public:
		void Init(PowerSupply *PSU, Turnout *TU, TrainDetector *TD, void (*TrainDetectedCallback)());
		void Attention();
    void Loop();
	private:
    PowerSupply *psu;
		Turnout *tu;
		TrainDetector *td;
		void (*tdcb)();
    bool active;
    unsigned long wait_millis;
};

#endif
