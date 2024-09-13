#ifndef AUTO2_H
#define AUTO2_H

#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Ticker.h>

extern const int led, air, fan;
extern ESP8266WebServer server;
extern Ticker timer;
extern int autoModeState;
extern bool isAutoMode, isOn, autoMode1, autoMode2, autoMode3;

void loadAutoModeState() {
  EEPROM.begin(512);
  autoModeState = EEPROM.read(110);
  EEPROM.end();
}

void disableAllAutoModes() {
  if (autoMode1 || autoMode2 || autoMode3) {
    timer.detach();  // Отключаем таймер, если любой из режимов был включен
    autoMode1 = false;
    autoMode2 = false;
    autoMode3 = false;
  }
}

void toggleAllDevices(bool state) {
  digitalWrite(led, state ? HIGH : LOW);
  digitalWrite(air, state ? HIGH : LOW);
  digitalWrite(fan, state ? HIGH : LOW);
}

void toggleAutoMode1() {
  if (autoMode1) {
    disableAllAutoModes();
    toggleAllDevices(false);
  } else {
    disableAllAutoModes();
    toggleAllDevices(false);

    timer.attach(12 * 3600, []() {
      isOn = !isOn;
      toggleAllDevices(isOn);
    });
    autoMode1 = true;
  }
  handleAuto();
}

void toggleAutoMode2() {
  if (autoMode2) {
    disableAllAutoModes();
    toggleAllDevices(false);
  } else {
    disableAllAutoModes();
    toggleAllDevices(false);

    timer.attach(1, []() {
      digitalWrite(led, !digitalRead(led));
    });
    autoMode2 = true;
  }
  handleAuto();
}

void toggleAutoMode3() {
  if (autoMode3) {
    disableAllAutoModes();
    toggleAllDevices(false);
  } else {
    disableAllAutoModes();
    toggleAllDevices(false);
    autoMode3 = true;
  }
  handleRoot();
}

void handleMode() {
  String automode = server.arg("mode");
  isAutoMode = true;
  autoModeState = 1;
  saveAutoModeState(autoModeState);
  if (automode == "1") {
    toggleAutoMode1();
  } else if (automode == "2") {
    toggleAutoMode2();
  } else if (automode == "3") {
    toggleAutoMode3();
  }
}

#endif
