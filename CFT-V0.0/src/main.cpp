#include <Arduino.h>
#include <Wire.h>
#include "Waktu.h"
#include "Sensor.h"
#include "Actuator.h"
#include "Display.h"

byte task;
// put function declarations here:
void waktuSekarang();
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  rtc.setTime(0, 52, 22, 9, 8, 2023);

  LCD.init();
  LCD.backlight();

  pinMode(buzzer, OUTPUT);
  pinMode(valveMasuk, OUTPUT);
  pinMode(valveKeluar, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  switch (task)
  {
  case 0:
    waktuSekarang();
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