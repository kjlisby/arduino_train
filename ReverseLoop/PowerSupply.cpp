#include "PowerSupply.h"
#include <Arduino.h>

void PowerSupply::Init(uint8_t DacPin, uint8_t StatusPin, uint8_t RelayPin) {
	this->dac_pin = DacPin;
	this->status_pin = StatusPin;
	this->relay_pin = RelayPin;
	pinMode(this->relay_pin, OUTPUT);
	this->STOP();
}

void PowerSupply::SetSpeed(int Speed) {
	if (this->PSUstatus) {
		Serial.print("PowerSupply::SetSpeed ");
		Serial.println(Speed);
		if (Speed > 255 || Speed < -255) {
			Serial.println("ILLEGAL SPEED");
			return;
		}
		this->desired_speed = Speed;
	}
}

void PowerSupply::STOP() {
  dacWrite(this->dac_pin, 0);
  this->current_speed = 0;
  this->desired_speed = 0;
  this->direction_reversed = false;
  digitalWrite(this->relay_pin, 0);
}
   
void PowerSupply::ReverseDirection(bool Reverse) {
  digitalWrite(this->relay_pin, Reverse);
  this->direction_reversed = Reverse;
  this->current_speed *= -1;
  this->desired_speed *= -1;
}

int PowerSupply::GetSpeed() {
	return this->current_speed;
}

bool PowerSupply::GetStatus() {
	return this->PSUstatus;
}

void PowerSupply::ResetStatus() {
	this->PSUstatus = true;
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
  this->STOP();
  this->PSUstatus = false;
}

void PowerSupply::Loop() {
  this->PollShortcircuit();
  unsigned long now = millis();
  if (this->current_speed != this->desired_speed && now%20 == 0 && now != this->last_millis) {
    this->last_millis = now;
    if (this->current_speed > this->desired_speed) {
      this->current_speed--;
    } else {
      this->current_speed++;
    }
    this->direction_reversed = (current_speed < 0);
    digitalWrite(this->relay_pin, direction_reversed);
    dacWrite(this->dac_pin, abs(current_speed));
  }
}
