#include <Keyboard.h>

void setup() {
  // Аппаратный UART для приема данных от ESP8266
  Serial1.begin(9600);
  // Инициализация USB HID клавиатуры
  Keyboard.begin();

  // Защита от мусора при загрузке: просто ждем 5 секунд перед стартом loop
  while (millis() < 5000) {
    if (Serial1.available() > 0) {
      Serial1.read(); // Очищаем буфер, если туда что-то прилетело
    }
  }
}

void loop() {
  // Если от ESP8266 пришли данные
  if (Serial1.available() > 0) {
    // Надежно читаем всю пришедшую строку целиком
    String command = Serial1.readString();
    
    // Удаляем случайные пробелы или символы перевода строки \r и \n
    command.trim(); 
    
    // Проверяем на специальные команды
    if (command == "[CTRL_ALT_DEL]") {
      // Отправляем Ctrl+Alt+Del
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_DELETE);
      delay(150);
      Keyboard.releaseAll();
    }
    else if (command == "[WIN_T]") {
      // Отправляем Win+T
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('t');
      delay(150);
      Keyboard.releaseAll();
    }
    else if (command.length() > 0) {
      // Отправляем обычный текст
      for (unsigned int i = 0; i < command.length(); i++) {
        Keyboard.write(command[i]);
        delay(5); // 5 мс — оптимально для предотвращения пропуска букв ПК
      }
    }
    
    // Пауза перед следующим циклом приема
    delay(10);
  }
}
