#ifndef LOOP_H
#define LOOP_H

#include <ESP8266WebServer.h>

extern ESP8266WebServer server;

void loop_func(int &a, int &b, bool &isWork, int led, int air, int fan, int work, int rest, bool isAuto) {
  if (isAuto) {
    Serial.println("automode");
  } else {
    if (isWork) {
      a -= 1;
      digitalWrite(led, HIGH);
      digitalWrite(air, HIGH);
      digitalWrite(fan, HIGH);
      Serial.println(a);
      if (a <= 0) {
        isWork = false;
        a = rest;
      }
    } else {
      b -= 1;
      digitalWrite(led, LOW);
      digitalWrite(air, LOW);
      digitalWrite(fan, LOW);
      Serial.println(b);
      if (b <= 0) {
        isWork = true;
        b = work;
      }
    }
  }
  server.handleClient();
}

#endif

