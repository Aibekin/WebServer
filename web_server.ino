#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <EEPROM.h>

#include "loop.h"

#define DHTPIN 0
#define DHTTYPE DHT11 // Тип датчика

DHT dht(DHTPIN, DHTTYPE);

const int led = 2;

const char *defaultSSID = "Aibek";
const char *defaultPassword = "11111111";

char ssid[32];
char password[32];

int onHour, onMinute, onSeconds, offHour, offMinute, offSeconds;
int workHour, workMinute, workSeconds, restHour, restMinute, restSeconds;

int a, b, work, rest;
bool isWork;

ESP8266WebServer server(80);

void handleRoot() {
  String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style>* { box-sizing: border-box; padding: 0; margin: 0;  } body {  padding: 30px 100px;  display: flex;  flex-direction: column; align-items: center;  justify-content: flex-start;  gap: 40px;  } h1 {  width: 230px; height: 70px; display: flex;  justify-content: center;  align-items: center;  background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex;  justify-content: center;  align-items: center;  gap: 15px;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .hours, .mins,  .secs { font-size: 30px;  } .links {  display: flex;  flex-direction: column; align-items: center;  justify-content: center;  gap: 10px;  padding: 15px;  width: 230px; background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .nav {  text-decoration: none;  color: #2c3e50; font-size: 20px;  width: 100%;  height: 30px; border-radius: 15px;  display: flex;  justify-content: center;  align-items: center;  transition: 0.3s all; } .nav:hover {  background-color: #fff; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .alarm {  width: 230px; height: 30px; display: flex;  justify-content: center;  align-items: center;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; }</style></head><body><h1>Main page</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='alarm'>00:00 - 00:00</div><div class='links'><a href='/data' class='nav'>Data</a><a href='/manip' class='nav'>Manip</a><a href='/light'  class='nav'>Light</a><a href='/fan' class='nav'>Fan</a><a href='/aeration' class='nav'>Aeration</a><a href='/settings' class='nav'>Settings</a><a href='/new_ip' class='nav'>New IP</a></div><script>function toggleTime() {  const time = new Date();  document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0');  document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0');  } function getTime() {  fetch('/getAlarm').then(response => response.json()).then(data => { let start = data.start.split(':').map(num => num.padStart(2, '0')).join(':'); let end = data.end.split(':').map(num => num.padStart(2, '0')).join(':'); const time = new Date();  const currentTime = time.getHours().toString().padStart(2, '0') + ':' + time.getMinutes().toString().padStart(2, '0') + ':' + time.getSeconds().toString().padStart(2, '0');  document.querySelector('.alarm').innerText = start.slice(0, 5) + ' - ' + end.slice(0, 5); if (currentTime >= start && currentTime < end) {  fetch('/LED_ON').then(response => response.json()).then(data => { console.log(currentTime, start);  }); } if (currentTime == end) { fetch('/LED_OFF').then(response => response.json()).then(data => {  console.log(currentTime, end);  }); } }); } function interval() { toggleTime(); getTime();  } window.onload = interval(); setInterval(interval, 1000);</script></body></html>";
  server.send(200, "text/html", output);
}

void handleData() {
  String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style>* { box-sizing: border-box; padding: 0; margin: 0;  } body {  padding: 30px 100px;  display: flex;  flex-direction: column; align-items: center;  justify-content: flex-start;  gap: 40px;  } h1 {  width: 230px; height: 70px; display: flex;  justify-content: center;  align-items: center;  background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex;  justify-content: center;  align-items: center;  gap: 15px;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .hours, .mins,  .secs { font-size: 30px;  } .alarm {  width: 230px; height: 30px; display: flex;  justify-content: center;  align-items: center;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .temp_data {  display: flex;  flex-direction: column; align-items: center;  justify-content: space-between; padding: 15px;  width: 230px; height: 150px;  background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .info { display: flex;  align-items: center;  justify-content: flex-start;  gap: 50px;  } .title {  color: #2c3e50; font-size: 35px;  width: 35px;  height: 35px; text-align: center; } .title img {  width: 100%;  height: 100%; } .info span {  color: #2c3e50; font-size: 35px;  } .back { width: 230px; height: 40px; font-size: 20px;  font-weight: 600; text-decoration: none;  color: #2c3e50; background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex;  justify-content: center;  align-items: center;  transition: 0.3s all; } .back:hover { background-color: #fff; }</style></head><body><h1>Data</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='alarm'>00:00 - 00:00</div><div class='temp_data'><div class='info'><div class='title'>t&deg;</div><span class='value' id='temp'>--&deg;C</span></div><div class='info'><div class='title'><img  src='https://www.freepnglogos.com/uploads/drop-png/waterdrop-sharp-glimpse-illustration-transparent-png-8.png'></div><span class='value' id='hum'>--%</span></div></div><a href='/' class='back'>Go Back</a><script>function toggleTime() { const time = new Date();  document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0');  document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0');  } function getTime() {  fetch('/getAlarm').then(response => response.json()).then(data => { let start = data.start.split(':').map(num => num.padStart(2, '0')).join(':'); let end = data.end.split(':').map(num => num.padStart(2, '0')).join(':'); const time = new Date();  const currentTime = time.getHours().toString().padStart(2, '0') + ':' + time.getMinutes().toString().padStart(2, '0') + ':' + time.getSeconds().toString().padStart(2, '0');  document.querySelector('.alarm').innerText = start.slice(0, 5) + ' - ' + end.slice(0, 5); if (currentTime >= start && currentTime < end) {  fetch('/LED_ON').then(response => response.json()).then(data => { console.log(currentTime, start);  }); } if (currentTime == end) { fetch('/LED_OFF').then(response => response.json()).then(data => {  console.log(currentTime, end);  }); } }); } function updateData() { fetch('/getData').then(response => response.json()).then(data => {  document.getElementById('temp').innerText = data.temperature + '°C';  document.getElementById('hum').innerText = data.humidity + '%'; }); } function interval() { toggleTime(); updateData(); getTime();  } window.onload = interval(); setInterval(interval, 1000);</script></body></html>";
  
  server.send(200, "text/html", output);
}

void handleGetData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String json = "{\"temperature\":" + String(t) + ",\"humidity\":" + String(h) + "}";
  server.send(200, "application/json", json);
}

void handleLight() {
  String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 30px 100px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 30px; } h1 { width: 230px; height: 70px; display: flex; justify-content: center; align-items: center; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex; justify-content: center; align-items: center; gap: 15px; background-color: #fff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px; font-weight: 600; text-align: center; } .hours, .mins, .secs { font-size: 30px; } .buttons { width: 230px; display: flex; justify-content: space-between; align-items: center; } .button { width: 65px; height: 30px; background-color: #77c9ff; border: none; border-radius: 5px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 14px; cursor: pointer; transition: 0.3s all; } .active { transform: translateY(4px); background-color: #5b9cc7; } .tabs { width: 230px; } .tab { width: 230px; display: none; padding: 15px; justify-content: center; gap: 30px; align-items: center; flex-wrap: wrap; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .tab-active { display: flex; } .switcher { background-color: #fff; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); border: none; border-radius: 30px; width: 40px; height: 20px; cursor: pointer; padding: 5px; transition: 0.3s all; } .switcher:before { content: ''; display: block; background-color: #000; width: 10px; height: 10px; border-radius: 50%; margin-left: 0; transition: 0.3s ease-in; } .switcher-active { background-color: green; } .switcher-active:before { margin-left: 20px; background-color: #fff; } .mode { width: 230px; display: flex; justify-content: space-between; align-items: center; } .submit { background-color: red; border: none; border-radius: 5px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); width: 85px; height: 40px; color: #fff; font-size: 25px; cursor: pointer; width: 100%; } .timer { display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 30px; width: 100%; } .wrapper { width: 100%; display: flex; align-items: center; justify-content: space-between; } label { font-size: 20px; color: #2c3e50; font-weight: 600; } .timer_input { width: 110px; height: 30px; background-color: #fff; font-size: 15px; color: black; display: flex; align-items: center; justify-content: center; gap: 20px; border: none; border-radius: 5px; } .back { width: 230px; height: 40px; font-size: 20px; font-weight: 600; text-decoration: none; color: #2c3e50; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex; justify-content: center; align-items: center; transition: 0.3s all; } .back:hover { background-color: #fff; } .form { width: 200px; display: flex; align-items: center; justify-content: space-between; } .automode { width: 150px; height: 30px; border: none; border-radius: 5px; cursor: pointer; background-color: #fff; color: #000; transition: 0.3s all; } .automode.automode-active { background-color: green; color: #fff; } .auto { display: flex; flex-direction: column; justify-content: center; align-items: center; gap: 10px; } .more { width: 30px; height: 30px; border: none; border-radius: 5px; cursor: pointer; background-color: #fff; color: #000; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .more.more-active { background-color: #ccc; color: #000; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .descr { display: none; } .descr-active { display: block; width: 150px; text-align: center; } </style></head><body><h1>Light</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='mode'>Автоматический режим: <div class='switcher switcher-active'></div></div><div class='tabs'><div class='tab tab-active'><div class='auto'><div class='form'><input type='submit' value='AutoMode1' class='automode'><button class='more'>↑</button></div><div class='descr'>Мейир Сигма</div></div><div class='auto'><div class='form'><input type='submit' value='AutoMode2' class='automode'><button class='more'>↑</button></div><div class='descr'>Амиржан Сигма</div></div><div class='auto'><div class='form'><input type='submit' value='AutoMode3' class='automode'><button class='more'>↑</button></div><div class='descr'>Айбек Сигма</div></div></div><div class='tab'><form action='/timer' class='timer' method='post' onsubmit='getTimer()'><div class='wrapper'><label for='work'>Work</label><input type='time' name='work' id=work' class='timer_input'></div><div class='wrapper'><label for='rest'>Rest</label><input type='time' name='rest' id=rest' class='timer_input'></div><input type='submit' value='Submit' class='submit'></form></div></div><a href='/' class='back'>Go Back</a><script type='module'> function toggleTime() { const time = new Date(); document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0'); document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0'); } const buttons = document.querySelectorAll('.button'); const inputs = document.querySelectorAll('.automode'); const mores = document.querySelectorAll('.more'); const descrs = document.querySelectorAll('.descr'); const tabs = document.querySelectorAll('.tab'); let a, b, init_a, init_b, isWork = true; mores.forEach((a, i) => { a.addEventListener('click', (event) => { event.target.classList.toggle('more-active'); descrs[i].classList.toggle('descr-active'); event.target.classList.contains('more-active') ? event.target.innerText = '↓' : event.target.innerText = '↑'; }); }); const switcher = document.querySelector('.switcher'); switcher.addEventListener('click', () => { switcher.classList.toggle('switcher-active'); if (switcher.classList.contains('switcher-active')) { tabs[0].classList.add('tab-active'); tabs[1].classList.remove('tab-active'); } else { tabs[1].classList.add('tab-active'); tabs[0].classList.remove('tab-active'); } }); inputs.forEach((a, i) => { a.addEventListener('click', (event) => { inputs.forEach((a) => a.classList.remove('automode-active')); event.target.classList.add('automode-active'); }); }); function getTimer() { fetch('/getTimer') .then(response => response.json()) .then(data => { const work = data.work.split(':').map(num => num.padStart(2, '0')); const rest = data.rest.split(':').map(num => num.padStart(2, '0')); init_a = Number(work[0]) * 60 + Number(work[1]); init_b = Number(rest[0]) * 60 + Number(rest[1]); a = init_a; b = init_b; }); } function interval() { toggleTime(); } window.onload = () => { setInterval(interval, 1000); }; </script></body></html>";
  getTimer();
  server.send(200, "text/html", output);
}

void ledOn() {
  digitalWrite(led, HIGH);

  String output = "{\"state\":" + String(digitalRead(0)) + "}";
  server.send(200, "application/json", output);
}

void ledOff() {
  digitalWrite(led, LOW);

  String output = "{\"state\":" + String(digitalRead(0)) + "}";
  server.send(200, "application/json", output);
}

void handleServo() {
  String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style>* { box-sizing: border-box; padding: 0; margin: 0;  } body {  padding: 30px 100px;  display: flex;  flex-direction: column; align-items: center;  justify-content: flex-start;  gap: 40px;  } h1 {  width: 230px; height: 70px; display: flex;  justify-content: center;  align-items: center;  background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex;  justify-content: center;  align-items: center;  gap: 15px;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .hours, .mins,  .secs { font-size: 30px;  } .alarm_time { width: 230px; height: 30px; display: flex;  justify-content: center;  align-items: center;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .wrapper {  display: flex;  flex-direction: column; height: 50px; width: 230px; align-items: center;  justify-content: space-between; } .servo {  -webkit-appearance: none; background: -webkit-linear-gradient(left, #77c9ff 0%, #77c9ff 50%, #ccc 50%, #ccc 100%);  width: 100%;  height: 20px; border-radius: 10px;  } .servo::-webkit-slider-thumb {  -webkit-appearance: none; width: 30px;  height: 30px; background-color: #77c9ff;  border-radius: 50%; cursor: pointer;  transition: 0.3s all; } .servo::-webkit-slider-thumb:hover {  transform: scale(1.2);  background-color: #5b9cc7;  } .value {  color: #2c3e50; font-size: 20px;  } .back { width: 230px; height: 40px; font-size: 20px;  font-weight: 600; text-decoration: none;  color: #2c3e50; background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex;  justify-content: center;  align-items: center;  transition: 0.3s all; } .back:hover { background-color: #fff; }</style></head><body><h1>Servo control</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='alarm_time'>00:00 - 00:00</div><div class='wrapper'><input type='range' min='0' max='180' value='90' name='servo1' class='servo'><label for='servo1' class='value'>90&deg;</label></div><div class='wrapper'><input type='range' min='0' max='180' value='90' name='servo1' class='servo'><label for='servo1' class='value'>90&deg;</label></div><div class='wrapper'><input type='range' min='0' max='180' value='90' name='servo1' class='servo'><label for='servo1' class='value'>90&deg;</label></div><div class='wrapper'><input type='range' min='0' max='180' value='90' name='servo1' class='servo'><label for='servo1' class='value'>90&deg;</label></div><a href='/' class='back'>Go Back</a><script>function toggleTime() { const time = new Date();  document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0');  document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0');  } function updateServo(angle, i) {  var xhttp = new XMLHttpRequest(); xhttp.open('GET', '/setServo' + String(i) + '?angle=' + angle, true); xhttp.send(); } document.addEventListener('DOMContentLoaded', function () { const servos = document.querySelectorAll('.servo'); const values = document.querySelectorAll('.value'); servos.forEach((a, i) => {  a.addEventListener('input', (event) => {  const target = event.target;  let value = target.value; updateServo(value, i);  values[i].innerHTML = `${value}&deg;`;  value = (value * 100) / 180;  target.style.background = '-webkit-linear-gradient(left, #77c9ff 0%, #77c9ff ' + value + '%, #ccc ' + value + '%, #ccc 100%)';  }); }); }); function getTime() {  fetch('/getAlarm')  .then(response => response.json())  .then(data => { let start = data.start.split(':').map(num => num.padStart(2, '0')).join(':'); let end = data.end.split(':').map(num => num.padStart(2, '0')).join(':'); const time = new Date();  const currentTime = time.getHours().toString().padStart(2, '0') + ':' + time.getMinutes().toString().padStart(2, '0') + ':' + time.getSeconds().toString().padStart(2, '0');  document.querySelector('.alarm_time').innerText = start.slice(0, 5) + ' - ' + end.slice(0, 5);  if (currentTime >= start && currentTime < end) {  fetch('/LED_ON').then(response => response.json()).then(data => { console.log(currentTime, start);  }); } if (currentTime == end) { fetch('/LED_OFF').then(response => response.json()).then(data => {  console.log(currentTime, end);  }); } }); } function interval() { toggleTime(); getTime();  } window.onload = interval(); setInterval(interval, 1000);</script></body></html>";

  server.send(200, "text/html", output);
}

void handleSettings() {
  String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style>* { box-sizing: border-box; padding: 0; margin: 0;  } body {  padding: 30px 100px;  display: flex;  flex-direction: column; align-items: center;  justify-content: flex-start;  gap: 40px;  } h1 {  width: 230px; height: 70px; display: flex;  justify-content: center;  align-items: center;  background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex;  justify-content: center;  align-items: center;  gap: 15px;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .hours, .mins,  .secs { font-size: 30px;  } .alarm_time { width: 230px; height: 30px; display: flex;  justify-content: center;  align-items: center;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .form { width: 230px; height: 200px;  padding: 15px;  color: #2c3e50; background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex;  flex-direction: column; justify-content: space-between; align-items: center;  } .wrapper {  width: 100%;  display: flex;  flex-direction: column; justify-content: center;  align-items: flex-start;  gap: 5px; } label { font-size: 18px;  font-weight: 600; } .input {  width: 100%;  height: 25px; border: none; border-radius: 5px; padding: 0 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .submit { height: 25px; background-color: #fff; font-size: 15px;  font-weight: 600; color: #000;  cursor: pointer;  transition: 0.3s all; } .submit:hover { background-color: #000; color: #fff;  } .back { width: 230px; height: 40px; font-size: 20px;  font-weight: 600; text-decoration: none;  color: #2c3e50; background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex;  justify-content: center;  align-items: center;  transition: 0.3s all; } .back:hover { background-color: #fff; }</style></head><body><h1>Wi-Fi Settings</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='alarm_time'>00:00 - 00:00</div><form action='/saveSettings' method='post' class='form'><div class='wrapper'><label for='ssid'>SSID:</label><input type='text' name='ssid' id='ssid' class='input'></div><div class='wrapper'><label for='ssid'>PASSWORD:</label><input type='password' name='password' id='password' class='input'></div><input type='submit' value='Submit' class='input submit'></form><a href='/' class='back'>Go Back</a><script>function toggleTime() {  const time = new Date();  document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0');  document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0');  } function getTime() {  fetch('/getAlarm')  .then(response => response.json())  .then(data => { let start = data.start.split(':').map(num => num.padStart(2, '0')).join(':'); let end = data.end.split(':').map(num => num.padStart(2, '0')).join(':'); const time = new Date();  const currentTime = time.getHours().toString().padStart(2, '0') + ':' + time.getMinutes().toString().padStart(2, '0') + ':' + time.getSeconds().toString().padStart(2, '0');  document.querySelector('.alarm_time').innerText = start.slice(0, 5) + ' - ' + end.slice(0, 5);  if (currentTime >= start && currentTime < end) {  fetch('/LED_ON').then(response => response.json()).then(data => { console.log(currentTime, start);  }); } if (currentTime == end) { fetch('/LED_OFF').then(response => response.json()).then(data => {  console.log(currentTime, end);  }); } }); } function interval() { toggleTime(); getTime();  } window.onload = interval(); setInterval(interval, 1000);</script></body></html>";
  server.send(200, "text/html", output);
}

void handleSaveSettings() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String newSSID = server.arg("ssid");
    String newPassword = server.arg("password");

    newSSID.toCharArray(ssid, 32);
    newPassword.toCharArray(password, 32);

    EEPROM.begin(512);
    for (int i = 0; i < 32; ++i) {
      EEPROM.write(i, ssid[i]);
      EEPROM.write(32 + i, password[i]);
    }
    EEPROM.commit();
    EEPROM.end();

    String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Settings Saved</title></head><body><h1>Settings Saved</h1><a href='/'>Go Back</a></body></html>";
    server.send(200, "text/html", output);

    ESP.restart();
  } else {
    server.send(400, "text/html", "Invalid Request");
  }
}

void loadSettings() {
  EEPROM.begin(512);
  for (int i = 0; i < 32; ++i) {
    ssid[i] = char(EEPROM.read(i));
    password[i] = char(EEPROM.read(32 + i));
  }
  EEPROM.end();

  if (strlen(ssid) == 0) {
    strcpy(ssid, defaultSSID);
  }
  if (strlen(password) == 0) {
    strcpy(password, defaultPassword);
  }
}

void LightState() {
  digitalWrite(led, !digitalRead(led));
  handleLight();
}

void handleTimer() {
  if (server.hasArg("work") && server.hasArg("rest")) {
    String workTime = server.arg("work");
    String restTime = server.arg("rest");
    
    workHour = workTime.substring(0, 2).toInt();
    workMinute = workTime.substring(3, 5).toInt();
    restHour = restTime.substring(0, 2).toInt();
    restMinute = restTime.substring(3, 5).toInt();

    EEPROM.begin(512);
    EEPROM.write(100, workHour);
    EEPROM.write(101, workMinute);
    EEPROM.write(103, restHour);
    EEPROM.write(104, restMinute);
    EEPROM.commit();
    EEPROM.end();

    Serial.println("Saved On Time: " + String(workHour) + ":" + String(workMinute));
    Serial.println("Saved Off Time: " + String(restHour) + ":" + String(restMinute));
    handleLight();
  }
}

void getTimer() {
  EEPROM.begin(512);
  int work_h = EEPROM.read(100);
  int work_m = EEPROM.read(101);
  int rest_h = EEPROM.read(103);
  int rest_m = EEPROM.read(104);
  EEPROM.end();
  work = work_h * 60 + work_m;
  rest = rest_h * 60 + rest_m;
  a = work;
  b = rest;
}

void handleNewIP() {
  String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style>* { box-sizing: border-box; padding: 0; margin: 0;  } body {  padding: 30px 100px;  display: flex;  flex-direction: column; align-items: center;  justify-content: flex-start;  gap: 40px;  } h1 {  width: 230px; height: 70px; display: flex;  justify-content: center;  align-items: center;  background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex;  justify-content: center;  align-items: center;  gap: 15px;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .hours, .mins,  .secs { font-size: 30px;  } .alarm_time { width: 230px; height: 30px; display: flex;  justify-content: center;  align-items: center;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .back { width: 230px; height: 40px; font-size: 20px;  font-weight: 600; text-decoration: none;  color: #2c3e50; background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex;  justify-content: center;  align-items: center;  transition: 0.3s all; } .back:hover { background-color: #fff; }</style></head><body><h1>New IP Address</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='alarm_time'>00:00 - 00:00</div><a href='/' class='back' id='ip'>New IP: </a><a href='/' class='back'>Go Back</a><script>function toggleTime() { const time = new Date();  document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0');  document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0');  } function getIP() {  fetch('/getIP').then(response => response.json()).then(data => { const ip = data.ip; const ip_link = document.getElementById('ip');  ip_link.innerText = 'New IP: ' + String(ip);  ip_link.setAttribute('href', 'http://' + String(ip)); }); } function getTime() {  fetch('/getAlarm')  .then(response => response.json())  .then(data => { let start = data.start.split(':').map(num => num.padStart(2, '0')).join(':'); let end = data.end.split(':').map(num => num.padStart(2, '0')).join(':'); const time = new Date();  const currentTime = time.getHours().toString().padStart(2, '0') + ':' + time.getMinutes().toString().padStart(2, '0') + ':' + time.getSeconds().toString().padStart(2, '0');  document.querySelector('.alarm_time').innerText = start.slice(0, 5) + ' - ' + end.slice(0, 5);  if (currentTime >= start && currentTime < end) {  fetch('/LED_ON').then(response => response.json()).then(data => { console.log(currentTime, start);  }); } if (currentTime == end) { fetch('/LED_OFF').then(response => response.json()).then(data => {  console.log(currentTime, end);  }); } }); } function interval() { toggleTime(); getTime();  } function load() { getIP();  interval(); } window.onload = load(); setInterval(interval, 1000);</script></body></html>";
  server.send(200, "text/html", output);
}

void getNewIP() {
  String ip = WiFi.localIP().toString();
  String output = "{\"ip\":\"" + ip + "\"}";
  server.send(200, "application/json", output);
}

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(115200);
  dht.begin();

  loadSettings();

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

  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.on("/getData", HTTP_GET, handleGetData);
  server.on("/light", HTTP_GET, handleLight);
  server.on("/LED", HTTP_GET, LightState);
  server.on("/settings", HTTP_GET, handleSettings);
  server.on("/saveSettings", HTTP_POST, handleSaveSettings);
  server.on("/LED_ON", HTTP_GET, ledOn);
  server.on("/LED_OFF", HTTP_GET, ledOff);
  server.on("/timer", HTTP_POST, handleTimer);
  server.on("/manip", HTTP_GET, handleServo);
  server.on("/new_ip", HTTP_GET, handleNewIP);
  server.on("/getIP", HTTP_GET, getNewIP);
  
  getTimer();
  server.begin();
}

void loop() {
  loop_func(a, b, isWork, led, work, rest);
  server.handleClient();
  delay(1000);
}
