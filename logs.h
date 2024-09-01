// Получение текущего времени
String GetTime() {
  time_t now = time(nullptr);  // получаем время с помощью библиотеки time.h
  String Time = "";  // Строка для результатов времени
  Time += ctime(&now);  // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
  int i = Time.indexOf(":");  // Ишем позицию первого символа :
  Time = Time.substring(i - 2, i + 6);  // Выделяем из строки 2 символа перед символом : и 6 символов после
  Time.replace("\n", "");
  Time.replace("\r", "");
  return Time;  // Возврашаем полученное время
}


// Получение даты
String GetDate() {
  time_t now = time(nullptr);  // получаем время с помощью библиотеки time.h
  String Data = "";  // Строка для результатов времени
  Data += ctime(&now);  // Преобразуем время в строку формата Thu Jan 19 00:55:35 2017
  int i = Data.lastIndexOf(" ");  // Ишем позицию последнего символа пробел
  String Time = Data.substring(i - 8, i + 1);  // Выделяем время и пробел
  Data.replace(Time, "");  // Удаляем из строки 8 символов времени и пробел
  Data.replace("\n", "");
  Data.replace("\r", "");
  return Data;  // Возврашаем полученную дату
}


void write_log(String s) {
  File file = SPIFFS.open(log_file, "a");
  if (!file) {
    Serial.println("Failed to open config file for writing");
  }
  s = "[" + GetDate() + " " + GetTime() + "] " + s;
  file.print(s);
  file.print("\n");

  file.close();
}


String read_log() {
  File file = SPIFFS.open(log_file, "r");
  if (!file) {
    Serial.println("Failed to open config file for read");
  }
  String s = "";
  s = file.readString();
  while (file.available()) {
    s += file.read();
  }

  Serial.println(s);

  file.close();
  return s;
}


bool clear_log() {
  SPIFFS.remove(log_file);
  return true;
}
