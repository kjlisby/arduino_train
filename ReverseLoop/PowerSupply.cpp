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
    Serial.print("PowerSupply::SetVoltage ");
    Serial.println(Voltage);
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
  if (this->status_pin == 32 && millis() % 30000 == 0) {
    Serial.print("Short circuit detection measure: ");
    Serial.println(value);
  }
	if (value > 1000 && millis() > 5000) {
		this->Disable();
	}
}

void PowerSupply::Disable() {
  if (this->PSUstatus) {
    Serial.print("PowerSupply::Disable ");
    Serial.println(this->status_pin);
  }
  this->storedVoltage = this->voltage;
  this->SetVoltage(0);
  this->PSUstatus = false;
}
