#include <Arduino.h>
#include <Wire.h>
#include "Waktu.h"
#include "Sensor.h"
#include "Actuator.h"
#include "Display.h"
#include "Networks.h"

byte task;

// put function declarations here:
void waktuSekarang();
void netwoksBot();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  rtc.setTime(40, 59, 23, 9, 8, 2023);

  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  LCD.init();
  LCD.backlight();

  pinMode(buzzer, OUTPUT);
  pinMode(valveMasuk, OUTPUT);
  pinMode(valveKeluar, OUTPUT);
  int i = 0;
  for (i = 0; i < 5; i++)
  {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
  }

  if (myBot.testConnection())
  {
    Serial.println("\ntestConnection OK");
    myBot.sendMessage(id, "Ready!");
  }

  else
  {
    Serial.println("\ntestConnection NOK");
  }

  digitalWrite(valveMasuk, HIGH);
  digitalWrite(valveKeluar, HIGH);
}

void loop()
{
  // put your main code here, to run repeatedly:
  switch (task)
  {
  case 0:
    waktuSekarang();
    netwoksBot();
    LCD.setCursor(0, 1);
    LCD.print("HST: " + String(hari) + String(" Hari"));

    if (rtc.getTime() == "00:00:00")
    {
      hari += 1;
    }

    if (rtc.getTime() == waktuKurasHarian)
    {
      LCD.clear();
      task = 1;
      delay(10);
    }

    if (rtc.getTime() == waktuKurasMingguan)
    {
      LCD.clear();
      task = 2;
      delay(10);
    }
    break;

  case 1:
    myBot.sendMessage(id, "Waktunya Kuras Air Harian 10% dari Total Timba!");
    LCD.setCursor(0, 0);
    LCD.print("Kuras Harian");
    LCD.setCursor(0, 1);
    LCD.print("Tinggi Air:" + String(hc.dist()));

    digitalWrite(valveKeluar, LOW);

    if (hc.dist() <= nilaiKurasHarian)
    {
      digitalWrite(valveKeluar, HIGH);
      LCD.clear();
      task = 3;
      delay(10);
    }
    break;

  case 2:
    myBot.sendMessage(id, "Waktunya Kuras Air Mingguan 80% dari Total Timba!");
    LCD.setCursor(0, 0);
    LCD.print("Kuras Mingguan");
    LCD.setCursor(0, 1);
    LCD.print("Tinggi Air:" + String(hc.dist()));

    digitalWrite(valveKeluar, LOW);

    if (hc.dist() <= nilaiKurasMingguan)
    {
      digitalWrite(valveKeluar, HIGH);
      LCD.clear();
      task = 3;
      delay(10);
    }
    break;

  case 3:
    myBot.sendMessage(id, "Pengisian Air!");
    LCD.setCursor(0, 0);
    LCD.print("Isi Air");
    LCD.setCursor(0, 1);
    LCD.print("Tinggi Air:" + String(hc.dist()));

    digitalWrite(valveMasuk, LOW);

    if (hc.dist() >= nilaiPengisianPenuh)
    {
      digitalWrite(valveMasuk, HIGH);
      LCD.clear();
      task = 0;
      delay(10);
    }
    break;
  }
}

// put function definitions here
void waktuSekarang()
{
  LCD.setCursor(0, 0);
  LCD.print(rtc.getTime("%a, %H:%M:%S"));
  delay(1000);
}