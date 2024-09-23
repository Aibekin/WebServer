#ifndef AUTO1_H
#define AUTO1_H

#include <ESP8266WebServer.h>
#include <Ticker.h>
#include "loop.h"

extern Ticker timer;
extern ESP8266WebServer server;
extern int workHour, workMinute, workSeconds, restHour, restMinute, restSeconds;
extern bool isAutoMode, isWork;
extern int autoModeState, a, b, work, rest;

void getTimer()
{
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

void handleAuto()
{
    String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 30px 100px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 30px; } h1 { width: 230px; height: 70px; display: flex; justify-content: center; align-items: center; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex; justify-content: center; align-items: center; gap: 15px; background-color: #fff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px; font-weight: 600; text-align: center; } .hours, .mins, .secs { font-size: 30px; } .buttons { width: 230px; display: flex; justify-content: space-between; align-items: center; } .button { width: 65px; height: 30px; background-color: #77c9ff; border: none; border-radius: 5px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 14px; cursor: pointer; transition: 0.3s all; } .active { transform: translateY(4px); background-color: #5b9cc7; } .tabs { width: 230px; } .tab { width: 230px; display: none; padding: 15px; justify-content: center; gap: 30px; align-items: center; flex-wrap: wrap; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .tab-active { display: flex; } .switcher { background-color: #fff; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); border: none; border-radius: 30px; width: 40px; height: 20px; cursor: pointer; padding: 5px; transition: 0.3s all; } .switcher:before { content: ''; display: block; background-color: #000; width: 10px; height: 10px; border-radius: 50%; margin-left: 0; transition: 0.3s ease-in; } .switcher-active { background-color: green; } .switcher-active:before { margin-left: 20px; background-color: #fff; } .mode { width: 230px; display: flex; justify-content: space-between; align-items: center; } .submit { background-color: red; border: none; border-radius: 5px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); width: 85px; height: 40px; color: #fff; font-size: 25px; cursor: pointer; width: 100%; } .timer { display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 30px; width: 100%; } .wrapper { width: 100%; display: flex; align-items: center; justify-content: space-between; } label { font-size: 20px; color: #2c3e50; font-weight: 600; } .timer_input { width: 110px; height: 30px; background-color: #fff; font-size: 15px; color: black; display: flex; align-items: center; justify-content: center; gap: 20px; border: none; border-radius: 5px; } .back { width: 230px; height: 40px; font-size: 20px; font-weight: 600; text-decoration: none; color: #2c3e50; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex; justify-content: center; align-items: center; transition: 0.3s all; } .back:hover { background-color: #fff; } .form { width: 200px; display: flex; align-items: center; justify-content: space-between; } .automode { width: 150px; height: 30px; border: none; border-radius: 5px; cursor: pointer; background-color: #fff; color: #000; transition: 0.3s all; } .automode.automode-active { background-color: green; color: #fff; } .auto { display: flex; flex-direction: column; justify-content: center; align-items: center; gap: 10px; } .more { width: 30px; height: 30px; border: none; border-radius: 5px; cursor: pointer; background-color: #fff; color: #000; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .more.more-active { background-color: #ccc; color: #000; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .descr { display: none; } .descr-active { display: block; width: 150px; text-align: center; } .arrow { transform: rotateX(0); pointer-events: none; transition: 0.3s all; } .arrow.arrow-active { transform: rotateX(180deg); } .work { width: 230px; height: 30px; display: flex; justify-content: center; align-items: center; background-color: #fff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px; font-weight: 600; text-align: center; } </style></head><body><h1>Light</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='work'>00:00 - 00:00</div><div class='mode'>Автоматический режим: <div class='switcher switcher-active'></div></div><div class='tabs'><div class='tab tab-active'><div class='auto'><div class='form'><input type='submit' value='AutoMode1' class='automode'><button class='more'><div class='arrow'>▲</div></button></div><div class='descr'>Это автомод номер 1</div></div><div class='auto'><div class='form'><input type='submit' value='AutoMode2' class='automode'><button class='more'><div class='arrow'>▲</div></button></div><div class='descr'>Это автомод номер 2</div></div><div class='auto'><div class='form'><input type='submit' value='AutoMode3' class='automode'><button class='more'><div class='arrow'>▲</div></button></div><div class='descr'>Это автомод номер 3</div></div></div><div class='tab'><form action='/timer' class='timer' method='post' onsubmit='getTimer()'><div class='wrapper'><label for='work'>Work</label><input type='time' name='work' id=work' class='timer_input'></div><div class='wrapper'><label for='rest'>Rest</label><input type='time' name='rest' id=rest' class='timer_input'></div><input type='submit' value='Submit' class='submit'></form></div></div><a href='/' class='back'>Go Back</a><script type='module'> function toggleTime() { const time = new Date(); document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0'); document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0'); } function getInfo() { fetch('/getInfo') .then(response => response.json()) .then(data => { let work = data.work.split(':').map(num => num.padStart(2, '0')).join(':'); let rest = data.rest.split(':').map(num => num.padStart(2, '0')).join(':'); info.innerText = work + ' - ' + rest; console.log(work, rest); }); } const buttons = document.querySelectorAll('.button'); const inputs = document.querySelectorAll('.automode'); const mores = document.querySelectorAll('.more'); const descrs = document.querySelectorAll('.descr'); const tabs = document.querySelectorAll('.tab'); const info = document.querySelector('.work'); const arrow = document.querySelectorAll('.arrow'); mores.forEach((a, i) => { a.addEventListener('click', (event) => { event.target.classList.toggle('more-active'); descrs[i].classList.toggle('descr-active'); arrow[i].classList.toggle('arrow-active'); }); }); const switcher = document.querySelector('.switcher'); switcher.addEventListener('click', () => { switcher.classList.toggle('switcher-active'); if (switcher.classList.contains('switcher-active')) { tabs[0].classList.add('tab-active'); tabs[1].classList.remove('tab-active'); } else { tabs[1].classList.add('tab-active'); tabs[0].classList.remove('tab-active'); } }); inputs.forEach((a, i) => { a.addEventListener('click', (event) => { inputs.forEach((a) => a.classList.remove('automode-active')); event.target.classList.add('automode-active'); fetch('/autoMode?mode=' + String(i + 1)); }); }); function interval() { toggleTime(); getInfo(); } window.onload = interval(); setInterval(interval, 1000); </script></body></html>";
    getTimer();
    server.send(200, "text/html", output);
}

void saveAutoModeState(int state)
{
    EEPROM.begin(512);
    EEPROM.write(110, state);
    EEPROM.commit();
    EEPROM.end();
}

void handleTimer()
{
    if (server.hasArg("work") && server.hasArg("rest"))
    {
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
        isAutoMode = false;
        autoModeState = 0;
        saveAutoModeState(autoModeState);
        timer.detach();
        loop_func();
        handleAuto();
    }
}

void getInfo()
{
    EEPROM.begin(512);
    int work_h = EEPROM.read(100);
    int work_m = EEPROM.read(101);
    int rest_h = EEPROM.read(103);
    int rest_m = EEPROM.read(104);
    EEPROM.end();
    String json = "{\"work\":\"" + String(work_h) + ":" + String(work_m) + "\", \"rest\":\"" + String(rest_h) + ":" + String(rest_m) + "\"}";
    server.send(200, "application/json", json);
}

#endif