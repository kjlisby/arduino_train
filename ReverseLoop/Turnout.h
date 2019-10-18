#ifndef TURNOUT_H
#define TURNOUT_H

#include <stdint.h>
#include <ESP32Servo.h>

class Turnout {
	public:
		void Init(uint8_t ServoPin);
		void Throw();
		void Close();
		void Loop();
	private:
		uint8_t servo_pin;
		Servo turnout;
		int pos_act;
		int pos_dest;
		unsigned long last_millis = 0;
		int detach_timer = 0;
};

#endif
