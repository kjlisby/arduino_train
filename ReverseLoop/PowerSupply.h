#ifndef POWER_SUPPLY_H
#define POWER_SUPPLY_H

#include <stdint.h>

class PowerSupply {
	public:
    void Init(uint8_t DacPin, uint8_t StatusPin, uint8_t RelayPin);
		void SetVoltage(uint8_t Voltage);
		void SetPolarity(bool Polarity);
		uint8_t GetVoltage();
		bool GetPolarity();
		bool GetStatus();
    void Disable();
		void ResetStatus();
	private:
		uint8_t dac_pin;
		uint8_t status_pin;
		uint8_t relay_pin;
		uint8_t voltage  = 0; // 0-255 equals 0-3.3V equals 0 - 18V
		uint8_t storedVoltage = 0;
    bool    polarity = true;
    bool    PSUstatus = true;
};

#endif
