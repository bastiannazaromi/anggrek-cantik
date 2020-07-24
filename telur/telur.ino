#include <Arduino.h>

// Wifi
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
// Buat object Wifi
ESP8266WiFiMulti WiFiMulti;
// Buat object http
HTTPClient http;
#define USE_SERIAL Serial

String simpan = "http://192.168.43.239/telur-pintar/Data/save?suhu=";
String update_telur = "http://192.168.43.239/telur-pintar/Data/update?ket=";

String respon = "";
String lampu_1, lampu_2, lampu_3, lampu_4;

boolean awal = true;

// button
#define buttonAwal D2
#define buttonLanjut D3

int jum = 1;

// Sensor pir
#define pirSensor D4

// Relay lampu
#define relay_on LOW
#define relay_off HIGH

#define r_lampu_1 D5
#define r_lampu_2 D6
#define r_lampu_3 D7
#define r_lampu_4 D8

// Sensor DHT
#include <DHT.h>

#define DHTPIN D9
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int before_suhu = 30;

void setup() {
  
  Serial.begin(115200);   //Komunikasi baud rate

  USE_SERIAL.begin(115200);
  USE_SERIAL.setDebugOutput(false);

  for(uint8_t t = 4; t > 0; t--) {
      USE_SERIAL.printf("[SETUP] Tunggu %d...\n", t);
      USE_SERIAL.flush();
      delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Project", "12345678"); // Sesuaikan SSID dan password ini

  for (int u = 1; u <= 5; u++)
  {
    if ((WiFiMulti.run() == WL_CONNECTED))
    {
      USE_SERIAL.println("Alhamdulillah wifi konek");
      USE_SERIAL.flush();
      delay(1000);
    }
    else
    {
      Serial.println("Hmmm wifi belum konek");
      delay(1000);
    }
  }

  pinMode(buttonAwal, INPUT_PULLUP);
  pinMode(buttonLanjut, INPUT_PULLUP);
  pinMode(pirSensor, INPUT_PULLUP);
  pinMode(r_lampu_1, OUTPUT);
  pinMode(r_lampu_2, OUTPUT);
  pinMode(r_lampu_3, OUTPUT);
  pinMode(r_lampu_4, OUTPUT);
  
  digitalWrite(r_lampu_1, relay_off);
  digitalWrite(r_lampu_2, relay_off);
  digitalWrite(r_lampu_3, relay_off);
  digitalWrite(r_lampu_4, relay_off);

  dht.begin();

  delay(1000);
  Serial.println("Silahkan tekan tombol Awal / Melanjutkan");
  
}

void loop() {

  int suhu = dht.readTemperature();
  if (suhu > 50)
  {
    suhu = before_suhu;  
  }
  else
  {
    before_suhu = suhu;
  }

  if ( awal == true )
  {
    if ( digitalRead(buttonAwal) == LOW )
    {
      Serial.println("Penetasan mulai dari awal");
      
      if ((WiFiMulti.run() == WL_CONNECTED))
      {
        USE_SERIAL.print("[HTTP] Memulai...\n");
        
        http.begin( update_telur + (String) 2 );
        
        USE_SERIAL.print("[HTTP] Menyimpan ke database ...\n");
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

      awal = false;
    }
    else if ( digitalRead(buttonLanjut) == LOW )
    {
      Serial.println("Melanjutkan penetasan");

      awal = false;
    }
  }

  delay(1000);

  if (awal == false)
  {
    Serial.println();
    Serial.print("Suhu : ");
    Serial.println(suhu);

    if ((WiFiMulti.run() == WL_CONNECTED))
    {
      USE_SERIAL.print("[HTTP] Memulai...\n");
      
      http.begin( simpan + (String) suhu );
      
      USE_SERIAL.print("[HTTP] Menyimpan ke database ...\n");
      int httpCode = http.GET();
  
      if(httpCode > 0)
      {
        USE_SERIAL.printf("[HTTP] kode response GET : %d\n", httpCode);
  
        if (httpCode == HTTP_CODE_OK)
        {
          respon = http.getString();
          USE_SERIAL.println("Respon : " + respon);
          lampu_1 = respon.substring(0,1);
          lampu_2 = respon.substring(1,2);
          lampu_3 = respon.substring(2,3);
          lampu_4 = respon.substring(3,4);

          USE_SERIAL.println("Lampu 1 : " + lampu_1);
          USE_SERIAL.println("Lampu 2 : " + lampu_2);
          USE_SERIAL.println("Lampu 3 : " + lampu_3);
          USE_SERIAL.println("Lampu 4 : " + lampu_4);
          
          delay(200);
        }
      }
      else
      {
        USE_SERIAL.printf("[HTTP] GET data gagal, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    }

    Serial.println();

    if (lampu_1.toInt() == 1)
    {
      digitalWrite(r_lampu_1, relay_on);
    }
    else 
    {
      digitalWrite(r_lampu_1, relay_off);
    }
    if (lampu_2.toInt() == 1)
    {
      digitalWrite(r_lampu_2, relay_on);
    }
    else 
    {
      digitalWrite(r_lampu_2, relay_off);
    }
    if (lampu_3.toInt() == 1)
    {
      digitalWrite(r_lampu_3, relay_on);
    }
    else
    {
      digitalWrite(r_lampu_3, relay_off);
    }
    if (lampu_4.toInt() == 1)
    {
      digitalWrite(r_lampu_4, relay_on);
    }
    else
    {
      digitalWrite(r_lampu_4, relay_off);
    }

    if ( digitalRead(pirSensor) == LOW )
    {
      Serial.println("Telor sudah ada yang menetas");
      
      if ((WiFiMulti.run() == WL_CONNECTED))
      {
        USE_SERIAL.print("[HTTP] Memulai...\n");
        
        http.begin( update_telur + (String) 1 );
        
        USE_SERIAL.print("[HTTP] Menyimpan ke database ...\n");
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
    }

    Serial.println();
    
  }

  delay(1000);
  
}
