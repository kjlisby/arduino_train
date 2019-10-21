#include "AjaxHandler.h"
#include "Turnout.h"
#include "PowerSupply.h"
#include "TrainDetector.h"
extern PowerSupply   *PSU1;
extern Turnout       *TU;
extern TrainDetector *TD1;

#define ledPin LED_BUILTIN

class InternalAjaxHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/ajax_");
  }

  bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri) {   
    Serial.println("AJAX GETINPUT");
    Serial.println(requestUri);

    String cmdarray[10];
    int index = 0;
    int startindex = 0;
    int cmd_length = 0;
    while (index < requestUri.length()) {
      if (requestUri[index] == '_') {
        cmdarray[cmd_length] = requestUri.substring(startindex, index);
        startindex = index+1;
        cmd_length++;
      }
      index++;
    }
    cmdarray[cmd_length] = requestUri.substring(startindex);
    index++;
    cmd_length++;
    
    for (int i=0; i<cmd_length; i++) {
      Serial.print(i); Serial.print(": ");
      Serial.println(cmdarray[i]);
    }

    if (cmdarray[1].equals("set")) {
      if (cmdarray[2].equals("B1")) {
        TU->Throw();
      }
      if (cmdarray[2].equals("B2")) {
        TU->Close();
      }
    }
    server.send(200, "text/plain", "");
  }
} internalAjaxHandler;

boolean LEDstate = false;
void AjaxHandler::Loop() {
    bool old_reedState = reedState;
    reedState = TD1->TrainSeen();
    if (old_reedState != reedState) {
      if (reedState) {
        analogWrite(ledPin,255);
        PSU1->ResetStatus();
        PSU1->SetSpeed(-255);
      } else {
        analogWrite(ledPin,25);
        PSU1->SetSpeed(0);
      }
    }
    counter++;
    if (counter > 500000) {
      counter = 0;
      Serial.print("TOGGLE LED ");
//      digitalWrite (ledPin,!digitalRead(ledPin));
      if (LEDstate) {
        Serial.println("OFF");
        LEDstate = false;
        analogWrite(ledPin,0);
      } else {
        Serial.println("ON");
        LEDstate = true;
        analogWrite(ledPin,100);
      }
    }
}

bool AJAXINITIALIZED = false;
void AjaxHandler::Init(WebServer *server) {
	if (AJAXINITIALIZED) return;
	AJAXINITIALIZED = true;
	server->addHandler(&internalAjaxHandler);
	pinMode ( ledPin, OUTPUT );
	analogWrite ( ledPin, 0 );
}
