// обработчик сообщений
void bot_handler(FB_msg & msg) {
  if (help_handler(msg)) return;
  Serial.println("bot_handler");
  // выводим всю информацию о сообщении
  Serial.println(msg.toString());

  String text = msg.text;

  if (msg.OTA) {
    String sticker = F("CAACAgIAAxkBAAEFEdxirLREsjWeMm1PqTd-YBHjoBqryQACTwADr8ZRGucm1YMqnhvOJAQ");
    bot.sendSticker(sticker, msg.chatID);
    bot.update();
  }

  Serial.println("new message:");
  Serial.println(text);
  write_log(text);
  String message = "";

  if (text == "/blink")
  {
    blink = !blink;
    if (blink) {
      message += "Blink :)";
      String sticker = F("CAACAgIAAxkBAAEFEdxirLREsjWeMm1PqTd-YBHjoBqryQACTwADr8ZRGucm1YMqnhvOJAQ");
      bot.sendSticker(sticker, msg.chatID);
    }
    else {
      message += "Blink_off :) ";
    }
  }
  //    else if (text == "/reset")
  //    {
  //      bot.sendMessage("reset - ok", msg.chatID);
  //      //      ESP.reset();
  //    }
  else if (text == "/on")
  {
    digitalWrite(LED_PIN, 0);
    message += "On: ";
    is_http_enable = true;
  }
  else if (text == "/off")
  {
    digitalWrite(LED_PIN, 1);
    message += "Off: ";
    is_http_enable = false;
  }
  else if (text == "/id")
  {
    message = "Ваш id:\n";
    message += msg.chatID;
  }
  else if (text == "/state")
  {
    message += "State: ";
    if (is_http_enable) {
      message += "✅";
    } else {
      message += "❌";
    }
  }
  else if (text == "/help_http")
  {
    message += "\n Установить имя SSDP устройства по запросу вида /ssdp?ssdp=WIFI1";
    message += "\n Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678";
    message += "\n Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439";
    message += "\n Установить BOT TOKEN /BOT_TOKEN?BOT_TOKEN=5155216886:AAF51QDjtFoM5A3sXH7WTdKO1WBjpMR4kn8";
    message += "\n Установить id чата /CHAT_ID?CHAT_ID=1234567890";
    message += "\n Перезагрузка модуля по запросу вида /restart?device=ok";
  }

  else if (text == "/ip")
  {
    message += "IP address: ";
    message += WiFi.localIP().toString();
  }
  else if (text == "/http_on")
  {
    is_http_enable = true;
    HTTP.begin();
    message += "HTTP server: ON";
  }
  else if (text == "/http_off")
  {
    is_http_enable = false;
    message += "HTTP server: OFF";
    HTTP.close();
  }
  else if (text == "/log")
  {
    message += "История:\n";
    message += read_log();
  }
  else if (text == "/download_log")
  {
    message += "Файл истории:\n";

    // bot.send_document(callback_query.from_user.id, f, reply_markup=kb.inline_kb_main);
  }
  else if (text == "/clear_log")
  {
    message += "Удалил историю:\n";
    clear_log();
  }
  //  else if (text == "/start" or text == "/help")
  //  {
  //    message += "Добрый день!\n";
  //    message += "я звонок на esp8266.\n";
  //    message += "/on - ВКЛ оповещение\n";
  //    message += "/off - ВЫКЛ оповещение \n";
  //    message += "/state - текущее состояние.\n";
  //    message += "/blink - поморгать.\n";
  //    message += "/ip - узнать ip адрес.\n";
  //    message += "/log - показать историю.\n";
  //    message += "/download_log - скачать файл истории.\n";
  //    message += "/clear_log - удалить историю.\n";
  //  }
  else {
    message = "Не понимаю что это значит.\nВоспользуйтесь командой /help";
  }
  String main_keyboard = "[ [\"/http_on\", \"/http_off\"], [\"/blink\", \"/ip\"], [\"/help_http\"], [\"/clear_log\"], [\"/download_log\"] ]";
  //    bot.sendMessageWithReplyKeyboard(CHAT_ID, message, "", main_keyboard, true);

  bot.sendMessage(message, msg.chatID);
  Serial.println(message);
  write_log(message);
}


bool help_handler(FB_msg & msg) {
  if (msg.text == "/start" or msg.text == "/help")
  {
    String message = "";
    message += "Добрый день!\n";
    message += "я звонок на esp8266.\n";
    message += "/on - ВКЛ оповещение\n";
    message += "/off - ВЫКЛ оповещение \n";
    message += "/state - текущее состояние.\n";
    message += "/blink - поморгать.\n";
    message += "/ip - узнать ip адрес.\n";
    message += "/log - показать историю.\n";
    message += "/download_log - скачать файл истории.\n";
    message += "/clear_log - удалить историю.\n";
    bot.sendMessage(message, msg.chatID);
    return true;
  }
  return false;
}
//  bot.setChatID(CHAT_ID);
