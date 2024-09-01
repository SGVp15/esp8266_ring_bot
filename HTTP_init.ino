bool HTTP_init() {
  HTTP.on("/configs.json", handle_ConfigJSON);  // формирование configs.json страницы для передачи данных в web интерфейс
  //  HTTP.on("/", handle_index);  // Index page

  // API для устройства
  HTTP.on("/ssdp", handle_Set_Ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
  HTTP.on("/ssid", handle_Set_Ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
  HTTP.on("/ssidap", handle_Set_Ssidap);  // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  HTTP.on("/BOT_TOKEN", handle_Set_BOT_TOKEN);  // Установить BOT TOKEN /BOT_TOKEN?BOT_TOKEN=5155216886:AAF51QDjtFoM5A3sXH7WTdKO1WBjpMR4kn8
  HTTP.on("/CHAT_ID", handle_Set_CHAT_ID);  // Установить id чата /CHAT_ID?CHAT_ID=1234567890
  HTTP.on("/restart", handle_Restart);   // Перезагрузка модуля по запросу вида /restart?device=ok
  // Добавляем функцию Update для перезаписи прошивки по WiFi при 1М(256K SPIFFS) и выше
  httpUpdater.setup(&HTTP);//, "", OTAUSER, OTAPASSWORD);
  // Запускаем HTTP сервер
  HTTP.begin();
  return true;
}



void handle_index() {
  String s = "Esp-ring OK\n SSDP: /ssdp?ssdp=proba";
  s += "\n Show config: /configs.json";
  s += "\n WIFI: /ssid?ssid=home2&password=12345678";
  s += "\n WIFI AP: /ssidap?ssidAP=home1&passwordAP=8765439";
  s += "\n BOT TOKEN: /BOT_TOKEN?BOT_TOKEN=5155216886:AAF51QDjtFoM5A3sXH7WTdKO1WBjpMR4kn8";
  s += "\n ID Chat Telegram: /CHAT_ID?CHAT_ID=1234567890";
  s += "\n Restart: /restart?device=ok";
  HTTP.send(200, "text/plain", s); // отправляем ответ
}


// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handle_Set_Ssdp() {
  SSDP_Name = HTTP.arg("ssdp");  // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  saveConfig();                 // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");  // отправляем ответ о выполнении
}


// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_Set_Ssid() {
  ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  password_wifi = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}


// Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handle_Set_Ssidap() {              //
  ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  password_AP = HTTP.arg("passwordAP");  // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}


// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = HTTP.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    HTTP.send(200, "text / plain", "Reset OK");  // Oтправляем ответ Reset OK
    ESP.reset();                                // перезагружаем модуль
  } else {                                        // иначе
    HTTP.send(200, "text / plain", "No Reset");  // Oтправляем ответ No Reset
  }
}


// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/BOT_TOKEN?BOT_TOKEN=home
void handle_Set_BOT_TOKEN() {
  BOT_TOKEN = HTTP.arg("BOT_TOKEN");   // Получаем значение BOT_TOKEN из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");  // отправляем ответ о выполнении
}


// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/CHAT_ID?CHAT_ID=
void handle_Set_CHAT_ID() {
  CHAT_ID = HTTP.arg("CHAT_ID");   // Получаем значение BOT_TOKEN из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");  // отправляем ответ о выполнении
}

void handle_ConfigJSON() {
  String root;  // Формировать строку для отправки в браузер json формат
  // {"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266

  DynamicJsonDocument doc(1024);
  // вызовите парсер JSON через экземпляр jsonDocument
  deserializeJson(doc, root);
  // Заполняем поля json
  doc["SSDP"] = SSDP_Name;
  doc["ssidAP"] = ssidAP;
  doc["passwordAP"] = password_AP;
  doc["ssid"] = ssid;
  doc["password"] = password_wifi;
  doc["timezone"] = timezone;
  doc["ip"] = WiFi.localIP().toString();
  doc["time"] = GetTime();
  doc["date"] = GetDate();
  doc["BOT_TOKEN"] = BOT_TOKEN;
  doc["CHAT_ID"] = CHAT_ID;
  // Помещаем созданный json в переменную doc

  serializeJson(doc, root);
  HTTP.send(200, "text/json", root);
}
