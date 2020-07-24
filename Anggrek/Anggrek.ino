#include <Arduino.h> // Library arduino
#include <EEPROM.h>

// Library Wifi
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;
HTTPClient http;

// lcd
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// SCL ---------------> D1
// SDA ---------------> D2
// VCC ---------------> VIN
// GND ---------------> GND

// Sensor LDR
#define pinLdr A0
int cahaya;

// Sensor DHT
#include <DHT.h>

#define DHTPIN D0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int before_suhu = 25;
int before_kelembapan = 70;

//sensor waterflow
const int waterFlowPin = D3; // variable for D2 pin
int addr = 0; //endereço eeprom
byte sensorInterrupt = 0; // 0 = digital pin 2

float calibrationFactor = 4.5;

volatile byte pulseCount;

float debit;
unsigned int flowMilliLitres;
unsigned long total;

unsigned long oldTime;

// Relay
#define pompa D4
#define relay_off HIGH
#define relay_on LOW

// Motor Stepper
#define pin1 D5
#define pin2 D6
#define pin3 D7
#define pin4 D8

int langkah_awal = 0;
int a = 0;
boolean atap = true, menyiram = false;

// Alamat pengiriman data
String simpan = "http://192.168.43.239/anggrek-cantik/Data/save?suhu=";
String ambil_data = "http://192.168.43.239/anggrek-cantik/Data/data_terakhir";

String respon, respon2;

boolean data_terakhir;

void setup() {
  Serial.begin(115200);
  USE_SERIAL.begin(115200);
  USE_SERIAL.setDebugOutput(false);
  
  for(uint8_t t = 3; t > 0; t--) {
      USE_SERIAL.printf("[SETUP] Tunggu %d...\n", t);
      USE_SERIAL.flush();
      delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Project", "12345678");

  for (int u = 1; u <= 5; u++)
  {
    if ((WiFiMulti.run() == WL_CONNECTED))
    {
      USE_SERIAL.println("Wifi conected");
      USE_SERIAL.flush();
      delay(1000);
    }
    else
    {
      Serial.println("Wifi disconected");
      delay(1000);
    }
  }

  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pompa, OUTPUT);
  digitalWrite(pompa, relay_off);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("MONITORING");
  lcd.setCursor(2, 1);
  lcd.print("BUNGA ANGGREK");
  
  dht.begin();
  pinMode(waterFlowPin, INPUT);
    
  pulseCount = 0;
  debit = 0.0;
  flowMilliLitres = 0;
  total = 0;
  oldTime = 0;

  digitalWrite(waterFlowPin, HIGH);
  attachInterrupt(digitalPinToInterrupt(waterFlowPin), pulseCounter, RISING);

  delay(2000);
  lcd.clear();
  
}
void loop() {
  
  // baca sensor suhu

  cahaya = analogRead(pinLdr);

  cahaya = 100 - (cahaya / 10.24);

  baca_water_flow();

  int suhu = dht.readTemperature();
  int kelembapan = dht.readHumidity();
  if (suhu > 50)
  {
    suhu = before_suhu;  
  }
  else
  {
    before_suhu = suhu;
  }
  if (kelembapan < 0 || kelembapan > 100)
  {
    kelembapan = before_kelembapan;
  }
  else
  {
    before_kelembapan = kelembapan;
  }

  lcd.clear();
  delay(100);
  
  lcd.setCursor(0, 0);
  lcd.print("SUHU : ");
  lcd.setCursor(7, 0);
  lcd.print(suhu);
  lcd.setCursor(0, 1);
  lcd.print("KELEMBAPAN : ");
  lcd.setCursor(13, 1);
  lcd.print(kelembapan);
  delay(1500);
  
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("DEBIT : ");
  lcd.setCursor(8, 0);
  lcd.print(int(flowMilliLitres));
  lcd.setCursor(0, 1);
  lcd.print("TOTAL : ");
  lcd.setCursor(8, 1);
  lcd.print(total);

  delay(1500);

  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("CAHAYA : ");
  lcd.setCursor(9, 0);
  lcd.print(cahaya);

  Serial.print("Suhu : ");
  Serial.println(suhu);
  delay(300); 
  Serial.print("Kelembapan : ");
  Serial.println(kelembapan);
  delay(300); 
  Serial.print("Intensitas Cahaya : ");
  Serial.println(cahaya);
  delay(300);
  Serial.print("Debit Air : ");
  Serial.print(int(flowMilliLitres));
  Serial.println(" mL/sec");
  delay(300); 
  Serial.print("Total Air : ");
  Serial.print(total);
  Serial.println(" mL");

  // ambil data terakhir
  if (data_terakhir == false)
  {
    ambil_data_terakhir();

    if (respon2 == "false")
    {
      atap = false;
    }
    else
    {
      atap = true;
    }

    data_terakhir = true;
  }

  // kirim ke database
  if ((WiFiMulti.run() == WL_CONNECTED))
  {
    http.begin( simpan + (String) suhu + "&kelembapan=" + (String) kelembapan + "&cahaya=" + (String) cahaya + "&debit=" + (String) flowMilliLitres + "&volume=" + (String) total );
    
    USE_SERIAL.print("[HTTP] Menyimpan data ke database ...\n");
    int httpCode = http.GET();

    if(httpCode > 0)
    {
      USE_SERIAL.printf("[HTTP] kode response GET : %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK)
      {
        respon = http.getString();
        USE_SERIAL.println("Respon : " + respon);
        
        delay(200);
      }
    }
    else
    {
      USE_SERIAL.printf("[HTTP] GET data gagal, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }

  if (cahaya >= 30)
  {
    if (atap == true)
    {
      while (a < 10000)
      {
        atap_nutup(false);
        delay(2);
        a++;
        Serial.println(a);
      }
      motor_berhenti();
      a = 0;
      langkah_awal = 0;
      atap = false;
    }
  }
  else
  {
    if (atap == false)
    {
      while (a < 10000)
      {
        atap_buka(false);
        delay(2);
        a++;
        Serial.println(a);
      }
      motor_berhenti();
      a = 0;
      langkah_awal = 0;
      atap = true;
    }
   
  }

  if (suhu > 32)
  {
    if (menyiram == false)
    {
      while (flowMilliLitres <= 500)
      {
        digitalWrite(pompa, relay_on);
        Serial.println("Pompa on");
        baca_water_flow();
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DEBIT : ");
        lcd.setCursor(8, 0);
        lcd.print(int(flowMilliLitres));
        lcd.setCursor(0, 1);
        lcd.print("TOTAL : ");
        lcd.setCursor(8, 1);
        lcd.print(total);
        
        delay(1000);
      }

      digitalWrite(pompa, relay_off);
      Serial.println("Pompa off");
      menyiram = true;
    }
    else
    {
      digitalWrite(pompa, relay_off);
      Serial.println("Pompa off");

      menyiram = true;
    }
  }
  else
  {
    digitalWrite(pompa, relay_off);
    Serial.println("Pompa off");

    menyiram = false;
  }

  delay(500);
}

void pulseCounter() {
    // Increment the pulse counter
    pulseCount++;
}

void baca_water_flow()
{
  detachInterrupt(sensorInterrupt);

  debit = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;

  oldTime = millis();

  flowMilliLitres = (debit / 60) * 1000;

  total += flowMilliLitres;

  unsigned int frac;

  // Reset the pulse counter so we can start incrementing again
  pulseCount = 0;

  // Enable the interrupt again now that we've finished sending output
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

void atap_buka(bool dir)
{
  if (dir)
  {
    switch(langkah_awal)
    {
      case 0:
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      break;
      
      case 1:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      break;

      case 2:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, HIGH);
      digitalWrite(pin4, LOW);
      break;

      case 3:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, HIGH);
      break;
    }
  }
  else
  {
    switch(langkah_awal)
    {
      case 0:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, HIGH);
      break;
      
      case 1:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, HIGH);
      digitalWrite(pin4, LOW);
      break;

      case 2:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      break;

      case 3:
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      break;
    }
  }

  langkah_awal++;
  if (langkah_awal > 3){
    langkah_awal = 0;
  }
}

void atap_nutup(bool dir)
{
  if (dir)
  {
    switch(langkah_awal)
    {
      case 0:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, HIGH);
      break;
      
      case 1:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, HIGH);
      digitalWrite(pin4, LOW);
      break;

      case 2:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      break;

      case 3:
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      break;
    }
  }
  else
  {
    switch(langkah_awal)
    {
      case 0:
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      break;
      
      case 1:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      break;

      case 2:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, HIGH);
      digitalWrite(pin4, LOW);
      break;

      case 3:
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, HIGH);
      break;
    }
  }

  langkah_awal++;
  if (langkah_awal > 3){
    langkah_awal = 0;
  }
}

void motor_berhenti()
{
   digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}

void ambil_data_terakhir()
{
  if ((WiFiMulti.run() == WL_CONNECTED))
  {
    USE_SERIAL.print("[HTTP] Memulai...\n");
    
    http.begin( ambil_data );
    
    USE_SERIAL.print("[HTTP] Ambil data terakhir ...\n");
    int httpCode = http.GET();

    if(httpCode > 0)
    {
      USE_SERIAL.printf("[HTTP] kode response GET : %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK)
      {
        respon2 = http.getString();
        USE_SERIAL.println("Respon : " + respon2);
        delay(200);
      }
    }
    else
    {
      USE_SERIAL.printf("[HTTP] GET data gagal, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}
