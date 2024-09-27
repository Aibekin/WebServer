#ifndef LOOP_H
#define LOOP_H

#include <ESP8266WebServer.h>
#include <Ticker.h>

extern const int led, air, fan;
extern Ticker timer;
extern ESP8266WebServer server;
extern bool isWork;
extern int a, b, work, rest;

void loop_func()
{
	timer.attach(1, []()
				 {
		if (isWork) {
			b -= 1;
			digitalWrite(led, HIGH);
			digitalWrite(air, HIGH);
			digitalWrite(fan, HIGH);
			Serial.println(b);
			if (b <= 0) {
				isWork = false;
				b = rest;
			}
		} else {
			a -= 1;
			digitalWrite(led, LOW);
			digitalWrite(air, LOW);
			digitalWrite(fan, LOW);
			Serial.println(a);
			if (a <= 0) {
				isWork = true;
				a = work;
			}
		} });
}

#endif