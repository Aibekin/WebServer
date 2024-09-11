#ifndef AUTO2_H
#define AUTO2_H

#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Ticker.h>

extern const int led, air, fan;
extern ESP8266WebServer server;
extern Ticker timer;
extern int autoModeState;
extern bool isAutoMode, isOn;

void loadAutoModeState() {
  EEPROM.begin(512);
  autoModeState = EEPROM.read(110);
  EEPROM.end();
}

void toggleAllDevices(bool state) {
  digitalWrite(led, state ? HIGH : LOW);
  digitalWrite(air, state ? HIGH : LOW);
  digitalWrite(fan, state ? HIGH : LOW);
}

void toggleAutoMode1() {
  toggleAllDevices(false); // Все устройства в начальной позиции выключены

  timer.attach(7, []() {  // Включает на 12 часов, затем выключает
    isOn = !isOn;
    toggleAllDevices(isOn);
  });
  handleAuto();
}

void handleMode() {
  String automode = server.arg("mode");
  isAutoMode = true;
  autoModeState = 1;
  saveAutoModeState(autoModeState);
  if (automode = "1") {
    toggleAutoMode1();
  }
}

#endif
