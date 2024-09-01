//=================================================================
//   Программа Звонок для ESP-8266
//   отправляет уведомления в телеграм бот о нажатой кнопке звонка
//=================================================================
#include "config.h"
#include "logs.h"


//==================================================================
//      SETUP      SETUP      SETUP      SETUP      SETUP      SETUP
//==================================================================

void setup() {
  Serial.begin(115200);
  Serial.println("");

  pinMode(LED_PIN, OUTPUT);
  pinMode(RING_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RING_PIN), ring, FALLING);
  digitalWrite(LED_PIN, 1);

  ESP8266WebServer HTTP(port);

  // Запускаем файловую систему
  Serial.print("[ Start FS: ");
  if ( FS_init() ) {
    Serial.println(F("OK. ]"));
  } else {
    Serial.println(F("ERROR. ]"));
  }

  Serial.print("[ Load FileConfig ");
  if ( loadConfig() ) {
    Serial.println(F("OK. ]"));
  } else {
    Serial.println(F("ERROR. ]"));
  }

  // Запускаем WIFI
  wifi_start();

  // Настраиваем и запусаем телеграм бота
  bot.attach(bot_handler);
  bot.setToken(BOT_TOKEN);
  Serial.println(BOT_TOKEN);

  //  bot.showMenu("/http_on\t /http_off \t /blink \t ip \n help_http \t /clear_log \t /download_log", CHAT_ID);
  //  String sticker = F("CAACAgIAAxkBAAEE6TBimvrjTXgKQyhdMBxohA_HZ6VtjAACTAADr8ZRGjrfxZE8LKM_JAQ");
  //  bot.sendSticker(sticker, CHAT_ID);

  // Получаем время из сети
  if ( Time_init() ) {
    Serial.println(F("OK. ]"));
  } else {
    Serial.println(F("ERROR. ]"));
  }

  // Настраиваем и запускаем SSDP интерфейс
  Serial.print("[ Start SSDP: ");
  if ( SSDP_init() ) {
    Serial.println(F("OK. ]"));
  } else {
    Serial.println(F("ERROR. ]"));
  }

  // Настраиваем и запускаем HTTP интерфейс
  Serial.print("[ Start WebServer: ");
  if ( HTTP_init() ) {
    Serial.println(F("OK. ]"));
  } else {
    Serial.println(F("ERROR. ]"));
  }

  ArduinoOTA.setHostname("Esp-8266-ring");
  ArduinoOTA.setPassword(OTAPASSWORD);
  ArduinoOTA.begin();
}


//=================================================================
//      MAIN      MAIN      MAIN      MAIN      MAIN      MAIN
//=================================================================

void loop() {
  ArduinoOTA.handle();
  ring_check();

  bot.tick();
  mashine_state();

  if (is_http_enable) {
    HTTP.handleClient();
    if (blink == false) digitalWrite(LED_PIN, 0);
  }
}
