#include "ReverseLoop.h"

#define LOG this->ws->sendMessage

void ReverseLoop::Init(SDWebServer *WS, PowerSupply *PSU, Turnout *TU, TrainDetector *TD, TrainDetector *TD2, void (*TrainDetectedCallback)()) {
  this->ws  = WS;
  this->psu = PSU;
	this->tu  = TU;
	this->td  = TD;
	this->td2 = TD2;
	this->tdcb = TrainDetectedCallback;
	this->stat = Idle;
	this->wait_millis = 0;
}

void ReverseLoop::Attention(String *TrainPosition) {
	LOG("--------------- ReverseLoop ATTENTION at "+*TrainPosition+" State==Starting\n");
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
      LOG("--------------- ReverseLoop forward. State==LookForFirstTD\n");
			this->wait_millis = 12000+millis();
			this->stat = LookForFirstTD;
			this->psu->SetSpeed(192);
		}
	} else if (this->stat == LookForFirstTD) {
		if (this->td->TrainSeen() || millis() >= this->wait_millis) {
      if (millis() >= this->wait_millis) LOG("TIMEOUT ");
      LOG("--------------- ReverseLoop looking for TD2. State==InLoop\n");
			*(this->train_position) = this->td->BlockName();
			this->tu->Throw();
			this->psu->ReverseDirection(true);
			this->td->AcknowledgeTrainSeen();
			this->td2->AcknowledgeTrainSeen();
			this->stat = InLoop;
			this->wait_millis = 18000+millis();
		}
	} else if (this->stat == InLoop) {
		if (this->td2->TrainSeen() || millis() >= this->wait_millis) {
      if (millis() >= this->wait_millis) LOG("TIMEOUT ");
      LOG("--------------- ReverseLoop out of loop. State==RunToPlatform\n");
			this->wait_millis = 100+millis();
			this->psu->SetSpeed(128);
			this->stat = RunToPlatform;
		}
	} else if (this->stat == RunToPlatform) {
		if (millis() >= this->wait_millis) {
      LOG("--------------- ReverseLoop stopping. State==Stopping\n");
			this->wait_millis = 0;
			this->psu->SetSpeed(0);
			this->stat = Stopping;
		}
	} else if (this->stat == Stopping) {
		if (this->psu->GetActualSpeed() == 0) {
      LOG("--------------- ReverseLoop stopped. State==Idle\n");
			this->td->AcknowledgeTrainSeen();
			this->td2->AcknowledgeTrainSeen();
			this->stat = Idle;
			this->tdcb();
		}
	}
}
