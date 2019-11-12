// -- Bibliotecas --//
#include <Wire.h>
#include "DHT.h"
#include <Adafruit_BMP085.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

// -- Codigos dos sensores -- //

#define ANEMOMETRO 0
#define BAROMETRO 1
#define HIGROMETRO 2
#define PIRANOMETRO 3
#define PLUVIOMETRO 4
#define TERMOMETRO 5

char nomeSensor[6][16] = {
    "ANEMOMETRO",
    "BAROMETRO",
    "HIGROMETRO",
    "PIRANOMETRO",
    "PLUVIOMETRO",
    "TERMOMETRO"
  };

//-- portas analogicas -- //

// DEFINIR TODAS AS PORTAS AINDA!!
#define piranometro A0
//#define termometro (SDA1, SCL1)
#define anemometro A1
#define DHTPIN A2 
#define DHTTYPE DHT11 
#define barometro (SDA,SCL)
#define pluviometro (22)

LiquidCrystal lcd (12, 11, 5, 4, 3, 2);

// -- Variaveis -- //

typedef struct {
  int sensor;
  double valor;
} Sensor;

double valorAnemo;
double valorPiranometro;
double valorHigrometro;
double valorTermometro;
double valorBarometro;
double valorPluviometro;

volatile int contaPulso = 0;
volatile int contaPulso2 = 0;
double  quantidadeChuva = 0.414;
volatile unsigned long ultContada = 0;
volatile unsigned long agora;
volatile long tmpUltContada;
unsigned long inicioTempo = 0;


DHT dht(DHTPIN, DHTTYPE, 30);
Adafruit_BMP085 bmp;
int i = 0;
Sensor leitura[10];

char key = '#';
char keyAnt = key;

int msgMostra = 1, tmpMostra = 0;


const byte ROWS = 4; 
const byte COLS = 3; 

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {36, 37, 38, 39}; 
byte colPins[COLS] = {40, 41, 42}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  dht.begin();
  pinMode(pluviometro, INPUT);
  attachInterrupt(pluviometro, incpulso, RISING); //Configura o pino de sinal para trabalhar como interrupção
  bmp.begin();
 if(!bmp.begin()){
  lcd.println("Sensor não encontrado");
  while(1) {}
  }
}

void loop() {

lerAnemometro();
lerBarometro();
lerHigrometro();
lerPiranometro();
lerPluviometro();
lerTermometro();
 menu();

  if (i <= 10) { 
    i++;
 
  }
  else {
    i = 0;
  }

}

void menu() {
  key = keypad.getKey();
  if (key == NULL) { 
    key = keyAnt;
  } else {
    keyAnt=key;
  }
  switch (key) {

    case '1' :
     mostrar(ANEMOMETRO, valorAnemo);
    
      break;

    case '2' :
      mostrar(BAROMETRO, valorBarometro);

      break;

    case '3' :
      mostrar(HIGROMETRO, valorHigrometro);
    
      break;

    case '4' :
      mostrar(PIRANOMETRO, valorPiranometro);

      break;

    case '5' :
      mostrar(PLUVIOMETRO, valorPluviometro);
 
      break;

    case '6' :
    mostrar(TERMOMETRO, valorTermometro);

      break;

    case '#' :
      int tmpUltMsg = millis() - tmpMostra;

      if (tmpUltMsg > 2000) {
        msgMostra++;
        tmpMostra = millis();
        if (msgMostra == 5) msgMostra = 1;
      }
      if (msgMostra == 1) {
        lcd.setCursor(0,0);
        lcd.println("   -- MENU --   ");
        lcd.setCursor(0,1);
        lcd.println("Escolha a opcao:");
      }
      if (msgMostra == 2) {
        lcd.setCursor(0,0);
        lcd.println("1 - Anemometro  ");
        lcd.setCursor(0,1);
        lcd.println("2 - Barometro   ");
      }
      if (msgMostra == 3) {
        lcd.setCursor(0,0);        
        lcd.println("3 - Higrometro  ");
        lcd.setCursor(0,1);
        lcd.println("4 - Piranometro ");
      }
      if (msgMostra == 4) {
        lcd.setCursor(0,0);
        lcd.println("5 - Pluviometro ");
        lcd.setCursor(0,1);
        lcd.println("6 - Termometro  ");
      }


      break;

  }
}

void gravar(int codSensor, double valor) {
  leitura[i].sensor = codSensor;
  leitura[i].valor = valor;

}

void mostrar(int codSensor, double valor) {
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print(nomeSensor[codSensor]);
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("Valor: ");
  lcd.print(valor);
}


void lerAnemometro() {

  valorAnemo = analogRead(anemometro) - 79; // -79 pq o sensor começa de 0.4V e o arduino trabalha de 0V a 5V
  if (valorAnemo < 1023) {
    valorAnemo = (valorAnemo * 32.4) / 1023;
  }
  else {
    valorAnemo = 32.4;
  }

  gravar(ANEMOMETRO, valorAnemo);

}

void lerPiranometro() {
  delay(500);
  valorPiranometro = analogRead(piranometro) * 3.924; // constante calibrada com sensor de luminosidade profissional
  
  gravar(PIRANOMETRO, valorPiranometro);

}

void lerHigrometro() {

  valorHigrometro = dht.readHumidity();

  gravar(HIGROMETRO, valorHigrometro);

}

void lerTermometro() {

  valorTermometro = dht.readTemperature();

  gravar(TERMOMETRO, valorTermometro);

}

void lerBarometro() {

  valorBarometro = bmp.readPressure()/100;

  gravar(BAROMETRO, valorBarometro);

}

void lerPluviometro() {

  if (contaPulso == 1)inicioTempo = micros();
  if (contaPulso != contaPulso2) {
    contaPulso2 = contaPulso;
    valorPluviometro = ((contaPulso * quantidadeChuva) * 0.06); // 0.06 eh a constante de conversão para um recepiente de 1m X 1m
  }
  gravar(PLUVIOMETRO, valorPluviometro);

}

void incpulso ()
{

  agora = micros();
  tmpUltContada = abs(agora - ultContada);

  if (tmpUltContada > 200000L) {
    contaPulso++; //Incrementa a variável de contagem dos pulsos
    ultContada = agora;
  }
}

  
