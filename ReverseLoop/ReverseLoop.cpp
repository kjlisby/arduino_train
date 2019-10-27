#include "ReverseLoop.h"

void ReverseLoop::Init(PowerSupply *PSU, Turnout *TU, TrainDetector *TD, void (*TrainDetectedCallback)()) {
	this->psu = PSU;
	this->tu = TU;
	this->td = TD;
	this->tdcb = TrainDetectedCallback;
	this->active = false;
	this->wait_for_train_to_stop = false;
	this->wait_millis = 0;
}

void ReverseLoop::Attention(String *TrainPosition) {
	Serial.println("--------------- ReverseLoop ATTENTION at "+*TrainPosition);
	this->train_position = TrainPosition;
	this->psu->SetSpeed(0);
	this->psu->ReverseDirection(false);
	this->tu->Close();
	this->active = true;
	this->wait_millis = 5000+millis();
}

void ReverseLoop::Loop() {
	if (this->active && this->wait_millis > 0) {
		if (millis() >= this->wait_millis) {
			Serial.println("--------------- ReverseLoop forward at half speed");
			this->wait_millis = 0;
			this->psu->SetSpeed(128);
			Serial.println("--------------- ReverseLoop forward at half speed - out of this->psu->SetSpeed");
		}
	} else if (!this->active && this->wait_millis > 0) {
		if (millis() >= this->wait_millis) {
			Serial.println("--------------- ReverseLoop stop at destination");
			this->wait_millis = 0;
			this->psu->SetSpeed(0);
			this->wait_for_train_to_stop = true;
		}
	} else if (this->active) {
		if (this->td->TrainSeen()) {
			Serial.println("--------------- ReverseLoop reversing at block "+this->td->BlockName());
			*(this->train_position) = this->td->BlockName();
			Serial.println("--------------- ReverseLoop reversing *(this->train_position)="+*(this->train_position));
			this->tu->Throw();
			this->psu->ReverseDirection(true);
			this->td->AcknowledgeTrainSeen();
			this->active = false;
			this->wait_millis = 15000+millis();
		}
	} else if (this->wait_for_train_to_stop) {
		if (this->psu->GetActualSpeed() == 0) {
			this->wait_for_train_to_stop = false;
			this->td->AcknowledgeTrainSeen();
			this->tdcb();
		}
	}
}
