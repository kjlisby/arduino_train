void ReverseLoop::Init(PowerSupply *PSU, Turnout *TU, TrainDetector *TD, void (*TrainDetectedCallback)()) {
  this->psu = PSU;
  this->tu = TU;
  this->td = TD;
  this->tdcb = TrainDetectedCallback;
  this->active = false;
  this->wait_millis = 0;
}

void ReverseLoop::Attention() {
  this->psu->SetSpeed(0);
  this->psu->ReverseDirection(false);
  this->tu->Close();
  this->active = true;
  this->wait_millis = 5000+millis();
}

void ReverseLoop::Loop() {
  if (this->active && this->wait_millis > 0) {
    if (millis() >= this->wait_millis) {
      this->wait_millis = 0;
      this->psu->SetSpeed(128);
    }
  } else if (!this->active && this->wait_millis > 0) {
    if (millis() >= this->wait_millis) {
      this->wait_millis = 0;
      this->psu->SetSpeed(0);
    }
  } else if (this->active) {
    if (this->td->TrainSeen()) {
      this->tu->Throw();
      this->psu->ReverseDirection(true);
      this->tdcb();
      this->td->AcknowledgeTrainSeen();
      this->active = false;
      this->wait_millis = 5000+millis();
    }
  }
}
