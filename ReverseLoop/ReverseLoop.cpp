#include "ReverseLoop.h"

void ReverseLoop::Init(PowerSupply *PSU, Turnout *TU, TrainDetector *TD, TrainDetector *TD2, void (*TrainDetectedCallback)()) {
	this->psu = PSU;
	this->tu = TU;
	this->td = TD;
	this->td2 = TD2;
	this->tdcb = TrainDetectedCallback;
	this->stat = Idle;
	this->wait_millis = 0;
}

void ReverseLoop::Attention(String *TrainPosition) {
	Serial.println("--------------- ReverseLoop ATTENTION at "+*TrainPosition);
	this->train_position = TrainPosition;
	this->psu->SetSpeed(0);
	this->psu->ReverseDirection(false);
	this->tu->Close();
	this->stat = Starting;
	this->wait_millis = 5000+millis();
}

void ReverseLoop::Loop() {
	if (this->stat == Starting) {
		if (millis() >= this->wait_millis) {
			Serial.println("--------------- ReverseLoop forward");
			this->wait_millis = 20000+millis();
			this->stat = LookForFirstTD;
			this->psu->SetSpeed(192);
		}
	} else if (this->stat == LookForFirstTD) {
		if (this->td->TrainSeen() || millis() >= this->wait_millis) {
			*(this->train_position) = this->td->BlockName();
			this->tu->Throw();
			this->psu->ReverseDirection(true);
			this->td->AcknowledgeTrainSeen();
			this->td2->AcknowledgeTrainSeen();
			this->stat = InLoop;
			this->wait_millis = 40000+millis();
		}
	} else if (this->stat == InLoop) {
		if (this->td2->TrainSeen() || millis() >= this->wait_millis) {
			this->wait_millis = 100+millis();
			this->psu->SetSpeed(128);
			this->stat = RunToPlatform;
		}
	} else if (this->stat == RunToPlatform) {
		if (millis() >= this->wait_millis) {
			this->wait_millis = 0;
			this->psu->SetSpeed(0);
			this->stat = Stopping;
		}
	} else if (this->stat == Stopping) {
		if (this->psu->GetActualSpeed() == 0) {
			this->td->AcknowledgeTrainSeen();
			this->td2->AcknowledgeTrainSeen();
			this->stat = Idle;
			this->tdcb();
		}
	}
}
