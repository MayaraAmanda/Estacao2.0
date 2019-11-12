#include <ESP8266WiFi.h>
#include <SimpleDHT.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>


const char* ssid     = "Residencia";
const char* password = "MaMa1220";

const char* host = "192.168.1.7"; //host que vou enviar os dados www.seusite.com.br

const int analogInPin = A0;
int pinDHT11 = D4;
int piranometro = 0;
SimpleDHT11 dht11;
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Conectando com: ");
  Serial.println(ssid);
  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");  
  Serial.println("Endereco de IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  //=====================================================
  //RESERVADO PARA A LEITURA DOS SENSORES
  byte temp = 0;
  byte humid = 0;

  if (dht11.read(pinDHT11, &temp, &humid, NULL)) {
    return;
  }
  piranometro = analogRead(analogInPin)*3.924;
  barometro = bmp.readPressure();
  Serial.print((int)temp); Serial.print(" *C, "); 
  Serial.print((int)humid); Serial.println(" %");
  Serial.println(piranometro);
  Serial.print(barometro);
  
  
  Serial.print("Conectando com: ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Falha na conexao");
    return;
  }
  
  // processo de montagem da url
  String url = "/estacao/salvar.php?";
         url += "barometro=";
         url += barometro;
         url += "&higrometro=";
         url += humid;
         url += "&piranometro=";
         url += piranometro;
         url += "&termometro=";
         url += temp;
          
  Serial.print("Requisitando URL: ");
  Serial.println(url);
  
  //efetua a solicitacao get
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Tempo esgotado!");
      client.stop();
      return;
    }
  }
  
  // Captura o retorno do servidor
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("Conexao fechada");
  delay(120000);
}

