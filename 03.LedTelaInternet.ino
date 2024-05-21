/*  Comunicação entre dispositivos IoT
 *  Para receber dados, entre no site http://www.hivemq.com/demos/websocket-client/
 */

// Adicionar bibliotecas ao projeto
#include <WiFi.h>
#include <PubSubClient.h>
#include "heltec.h"

// Variaveis do servidor
WiFiClient espClient;
PubSubClient client(espClient);
const char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

// Configurações da WiFi e servidor
const char* redeWiFi = "donay";
const char* senhaWiFi = "donay123";
const char* identificador = "Placa01";

void setup()
{
  // Inicia o LED e a tela com uma mensagem
  pinMode(LED, OUTPUT);
  Heltec.begin(true, false, false, false, 915E6);
  Heltec.display->init();
  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(0, 0, "Tela e LED IoT");
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 18, String(identificador));
  Heltec.display->drawString(0, 32, "Conectando...");
  Heltec.display->display();

  // Conecta na Wi-Fi
  WiFi.begin(redeWiFi, senhaWiFi);                    // Inicializa conexão WiFi com a rede
  while (WiFi.status() != WL_CONNECTED) delay(1);     // Espera a WiFi conectar
  Heltec.display->drawString(0, 42 , "Rede OK");  // Avisa na tela que a WiFi conectou
  Heltec.display->display();

  // Conecta no servidor
  client.setServer(mqttServer, mqttPort); // Define o servidor
  client.setCallback(callback);           // Faz algo quando receber mensagens
  if (client.connect(identificador))      // Inicializa conexão com servidor
  {
    client.subscribe(identificador);                  // Fica esperando mensagens com o identificador
    Heltec.display->drawString(0, 52, "Servidor OK"); // Avisa na tela que conectou no servidor
    Heltec.display->display();
  }
}

// Recebe e mostra a mensagem
void callback(char* topic, byte* message, unsigned int length)
{
  String messageReceived;
  for (int i = 0; i < length; i++) messageReceived += (char)message[i];
  if(messageReceived == "On") digitalWrite(LED, HIGH);
  if(messageReceived == "Off") digitalWrite(LED, LOW);
  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(0, 0, messageReceived);
  Heltec.display->display();
}

// Mantem pronto para receber mensagens
void loop()
{
  client.loop();
}
