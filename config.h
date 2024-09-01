// ------------------------------------------------------------------------------------------------------------------------------
//      Webserver       Webserver       Webserver       Webserver       Webserver       Webserver       Webserver       Webserver
// ------------------------------------------------------------------------------------------------------------------------------
#include <ESP8266WebServer.h>   // Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step2-webserver
#include <ESP8266HTTPUpdateServer.h>  // Содержится в пакете.

// Объект для обновления с web страницы
ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

// ---------------------------------------------------------------
//     Debug     Debug     Debug     Debug     Debug     Debug
// ---------------------------------------------------------------
#define len(x) sizeof(x) / sizeof(x[0])

// ---------------------------------------------------------------
//     ArduinoOTA     ArduinoOTA     ArduinoOTA     ArduinoOTA
// ---------------------------------------------------------------
#include <ArduinoOTA.h>         // Прошивка по WIFI
#include <ESP8266mDNS.h>
#include <WIFIUdp.h>

#define OTAUSER         "admin1"    // Логин для входа в OTA
#define OTAPASSWORD     "admin1"    // Пароль для входа в ОТА

// ---------------------------------------------------------------
//      Файловой система    Файловой система    Файловой система
// ---------------------------------------------------------------
#include <FS.h>                 // Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step4-fswebserver
#include <ArduinoJson.h>        // Установить из менеджера библиотек.

File fsUploadFile;

// ---------------------------------------------------------------
//     SSDP    SSDP    SSDP    SSDP    SSDP    SSDP    SSDP
// ---------------------------------------------------------------
#include <ESP8266SSDP.h>        // Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step3-ssdp

String SSDP_Name;  // Имя SSDP

// ---------------------------------------------------------------
//     Определяем переменные wifi     Определяем переменные wifi
// ---------------------------------------------------------------
#include <ESP8266WiFi.h>        // Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step1-wifi

String ssid;       // = "n5";              // Для хранения SSID
String password_wifi;   // = "1234567890";      // Для хранения пароля сети
String ssidAP;     // = "ESP-8266-ring";   // SSID AP точки доступа
String password_AP; // = "1234567890";      // пароль точки доступа

bool is_AP_mode = true;                    // Статус wifi точки, по ней проверяем в каком статусе WIFI модуль
const unsigned long wifi_ap_timeout = 3 * 60 * 1000; // Время мс, через которое esp пытается подключится к wifi (5 минут)
int timezone;       // = 3;                 // часовой пояс GTM

// ---------------------------------------------------------------
//     Config     Config     Config     Config     Config     Conf
// ---------------------------------------------------------------
String jsonConfig = "/config.json";
String log_file = "/log.txt";
int port = 80;
bool is_http_enable = true;

// ---------------------------------------------------------------
//     Pins     Pins     Pins     Pins     Pins     Pins     Pins
// ---------------------------------------------------------------
#define LED_PIN D4   // Светодиод
#define RING_PIN D5  // Кнопка
volatile bool status_ring_button = false;
void ICACHE_RAM_ATTR ring() {
  status_ring_button = true;
}


// ---------------------------------------------------------------
//     State_Led    State_Led    State_Led    State_Led    State_Led
// ---------------------------------------------------------------
bool blink = false;
bool led_state = true;


// ---------------------------------------------------------------
//     Telegram     Telegram     Telegram     Telegram     Telegra
// ---------------------------------------------------------------
#include <FastBot.h>

String BOT_TOKEN;
String CHAT_ID;
bool state_ring = false;  // Звонок
const unsigned long bot_timeout = 3 * 1000;  // Время мс, через которое бот проверяет новые сообщения
FastBot bot;
// посмотреть

bool send_ring = false;
//@idstickerbot
