#include "PowerSupply.h"
#include <Arduino.h>

void PowerSupply::Init(uint8_t DacPin, uint8_t StatusPin, uint8_t RelayPin) {
	this->dac_pin = DacPin;
	this->status_pin = StatusPin;
	this->relay_pin = RelayPin;
	pinMode(this->relay_pin, OUTPUT);
	this->SetVoltage(0);
	this->SetPolarity(true);
}

void PowerSupply::SetVoltage(uint8_t Voltage) {
	if (this->PSUstatus) {
		dacWrite(this->dac_pin, Voltage);
		this->voltage = Voltage;
	}
}

void PowerSupply::SetPolarity(bool Polarity) {
	digitalWrite(this->relay_pin, Polarity);
	this->polarity = Polarity;
}

uint8_t PowerSupply::GetVoltage() {
	return this->voltage;
}

bool PowerSupply::GetPolarity() {
	return this->polarity;
}

bool PowerSupply::GetStatus() {
	return this->PSUstatus;
}

void PowerSupply::ResetStatus() {
	this->PSUstatus = true;
	this->SetVoltage(this->storedVoltage);
}

void PowerSupply::PollShortcircuit() {
	int value = analogRead(this->status_pin);
	if (value > 1000) {
		this->Disable();
	}
}

void PowerSupply::Disable() {
  this->storedVoltage = this->voltage;
  this->SetVoltage(0);
  this->PSUstatus = false;
}
