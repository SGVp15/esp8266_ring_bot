// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    // если файл не найден
    Serial.println("Failed to open config file");
    // Создаем файл, залив в него данные по умолчанию
    saveConfig();
    configFile.close();
    return false;
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    return false;
  }

  // загружаем файл конфигурации в глобальную переменную
  jsonConfig = configFile.readString();
  configFile.close();
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
  DynamicJsonDocument doc(1024);
  // вызовите парсер JSON через экземпляр jsonDocument
  // строку возьмем из глобальной переменной String jsonConfig
  deserializeJson(doc, jsonConfig);
  
  // Теперь можно получить значения из doc
  ssidAP = doc["ssidAPName"].as<String>();  // Так получаем строку
  password_AP = doc["ssidAPPassword"].as<String>();
  timezone = doc["timezone"];               // Так получаем число
  SSDP_Name = doc["SSDPName"].as<String>();
  ssid = doc["ssidName"].as<String>();
  password_wifi = doc["ssidPassword"].as<String>();
  BOT_TOKEN = doc["BOT_TOKEN"].as<String>();
  CHAT_ID = doc["CHAT_ID"].as<String>();
  
  return true;
}


// Запись данных в файл config.json
bool saveConfig() {
  // Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
  DynamicJsonDocument doc(1024);
  // вызовите парсер JSON через экземпляр jsonDocument
  //  deserializeJson(doc, jsonConfig);
  // Заполняем поля json
  doc["SSDPName"] = SSDP_Name;
  doc["ssidAPName"] = ssidAP;
  doc["ssidAPPassword"] = password_AP;
  doc["ssidName"] = ssid;
  doc["ssidPassword"] = password_wifi;
  doc["timezone"] = timezone;
  doc["BOT_TOKEN"] = BOT_TOKEN;
  doc["CHAT_ID"] = CHAT_ID;
  
  // Помещаем созданный json в глобальную переменную serializeJson(doc, jsonConfig);
  serializeJson(doc, jsonConfig);
  // Открываем файл для записи
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    // Serial.println("Failed to open config file for writing");
    configFile.close();
    return false;
  }
  // Записываем строку json в файл
  serializeJson(doc, configFile);

  configFile.close();
  return true;
}
