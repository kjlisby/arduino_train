#include "Turnout.h"

void Turnout::Init(uint8_t ServoPin) {
	this->servo_pin = ServoPin;
	this->turnout.attach(this->servo_pin);
	this->turnout.write(90);
	this->pos_act  = 90;
	this->pos_dest = 90;
	this->detach_timer = millis()+1000;
}

void Turnout::Throw() {
	Serial.println("SETTING B2 SERVO");
	this->turnout.attach(this->servo_pin);
	this->pos_dest = 145;
}

void Turnout::Close() {
	Serial.println("SETTING B1 SERVO");
	this->turnout.attach(this->servo_pin);
	this->pos_dest = 50;
}

void Turnout::Loop() {
    unsigned long now = millis();
    if (this->pos_act != this->pos_dest && now%20 == 0 && now != this->last_millis) {
      this->last_millis = now;
      if (this->pos_act > this->pos_dest) {
        this->pos_act--;
      } else {
        this->pos_act++;
      }
      this->turnout.write(this->pos_act);
      if (this->pos_act == this->pos_dest) {
        this->detach_timer = millis()+1000;
      }
    }
    if (this->detach_timer > 0) {
      if (millis() > this->detach_timer) {
        this->detach_timer = 0;
        this->turnout.detach();
      }
    }
}