#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>


const char* ssid = "1F13";
const char* password = "1310dbntr";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Получение внешнего IP-адреса
  HTTPClient http;
  http.begin("http://checkip.dyndns.org/");
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    int start = payload.indexOf("Current IP Address: ");
    int end = payload.indexOf("</body>");
    String externalIp = payload.substring(start + 20, end);
    Serial.println("External IP: " + externalIp);
  }
  
  else {
    Serial.println("Error on HTTP request");
  }
  http.end();

  // Получение данных о погоде
  http.begin("http://api.weatherapi.com/v1/current.json?key=fa7498fdfcd44ac1984122232222512&q=auto:ip");
  httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    // Парсинг ответа сервера и извлечение нужных данных о погоде
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
       Serial.printf("Error parsing JSON: %s\n", error.c_str());
      return;
    }
    float temperature = doc["current"]["temp_c"];
    float windSpeed = doc["current"]["wind_kph"];
    float pressure = doc["current"]["pressure_mb"];
    // Вывод результатов на экран
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.print("Wind speed: ");
    Serial.print(windSpeed);
    Serial.println("m/s");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println("hPa");
  }
  else {
    Serial.println("Error on HTTP request");
  }
  http.end();

  delay(10000);
}