#include <time.h>


bool Time_init() {
  HTTP.on("/Time", handle_Time);     // Синхронизировать время устройства по запросу вида /Time
  HTTP.on("/TimeZone", handle_time_zone);    // Установка времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
  timeSynch(timezone);
  return true;
}


void timeSynch(int zone) {
  if (WiFi.status() == WL_CONNECTED) {
    // Настройка соединения с NTP сервером
    configTime(zone * 3600, 0, "pool.ntp.org", "ru.pool.ntp.org");
    int i = 0;
    Serial.println("\nWaiting for time");
    while (!time(nullptr) && i < 10) {
      Serial.print(".");
      i++;
      delay(1000);
    }
    Serial.print("ITime Ready! ");
    Serial.print(GetTime());
    Serial.print(GetDate());
  }
}


// Установка параметров времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
void handle_time_zone() {
  timezone = HTTP.arg("timezone").toInt();  // Получаем значение timezone из запроса конвертируем в int сохраняем в глобальной переменной
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}


void handle_Time() {
  timeSynch(timezone);
  HTTP.send(200, "text/plain", "OK");  // отправляем ответ о выполнении
}
