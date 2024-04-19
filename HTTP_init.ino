void HTTP_init(void) {

  server.on("/configs.json", handle_ConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
  // API для устройства
  server.on("/ssdp", handle_Set_Ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
  server.on("/ssid", handle_Set_Ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
  server.on("/ssidap", handle_Set_Ssidap); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  server.on("/restart", handle_Restart);   // Перезагрузка модуля по запросу вида /restart?device=ok
  // Добавляем функцию Update для перезаписи прошивки по WiFi при 1М(256K SPIFFS) и выше
  httpUpdater.setup(&HTTP);
  // Запускаем HTTP сервер
  server.begin();
}


// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handle_Set_Ssdp() {
  SSDP_Name = server.arg("ssdp"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  saveConfig();                 // Функция сохранения данных во Flash
  server.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}


// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_Set_Ssid() {
  _ssid = server.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  _password = server.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash
  server.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}


//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handle_Set_Ssidap() {              //
  _ssidAP = server.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  _passwordAP = server.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash
  server.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = server.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    server.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                                // перезагружаем модуль
  }
  else {                                        // иначе
    server.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
  }
}

void handle_ConfigJSON() {
  String root = "{}";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  
  DynamicJsonDocument doc(1024);
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //JsonObject json = jsonBuffer.parseObject(root);
  deserializeJson(doc, input);
  // Заполняем поля json
  doc["SSDP"] = SSDP_Name;
  doc["ssidAP"] = _ssidAP;
  doc["passwordAP"] = _passwordAP;
  doc["ssid"] = _ssid;
  doc["password"] = _password;
  doc["timezone"] = timezone;
  doc["ip"] = WiFi.localIP().toString();
  doc["time"] = GetTime();
  doc["date"] = GetDate();
  // Помещаем созданный json в переменную root
  root = "";
  json.printTo(root);
  server.send(200, "text/json", root);
}
