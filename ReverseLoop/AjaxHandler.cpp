#include "AjaxHandler.h"
#include "Turnout.h"
#include "PowerSupply.h"
#include "TrainDetector.h"
extern PowerSupply   *PSU1;
extern PowerSupply   *PSU2;
extern Turnout       *TU1;
extern Turnout       *TU2;
extern Turnout       *TU3;
extern Turnout       *TU4;
extern Turnout       *TU5;
extern Turnout       *TU6;
extern TrainDetector *TD1;
extern TrainDetector *TD2;
extern TrainDetector *TD3;
extern TrainDetector *TD4;
extern bool          AutoMode;
extern void          ToggleAutoMode();
extern String        Train1Position;
extern String        Train2Position;

#define ledPin LED_BUILTIN

String JSON_ArrayStart() {
  return "{\"objects\":[";
}
String JSON_ArrayDivider() {
  return ",";
}
String JSON_ArrayEnd() {
  return "]}";
}
String JSON_item(String Name, String Value) {
  return "{\"item\":\""+Name+"\",\"value\":\""+Value+"\"}";
}

class InternalAjaxHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith("/ajax_");
  }

  String SetTurnout(String TurnoutName, String Cmd) {
    //Serial.println("xXxXxXxX--------- SetTurnout "+TurnoutName+" "+Cmd);
    Turnout *TU;
    switch (TurnoutName[1]) {
      case '1': TU = TU1; break;
      case '2': TU = TU2; break;
      case '3': TU = TU3; break;
      case '4': TU = TU4; break;
      case '5': TU = TU5; break;
      case '6': TU = TU6; break;
      default: return "UNKNOWN TURNOUT!!!";
    }
    if (Cmd == "S") {
      TU->Throw();
      Serial.println("xXxXxXxX--------- SetTurnout "+TurnoutName+" THROW");
      return JSON_ArrayStart()+JSON_item(TurnoutName, "thrown")+JSON_ArrayEnd();
    } else {
      TU->Close();
      Serial.println("xXxXxXxX--------- SetTurnout "+TurnoutName+" CLOSE");
      return JSON_ArrayStart()+JSON_item(TurnoutName, "closed")+JSON_ArrayEnd();
    }
  }

  String GetStatus() {
    Serial.println("GET STATUS  Train1Position="+Train1Position);
    String RetVal = JSON_ArrayStart();
    RetVal += JSON_item("AutoMode",String(AutoMode));
    RetVal += JSON_ArrayDivider()+JSON_item("Power1", String(PSU1->GetSpeed()));
    RetVal += JSON_ArrayDivider()+JSON_item("Power2", String(PSU2->GetSpeed()));
    if (!PSU1->GetStatus()) {
      RetVal += JSON_ArrayDivider()+JSON_item("PSU1FB", "DISABLED");
    } else {
      RetVal += JSON_ArrayDivider()+JSON_item("PSU1FB", String(analogRead(32)));
    }
    if (!PSU2->GetStatus()) {
      RetVal += JSON_ArrayDivider()+JSON_item("PSU2FB", "DISABLED");
    } else {
      RetVal += JSON_ArrayDivider()+JSON_item("PSU2FB", String(analogRead(33)));
    }
    if (TD1->TrainSeen()) {
      RetVal += JSON_ArrayDivider()+JSON_item("TD1", "TD-1");
    }
    if (TD2->TrainSeen()) {
      RetVal += JSON_ArrayDivider()+JSON_item("TD2", "TD-2");
    }
    RetVal += JSON_ArrayDivider()+JSON_item("T1", TU1->GetPosition());
    RetVal += JSON_ArrayDivider()+JSON_item("T2", TU2->GetPosition());
    RetVal += JSON_ArrayDivider()+JSON_item("T3", TU3->GetPosition());
    RetVal += JSON_ArrayDivider()+JSON_item("T4", TU4->GetPosition());
    RetVal += JSON_ArrayDivider()+JSON_item("T5", TU5->GetPosition());
    RetVal += JSON_ArrayDivider()+JSON_item("T6", TU6->GetPosition());
    RetVal += JSON_ArrayDivider()+JSON_item("train1Pos", Train1Position);
    RetVal += JSON_ArrayDivider()+JSON_item("train2Pos", Train2Position);
    Serial.println("GET STATUS - end of GetStatus()");
    return RetVal+JSON_ArrayEnd();
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

    if (cmdarray[1].startsWith("getstatus")) {
      server.send(200, "text/plain", GetStatus());
    } else if (cmdarray[1].equals("set")) {
      if (cmdarray[2].equals("B1")) {
        server.send(200, "text/plain", "B1 activated");
      } else if (cmdarray[2].equals("B2")) {
        server.send(200, "text/plain", "B2 activated");
      } else if (cmdarray[2].equals("B4")) {
        server.send(200, "text/plain", "B4 activated");
      } else if (cmdarray[2].equals("B5")) {
        server.send(200, "text/plain", "B5 activated");
      } else if (cmdarray[2].equals("B6")) {
        server.send(200, "text/plain", "B6 activated");
      } else if (cmdarray[2].equals("B7")) {
        server.send(200, "text/plain", "B7 activated");
      } else if (cmdarray[2].equals("automode")) {
        ToggleAutoMode();
        Serial.println("AutoMode toggled to "+String(AutoMode));
        server.send(200, "text/plain", JSON_ArrayStart()+JSON_item("automode", String(AutoMode))+JSON_ArrayEnd());
      } else if (cmdarray[2].equals("stop1")) {
        PSU1->STOP();
        server.send(200, "text/plain", JSON_ArrayStart()+JSON_item("Power1", "0")+JSON_ArrayEnd());
      } else if (cmdarray[2].equals("stop2")) {
        PSU2->STOP();
        server.send(200, "text/plain", JSON_ArrayStart()+JSON_item("Power2", "0")+JSON_ArrayEnd());
      } else if (cmdarray[2].equals("reset1")) {
        PSU1->ResetStatus();
        server.send(200, "text/plain", JSON_ArrayStart()+JSON_item("Power1", String(PSU1->GetSpeed()))+JSON_ArrayEnd());
      } else if (cmdarray[2].equals("reset2")) {
        PSU2->ResetStatus();
        server.send(200, "text/plain", JSON_ArrayStart()+JSON_item("Power2", String(PSU2->GetSpeed()))+JSON_ArrayEnd());
      } else if (cmdarray[2].equals("psu1")) {
        if (!AutoMode) {
          PSU1->SetSpeed(cmdarray[3].toInt());
          server.send(200, "text/plain", JSON_ArrayStart()+JSON_item("Power1", String(PSU1->GetSpeed()))+JSON_ArrayEnd());
        } else {
          server.send(409, "text/plain", "Speed change not allowed in auto mode");
        }
      } else if (cmdarray[2].equals("psu2")) {
        if (!AutoMode) {
          PSU2->SetSpeed(cmdarray[3].toInt());
          server.send(200, "text/plain", JSON_ArrayStart()+JSON_item("Power2", String(PSU2->GetSpeed()))+JSON_ArrayEnd());
        } else {
          server.send(409, "text/plain", "Speed change not allowed in auto mode");
        }
      } else if (cmdarray[2].length() == 2 &&
                 cmdarray[2][0] == 'T' &&
                 cmdarray[2][1] >= '1' &&
                 cmdarray[2][1] <= '6' &&
                (cmdarray[3].equals("S") || cmdarray[3].equals("T"))) {
        server.send(200, "text/plain", SetTurnout(cmdarray[2], cmdarray[3]));
      } else {
        server.send(404, "text/plain", "Unknown HTLM element for set command");
      }
    } else {
      server.send(404, "text/plain", "Unknown AJAX command");
    }
  }
} internalAjaxHandler;

boolean LEDstate = false;
void AjaxHandler::Loop() {
//    bool old_reedState = reedState;
//    reedState = TD1->TrainSeen();
//    if (old_reedState != reedState) {
//      if (reedState) {
//        analogWrite(ledPin,255);
//        PSU1->ResetStatus();
//        PSU1->SetSpeed(-255);
//      } else {
//        analogWrite(ledPin,25);
//        PSU1->SetSpeed(0);
//      }
//    }
    counter++;
    if (counter > 500000) {
      counter = 0;
      Serial.print("TOGGLE LED ");
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
