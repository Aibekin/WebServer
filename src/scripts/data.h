#ifndef DATA_H
#define DATA_H

#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN 0
#define DHTTYPE DHT11 // Тип датчика

DHT dht(DHTPIN, DHTTYPE);

extern ESP8266WebServer server;

void handleData()
{
    String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style>* { box-sizing: border-box; padding: 0; margin: 0;  } body {  padding: 30px 100px;  display: flex;  flex-direction: column; align-items: center;  justify-content: flex-start;  gap: 40px;  } h1 {  width: 230px; height: 70px; display: flex;  justify-content: center;  align-items: center;  background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex;  justify-content: center;  align-items: center;  gap: 15px;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .hours, .mins,  .secs { font-size: 30px;  } .alarm {  width: 230px; height: 30px; display: flex;  justify-content: center;  align-items: center;  background-color: #fff; border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px;  font-weight: 600; text-align: center; } .temp_data {  display: flex;  flex-direction: column; align-items: center;  justify-content: space-between; padding: 15px;  width: 230px; height: 150px;  background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .info { display: flex;  align-items: center;  justify-content: flex-start;  gap: 50px;  } .title {  color: #2c3e50; font-size: 35px;  width: 35px;  height: 35px; text-align: center; } .title img {  width: 100%;  height: 100%; } .info span {  color: #2c3e50; font-size: 35px;  } .back { width: 230px; height: 40px; font-size: 20px;  font-weight: 600; text-decoration: none;  color: #2c3e50; background-color: #77c9ff;  border-radius: 15px;  box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex;  justify-content: center;  align-items: center;  transition: 0.3s all; } .back:hover { background-color: #fff; }</style></head><body><h1>Data</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='alarm'>00:00 - 00:00</div><div class='temp_data'><div class='info'><div class='title'>t&deg;</div><span class='value' id='temp'>--&deg;C</span></div><div class='info'><div class='title'><img  src='https://www.freepnglogos.com/uploads/drop-png/waterdrop-sharp-glimpse-illustration-transparent-png-8.png'></div><span class='value' id='hum'>--%</span></div></div><a href='/' class='back'>Go Back</a><script>function toggleTime() { const time = new Date();  document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0');  document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0');  } function getTime() {  fetch('/getAlarm').then(response => response.json()).then(data => { let start = data.start.split(':').map(num => num.padStart(2, '0')).join(':'); let end = data.end.split(':').map(num => num.padStart(2, '0')).join(':'); const time = new Date();  const currentTime = time.getHours().toString().padStart(2, '0') + ':' + time.getMinutes().toString().padStart(2, '0') + ':' + time.getSeconds().toString().padStart(2, '0');  document.querySelector('.alarm').innerText = start.slice(0, 5) + ' - ' + end.slice(0, 5); if (currentTime >= start && currentTime < end) {  fetch('/LED_ON').then(response => response.json()).then(data => { console.log(currentTime, start);  }); } if (currentTime == end) { fetch('/LED_OFF').then(response => response.json()).then(data => {  console.log(currentTime, end);  }); } }); } function updateData() { fetch('/getData').then(response => response.json()).then(data => {  document.getElementById('temp').innerText = data.temperature + '°C';  document.getElementById('hum').innerText = data.humidity + '%'; }); } function interval() { toggleTime(); updateData(); getTime();  } window.onload = interval(); setInterval(interval, 1000);</script></body></html>";

    server.send(200, "text/html", output);
}

void handleGetData()
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    String json = "{\"temperature\":" + String(t) + ",\"humidity\":" + String(h) + "}";
    server.send(200, "application/json", json);
}

#endif