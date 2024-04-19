#include <ArduinoJson.h>        //Установить из менеджера библиотек.
//                                ЗАПИСЬ И ЧТЕНИЕ ПАРАМЕТРОВ КОНФИГУРАЦИИ В ФАЙЛ. Видео с уроком http://esp8266-arduinoide.ru/step7-fileconfig/
#include <ESP8266HTTPUpdateServer.h>  //Содержится в пакете.
#include <ESP8266WiFi.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step1-wifi
#include <ESP8266WebServer.h>   //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step2-webserver
#include <ESP8266SSDP.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step3-ssdp
#include <FS.h>                 //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step4-fswebserver
//                                ПЕРЕДАЧА ДАННЫХ НА WEB СТРАНИЦУ. Видео с уроком http://esp8266-arduinoide.ru/step5-datapages/
//                                ПЕРЕДАЧА ДАННЫХ С WEB СТРАНИЦЫ.  Видео с уроком http://esp8266-arduinoide.ru/step6-datasend/
//Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step4-fswebserver



// Объект для обнавления с web страницы 
ESP8266HTTPUpdateServer httpUpdater;

// Определяем переменные wifi
String _ssid     = "home"; // Для хранения SSID
String _password = "i12345678"; // Для хранения пароля сети
String _ssidAP = "WiFi_Ring";   // SSID AP точки доступа
String _passwordAP = "1234567890"; // пароль точки доступа
String SSDP_Name = "Update-Time"; // Имя SSDP
int timezone = 3;               // часовой пояс GTM
int port = 80;

String jsonConfig = "{}";

// Для файловой системы
File fsUploadFile;

String input = "./config.json";


ESP8266WebServer server(port);

void setup() {  // Запуск телеграмбота
  start_bot();

  Serial.begin(115200);
  Serial.println("");
  
  // Запускаем файловую систему
  Serial.println("Start 4-FS");
  FS_init();

  // Первоначальный конфиг
  Serial.println("Step7-FileConfig");
  loadConfig();
  
  // Запускаем WIFI
  Serial.println("Start 1-WIFI");
  WIFIinit();
  
  
  // Получаем время из сети
  Serial.println("Start 8-Time");
  Time_init();
  
  // Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start 3-SSDP");
  SSDP_init();
  
  // Настраиваем и запускаем HTTP интерфейс
  Serial.println("Start 2-WebServer");
  HTTP_init();
}

void loop() {
  server.handleClient();
  delay(1);
}
