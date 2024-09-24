#ifndef IP_H
#define IP_H

#include <ESP8266WebServer.h>

extern ESP8266WebServer server;

void handleNewIP()
{
    String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 30px 100px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 40px; } h1 { width: 230px; height: 70px; display: flex; justify-content: center; align-items: center; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex; justify-content: center; align-items: center; gap: 15px; background-color: #fff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px; font-weight: 600; text-align: center; } .hours, .mins, .secs { font-size: 30px; } .work { width: 230px; height: 30px; display: flex; justify-content: center; align-items: center; background-color: #fff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px; font-weight: 600; text-align: center; } .back { width: 230px; height: 40px; font-size: 20px; font-weight: 600; text-decoration: none; color: #2c3e50; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex; justify-content: center; align-items: center; transition: 0.3s all; } .back:hover { background-color: #fff; } </style></head><body><h1>New IP Address</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='work'>00:00 - 00:00</div><a href='/' class='back' id='ip'>New IP: </a><a href='/' class='back'>Go Back</a><script> function toggleTime() { const time = new Date(); document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0'); document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0'); } function getIP() { fetch('/get_ip').then(response => response.json()).then(data => { const ip = data.ip; const ip_link = document.getElementById('ip'); ip_link.innerText = 'New IP: ' + String(ip); ip_link.setAttribute('href', 'http://' + String(ip)); }); } const info = document.querySelector('.work'); function getInfo() { fetch('/getInfo') .then(response => response.json()) .then(data => { let work = data.work.split(':').map(num => num.padStart(2, '0')).join(':'); let rest = data.rest.split(':').map(num => num.padStart(2, '0')).join(':'); info.innerText = work + ' - ' + rest; console.log(work, rest); }); } function interval() { toggleTime(); getInfo(); } function load() { getIP(); interval(); } window.onload = load(); setInterval(interval, 1000); </script></body></html>";
    server.send(200, "text/html", output);
}

void getNewIP()
{
    String ip = WiFi.localIP().toString();
    String output = "{\"ip\":\"" + ip + "\"}";
    server.send(200, "application/json", output);
}

#endif