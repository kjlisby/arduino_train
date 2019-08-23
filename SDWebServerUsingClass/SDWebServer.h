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
#ifndef SD_WEB_SERVER_H
#define SD_WEB_SERVER_H

#include <ESP8266WebServer.h>

class SDWebServer {
	public:
		SDWebServer();
    void Init();
    ESP8266WebServer *getServer();
	private:
		const char* ssid = "ESP8266";
		const char* password = "ESP8266Test";
//		File uploadFile;
//		void returnOK();
//		void returnFail(String msg);
//		bool loadFromSdCard(String path);
//		void handleFileUpload();
//		void deleteRecursive(String path);
//		void handleDelete();
//		void handleCreate();
//		void printDirectory();
//		void handleNotFound();
};

#endif
