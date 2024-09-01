bool WIFIinit() {
  loadConfig();
  // Попытка подключения к точке доступа
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password_wifi.c_str());

  // Делаем проверку подключения до тех пор пока счетчик tries
  // не станет равен нулю или не получим подключение
  byte tries = 11;
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin("n5", "12345678901234567890a");
    byte tries = 11;
    while (--tries && WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(1000);
    }
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    // Если не удалось подключиться запускаем в режиме AP
    Serial.print("WiFi up AP");
  }
  else {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    bot.sendMessage("Бот запущен ✅", CHAT_ID);
    write_log("Бот запущен");
    return true;
  }
  return false;
}

bool wifi_init_AP_Mode() {
  IPAddress apIP(192, 168, 4, 1);
  // Отключаем WIFI
  WiFi.disconnect();
  // Меняем режим на режим точки доступа
  WiFi.mode(WIFI_AP);
  // Задаем настройки сети
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  // Включаем WIFI в режиме точки доступа с именем и паролем
  // хронящихся в переменных ssidAP password_AP
  WiFi.softAP(ssidAP.c_str(), password_AP.c_str());
  is_AP_mode = true;
  return true;
}


void wifi_start() {
  Serial.print("[ Start Wi-fi ");
  if ( WIFIinit() ) {
    Serial.println(F("ok. ]"));
    is_AP_mode = false;
  } else {
    is_AP_mode = true;
    Serial.println(F("fail. ]"));

    Serial.print("[ Start WIFI AP mode ");
    if ( wifi_init_AP_Mode() ) {
      Serial.println(F("ok. ]"));
    } else {
      Serial.println(F("fail. ]"));
    }
  }
}
