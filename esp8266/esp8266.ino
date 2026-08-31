#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "web_page.h"

const char *ssid = "Soft_AP";
const char *password = "12345678";

DNSServer dnsServer;
IPAddress apIP(192, 168, 4, 1);

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleSend() {
  if (server.hasArg("plain")) {
    String message = server.arg("plain");
    
    // Обработка специальных команд без задержек на посимвольный ввод
    if (message == "[CTRL_ALT_DEL]") {
      // Отправляем маркер для Arduino
      Serial.print("[CTRL_ALT_DEL]");
    } 
    else if (message == "[WIN_T]") {
      // Отправляем маркер для Arduino
      Serial.print("[WIN_T]");
    }
    else {
      // Обычная отправка текста
      for (unsigned int i = 0; i < message.length(); i++) {
        Serial.print(message[i]);
        delay(5);
      }
    }
  }
  server.send(200, "text/plain", "OK");
}


void handleNotFound() {
  server.sendHeader("Location", "http://192.168.4.1", true);
  server.send(302, "text/plain", "");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  
  // DNS-сервер перенаправляет все домены (*) на IP платы
  dnsServer.start(53, "*", apIP);

  server.on("/", handleRoot);
  server.on("/send", HTTP_POST, handleSend);
  server.onNotFound(handleNotFound); 
  server.begin();
  
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  digitalWrite(LED_BUILTIN, LOW);
  delay(1);
}
