/*
  SDWebServer class

  Heavily inspired by the Example WebServer with SD Card backend for esp8266
  Which is Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Have a FAT Formatted SD Card connected to the SPI port of the ESP8266
  The web root is the SD Card root folder
  File extensions with more than 3 charecters are not supported by the SD Library
  File Names longer than 8 charecters will be truncated by the SD library, so keep filenames shorter
  index.htm is the default index (works on subfolders as well)

  upload the contents of SdRoot to the root of the SDcard and access the editor by going to http://esp8266sd.local/edit

*/
#include "SDWebServer.h"

void SDWebServer::returnOK () {
	this->server->send(200, "text/plain", "");
}

void SDWebServer::returnFail (String msg) {
	this->server->send(500, "text/plain", msg + "\r\n");
}

bool SDWebServer::loadFromSdCard (String path) {
	String dataType = "text/plain";
	if (path.endsWith("/")) {
		path += "index.htm";
	}
	if (path.endsWith(".src")) {
		path = path.substring(0, path.lastIndexOf("."));
	} else if (path.endsWith(".htm")) {
		dataType = "text/html";
	} else if (path.endsWith(".css")) {
		dataType = "text/css";
	} else if (path.endsWith(".js")) {
		dataType = "application/javascript";
	} else if (path.endsWith(".png")) {
		dataType = "image/png";
	} else if (path.endsWith(".gif")) {
		dataType = "image/gif";
	} else if (path.endsWith(".jpg")) {
		dataType = "image/jpeg";
	} else if (path.endsWith(".ico")) {
		dataType = "image/x-icon";
	} else if (path.endsWith(".xml")) {
		dataType = "text/xml";
	} else if (path.endsWith(".pdf")) {
		dataType = "application/pdf";
	} else if (path.endsWith(".zip")) {
		dataType = "application/zip";
	}
	File dataFile = SD.open(path.c_str());
	if (dataFile.isDirectory()) {
		path += "/index.htm";
		dataType = "text/html";
		dataFile = SD.open(path.c_str());
	}
	if (!dataFile) {
		return false;
	}
	if (server->hasArg("download")) {
		dataType = "application/octet-stream";
	}
	if (this->server->streamFile(dataFile, dataType) != dataFile.size()) {
		Serial.println("Sent less data than expected!");
	}
	dataFile.close();
	return true;
}

SDWebServer::SDWebServer() {
	this->server   = new WebServer(80);
	this->apIP     = new IPAddress(42, 42, 42, 42);
	this->staticIP = new IPAddress(192,168,1,200);
	this->gateway  = new IPAddress(192,168,1,1);
	this->subnet   = new IPAddress(255,255,255,0);
	WiFi.persistent(false);
	WiFi.mode(WIFI_AP_STA);
	WiFi.config(*(this->staticIP), *(this->gateway), *(this->subnet));
	WiFi.softAPConfig(*(this->apIP), *(this->apIP), *(this->subnet));
	WiFi.softAP(this->ssid, this->password); 
	IPAddress myAPIP = WiFi.softAPIP(); 
	Serial.print("AP IP address: "); 
	Serial.println(myAPIP); 
	IPAddress mySTAIP = WiFi.localIP(); 
	Serial.print("STA IP address: "); 
	Serial.println(mySTAIP);
}

bool INITIALIZED = false;
void SDWebServer::Init(uint8_t SDPin) {
	if (INITIALIZED) return;
	INITIALIZED = true;
	
	this->sd_pin = SDPin;
	server->begin();
	Serial.println("HTTP server started");
	if (SD.begin(this->sd_pin)) {
		Serial.println("SD Card initialized.");
	}
}

WebServer *SDWebServer::getServer() {
	return server;
}
