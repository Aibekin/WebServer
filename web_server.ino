#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Ticker.h>

#include "loop.h"
#include "root.h"
#include "data.h"
#include "ip.h"
#include "manual.h"
#include "settings.h"
#include "autoMode1.h"
#include "autoMode2.h"

const int led = 5;
const int air = 16;
const int fan = 4;

const char *defaultSSID = "Aibek";
const char *defaultPassword = "11111111";

char ssid[32];
char password[32];

int onHour, onMinute, onSeconds, offHour, offMinute, offSeconds;
int workHour, workMinute, workSeconds, restHour, restMinute, restSeconds;

int a, b, work, rest;
bool isWork, isManual;
bool lightState = false;
bool aeratorState = false;
bool fanState = false;

bool isAutoMode = true;
int autoModeState = 1;
bool isOn = false;

ESP8266WebServer server(80);
Ticker timer;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(air, OUTPUT);
  pinMode(fan, OUTPUT);
  digitalWrite(led, LOW);
  digitalWrite(air, LOW);
  digitalWrite(fan, LOW);
  Serial.begin(115200);
  dht.begin();

  loadSettings();
  loadAutoModeState();
  isManual = true;

  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 10000; // 10 секунд

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("STA IP-адрес: ");
  Serial.println(WiFi.localIP());

  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(defaultSSID, defaultPassword);
  Serial.print("AP IP-адрес: ");
  Serial.println(WiFi.softAPIP());

  if (autoModeState == 1) {
    isAutoMode = true;
  } else {
    isAutoMode = false;
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.on("/getData", HTTP_GET, handleGetData);
  server.on("/auto_mode", HTTP_GET, handleAuto);
  server.on("/settings", HTTP_GET, handleSettings);
  server.on("/saveSettings", HTTP_POST, handleSaveSettings);
  server.on("/LED_ON", HTTP_GET, ledOn);
  server.on("/LED_OFF", HTTP_GET, ledOff);
  server.on("/AIR_ON", HTTP_GET, airOn);
  server.on("/AIR_OFF", HTTP_GET, airOff);
  server.on("/FAN_ON", HTTP_GET, fanOn);
  server.on("/FAN_OFF", HTTP_GET, fanOff);
  server.on("/timer", HTTP_POST, handleTimer);
  server.on("/manual", HTTP_GET, handleManual);
  server.on("/new_ip", HTTP_GET, handleNewIP);
  server.on("/getIP", HTTP_GET, getNewIP);
  server.on("/getInfo", HTTP_GET, getInfo);
  server.on("/autoMode", HTTP_GET, handleMode);
  
  getTimer();
  server.begin();
}

void loop() {
  if (!isManual) {
    loop_func(b, a, isWork, led, air, fan, work, rest, isAutoMode);
    delay(1000);
  } else {
    server.handleClient(); 
  }
}
