// API de clima https://console.hgbrasil.com/documentation/weather/tools

#include <WiFi.h>
#include <HTTPClient.h>
#include "heltec.h"

String serverName = "https://api.hgbrasil.com/weather?woeid=429100";
String payload;
unsigned long lastTime = -100000;
unsigned long timerDelay = 6000;

// Configurações da WiFi
const char* ssid = "donay";
const char* password = "donay123";

void setup()
{
  Serial.begin(115200);
  Heltec.begin(true, false, false, false, 915E6);
  Heltec.display->init();
  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(0, 0, "Clima");
  Heltec.display->drawString(0, 20, "Caxias");
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 50, "Conectando...");
  Heltec.display->display();

  Serial.println("==================================================");
  Serial.print("Conectando WiFi - Rede: \"");
  Serial.print(ssid);
  Serial.print("\" - Senha: \"");
  Serial.print(password);
  Serial.println("\"");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado a rede com o IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Intervalo entre requisições (em ms): ");
  Serial.println(timerDelay);
  Serial.println("==================================================");
}

void loop()
{
  if (millis() - lastTime > timerDelay)
  {
    if(WiFi.status()== WL_CONNECTED)
    {
      HTTPClient http;      
      http.begin(serverName.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
        Serial.println(payload);
      }
      else
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else
    {
      Serial.println("!!! WiFi Desconectado !!!");
      WiFi.begin(ssid, password);
    }
    lastTime = millis();



    String temp;
    temp = payload.substring(payload.indexOf("time")+7,payload.length());
    temp = temp.substring(0,temp.indexOf("\""));
    String updateTime = temp;
    Serial.print("Tempo = ");
    Serial.println(updateTime);

    temp = payload.substring(payload.indexOf("temp")+6,payload.length());
    temp = temp.substring(0,temp.indexOf(","));
    int temperature = temp.toInt();
    Serial.print("Temperature = ");
    Serial.println(temperature);

    temp = payload.substring(payload.indexOf("humidity")+10,payload.length());
    temp = temp.substring(0,temp.indexOf(","));
    int humidity = temp.toInt();
    Serial.print("Umidade = ");
    Serial.println(humidity);

    temp = payload.substring(payload.indexOf("wind_speedy")+14,payload.length());
    temp = temp.substring(0,temp.indexOf("\""));
    temp.replace(" ","");
    String windSpeed = temp;
    Serial.print("Vento = ");
    Serial.println(windSpeed);

    temp = payload.substring(payload.indexOf("description")+14,payload.length());
    temp = temp.substring(0,temp.indexOf("\""));
    String description = temp;
    Serial.print("Descricao = ");
    Serial.println(description);

    String toPrint;
    Heltec.display->clear();
    toPrint += temperature;
    toPrint += "°C  ";
    toPrint += humidity;
    toPrint += "%";
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(0, 0, toPrint);
    Heltec.display->display();
    toPrint = "Vento ";
    toPrint += windSpeed;
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 26, toPrint);
    Heltec.display->display();
    toPrint = description;
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 45, toPrint);
    Heltec.display->display();
  }
}
