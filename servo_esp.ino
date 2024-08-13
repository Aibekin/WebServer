#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int pos1 = 90;
int pos2 = 90;
int pos3 = 90;
int pos4 = 90;

ESP8266WebServer server(80);

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  Serial.println("Connected to WiFi");

  // Инициализация сервоприводов
  servo1.attach(D1);
  servo2.attach(D2);
  servo3.attach(D3);
  servo4.attach(D4);

  // Настройка маршрутов для веб-сервера
  server.on("/", handleRoot);
  server.on("/setServo", handleSetServo);

  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 30px 100px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 40px; } h1 { width: 230px; height: 70px; display: flex; justify-content: center; align-items: center; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex; justify-content: center; align-items: center; gap: 15px; background-color: #fff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px; font-weight: 600; text-align: center; } .hours, .mins, .secs { font-size: 30px; } .alarm_time { width: 230px; height: 30px; display: flex; justify-content: center; align-items: center; background-color: #fff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px; font-weight: 600; text-align: center; } .wrapper { display: flex; flex-direction: column; height: 50px; width: 230px; align-items: center; justify-content: space-between; } .servo { -webkit-appearance: none; background: -webkit-linear-gradient(left, #77c9ff 0%, #77c9ff 50%, #ccc 50%, #ccc 100%); width: 100%; height: 20px; border-radius: 10px; } .servo::-webkit-slider-thumb { -webkit-appearance: none; width: 30px; height: 30px; background-color: #77c9ff; border-radius: 50%; cursor: pointer; transition: 0.3s all; } .servo::-webkit-slider-thumb:hover { transform: scale(1.2); background-color: #5b9cc7; } .value { color: #2c3e50; font-size: 20px; } .back { width: 230px; height: 40px; font-size: 20px; font-weight: 600; text-decoration: none; color: #2c3e50; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex; justify-content: center; align-items: center; transition: 0.3s all; } .back:hover { background-color: #fff; } </style></head><body><h1>Servo control</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='alarm_time'>00:00 - 00:00</div><div class='wrapper'><input type='range' min='0' max='180' value='90' name='servo1' class='servo' id='servo1'><label for='servo1' class='value'>90&deg;</label></div><div class='wrapper'><input type='range' min='0' max='180' value='90' name='servo1' class='servo' id='servo2'><label for='servo1' class='value'>90&deg;</label></div><div class='wrapper'><input type='range' min='0' max='180' value='90' name='servo1' class='servo' id='servo3'><label for='servo1' class='value'>90&deg;</label></div><div class='wrapper'><input type='range' min='0' max='180' value='90' name='servo1' class='servo' id='servo4'><label for='servo1' class='value'>90&deg;</label></div><a href='/' class='back'>Go Back</a><script> function toggleTime() { const time = new Date(); document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0'); document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0'); } function updateServo(angle, i) { var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setServo?servo' + String(i) + '=' + angle, true); xhttp.send(); } const servos = document.querySelectorAll('.servo'); const values = document.querySelectorAll('.value'); servos.forEach((a, i) => { a.addEventListener('input', (event) => { const target = event.target; let value = target.value; updateServo(value, i + 1); values[i].innerHTML = `${value}&deg;`; value = (value * 100) / 180; target.style.background = '-webkit-linear-gradient(left, #77c9ff 0%, #77c9ff ' + value + '%, #ccc ' + value + '%, #ccc 100%)'; }); }); function interval() { toggleTime(); } window.onload = interval(); setInterval(interval, 1000); </script></body></html>";
  
  server.send(200, "text/html", html);
}

void handleSetServo() {
  if (server.hasArg("servo1")) {
    pos1 = server.arg("servo1").toInt();
    servo1.write(pos1);
  }
  if (server.hasArg("servo2")) {
    pos2 = server.arg("servo2").toInt();
    servo2.write(pos2);
  }
  if (server.hasArg("servo3")) {
    pos3 = server.arg("servo3").toInt();
    servo3.write(pos3);
  }
  if (server.hasArg("servo4")) {
    pos4 = server.arg("servo4").toInt();
    servo4.write(pos4);
  }

  server.send(200, "text/plain", "OK");
}

void loop() {
  server.handleClient();
}
