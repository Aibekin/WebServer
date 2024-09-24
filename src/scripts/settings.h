#ifndef SETTINGS_H
#define SETTINGS_H

#include <ESP8266WebServer.h>
#include <EEPROM.h>

extern const char *defaultSSID;
extern const char *defaultPassword;
extern ESP8266WebServer server;
extern char ssid[32];
extern char password[32];

void loadSettings()
{
    EEPROM.begin(512);
    for (int i = 0; i < 32; ++i)
    {
        ssid[i] = char(EEPROM.read(i));
        password[i] = char(EEPROM.read(32 + i));
    }
    EEPROM.end();

    if (strlen(ssid) == 0)
    {
        strcpy(ssid, defaultSSID);
    }
    if (strlen(password) == 0)
    {
        strcpy(password, defaultPassword);
    }
}

void handleSettings()
{
    String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Document</title><style> * { box-sizing: border-box; padding: 0; margin: 0; } body { padding: 30px 100px; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; gap: 40px; } h1 { width: 230px; height: 70px; display: flex; justify-content: center; align-items: center; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .time { width: 230px; height: 40px; padding: 5px; display: flex; justify-content: center; align-items: center; gap: 15px; background-color: #fff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px; font-weight: 600; text-align: center; } .hours, .mins, .secs { font-size: 30px; } .work { width: 230px; height: 30px; display: flex; justify-content: center; align-items: center; background-color: #fff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); color: #2c3e50; font-size: 20px; font-weight: 600; text-align: center; } .form { width: 230px; height: 200px; padding: 15px; color: #2c3e50; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex; flex-direction: column; justify-content: space-between; align-items: center; } .wrapper { width: 100%; display: flex; flex-direction: column; justify-content: center; align-items: flex-start; gap: 5px; } label { font-size: 18px; font-weight: 600; } .input { width: 100%; height: 25px; border: none; border-radius: 5px; padding: 0 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); } .submit { height: 25px; background-color: #fff; font-size: 15px; font-weight: 600; color: #000; cursor: pointer; transition: 0.3s all; } .submit:hover { background-color: #000; color: #fff; } .back { width: 230px; height: 40px; font-size: 20px; font-weight: 600; text-decoration: none; color: #2c3e50; background-color: #77c9ff; border-radius: 15px; box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24); display: flex; justify-content: center; align-items: center; transition: 0.3s all; } .back:hover { background-color: #fff; } </style></head><body><h1>Wi-Fi Settings</h1><div class='time'><div class='hours'>--</div>:<div class='mins'>--</div>:<div class='secs'>--</div></div><div class='work'>00:00 - 00:00</div><form action='/saveSettings' method='post' class='form'><div class='wrapper'><label for='ssid'>SSID:</label><input type='text' name='ssid' id='ssid' class='input'></div><div class='wrapper'><label for='ssid'>PASSWORD:</label><input type='password' name='password' id='password' class='input'></div><input type='submit' value='Submit' class='input submit'></form><a href='/' class='back'>Go Back</a><script> function toggleTime() { const time = new Date(); document.querySelector('.hours').innerText = time.getHours().toString().padStart(2, '0'); document.querySelector('.mins').innerText = time.getMinutes().toString().padStart(2, '0'); document.querySelector('.secs').innerText = time.getSeconds().toString().padStart(2, '0'); } const info = document.querySelector('.work'); function getInfo() { fetch('/getInfo') .then(response => response.json()) .then(data => { let work = data.work.split(':').map(num => num.padStart(2, '0')).join(':'); let rest = data.rest.split(':').map(num => num.padStart(2, '0')).join(':'); info.innerText = work + ' - ' + rest; console.log(work, rest); }); } function interval() { toggleTime(); getInfo(); } window.onload = interval(); setInterval(interval, 1000); </script></body></html>";
    server.send(200, "text/html", output);
}

void handleSaveSettings()
{
    if (server.hasArg("ssid") && server.hasArg("password"))
    {
        String newSSID = server.arg("ssid");
        String newPassword = server.arg("password");

        newSSID.toCharArray(ssid, 32);
        newPassword.toCharArray(password, 32);

        EEPROM.begin(512);
        for (int i = 0; i < 32; ++i)
        {
            EEPROM.write(i, ssid[i]);
            EEPROM.write(32 + i, password[i]);
        }
        EEPROM.commit();
        EEPROM.end();

        String output = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Settings Saved</title></head><body><h1>Settings Saved</h1><a href='/'>Go Back</a></body></html>";
        server.send(200, "text/html", output);

        ESP.restart();
    }
    else
    {
        server.send(400, "text/html", "Invalid Request");
    }
}

#endif