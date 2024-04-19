// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
  // если файл не найден  
    Serial.println("Failed to open config file");
  //  Создаем файл запиав в него аные по умолчанию
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
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //  строку возьмем из глобальной переменной String jsonConfig
  // JsonObject root = jsonBuffer.parseObject(jsonConfig);
    deserializeJson(doc, input);
  // Теперь можно получить значения из root  
    _ssidAP = doc["ssidAPName"].as<String>(); //  получаем строку
    _passwordAP = doc["ssidAPPassword"].as<String>();
    timezone = doc["timezone"];               // получаем число
    SSDP_Name = doc["SSDPName"].as<String>();
    _ssid = doc["ssidName"].as<String>();
    _password = doc["ssidPassword"].as<String>();
    return true;
}

// Запись данных в файл config.json
bool saveConfig() {
  // Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
  DynamicJsonDocument doc(1024);
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject json = deserializeJson(doc, input);
  
  // Заполняем поля json 
  doc["SSDPName"] = SSDP_Name;
  doc["ssidAPName"] = _ssidAP;
  doc["ssidAPPassword"] = _passwordAP;
  doc["ssidName"] = _ssid;
  doc["ssidPassword"] = _password;
  doc["timezone"] = timezone;
  // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  json.printTo(jsonConfig);
  // Открываем файл для записи
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    //Serial.println("Failed to open config file for writing");
    configFile.close();
    return false;
  }
  // Записываем строку json в файл 
  json.printTo(configFile);
  configFile.close();
  return true;
  }
