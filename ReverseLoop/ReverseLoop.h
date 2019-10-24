#ifndef REVERSELOOP_H
#define REVERSELOOP_H

#include <stdint.h>
#include "Turnout.h"
#include "TrainDetector.h"

class ReverseLoop {
	public:
		void Init(Turnout *TU, TrainDetector *TD, void (*TrainDetectedCallback)() CB);
		void Attention();
	private:
		Turnout *tu;
		TrainDetector *td;
		void (*tdcb)();
};

#endif
