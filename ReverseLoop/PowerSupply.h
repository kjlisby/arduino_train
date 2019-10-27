#ifndef POWER_SUPPLY_H
#define POWER_SUPPLY_H

#include <stdint.h>

class PowerSupply {
	public:
		void Init(uint8_t DacPin, uint8_t StatusPin, uint8_t RelayPin);
		void SetSpeed(int Speed);
		void STOP();
		int  GetActualSpeed();
		int  GetSpeed();
		void ReverseDirection(bool Reverse);
		bool GetStatus();
		void ResetStatus();
		void Loop();
	private:
		uint8_t dac_pin;
		uint8_t status_pin;
		uint8_t relay_pin;
		int     current_speed = 0; // 0-255 equals 0-3.3V equals 0 - 18V
		int     desired_speed = 0;
		unsigned long last_millis;
		bool    direction_reversed = false;
		bool    PSUstatus = true;
		void Disable();
		void PollShortcircuit();
};

#endif
