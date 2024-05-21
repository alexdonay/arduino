#include "heltec.h" // Adiciona biblioteca com comandos para comunicar com a tela

String texto = "TECH Factory - FSG";

void setup()
{
  Heltec.begin(true, false, false, false, 915E6);           // Configurações da tela
  Heltec.display->init();                                   // Inicializa a tela
  Heltec.display->clear();                                  // Limpa a tela
  Heltec.display->setFont(ArialMT_Plain_24);                // Define a fonte como Arial tamanho 24
  Heltec.display->drawString(3, 3, texto);                  // X e Y do display + texto a ser escrito
  Heltec.display->display();                                // Mostra o texto na tela
}

void loop()
{

}
