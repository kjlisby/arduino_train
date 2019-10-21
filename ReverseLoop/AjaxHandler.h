#ifndef AJAXHANDLER_H
#define AJAXHANDLER_H

#include <stdint.h>
#include <WebServer.h>

class AjaxHandler {
	public:
		void Init(WebServer *server);
		void Loop();
	private:
		int  counter   = 0;
		bool ledState  = false;
		int  reedState = false;
};

#endif
