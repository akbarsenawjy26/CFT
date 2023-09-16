#include <Arduino.h>
#include <Wire.h>
#include "Waktu.h"
#include "Sensor.h"
#include "Actuator.h"
#include "Display.h"
#include "Networks.h"

byte task, hari = 0, i;

// put function declarations here:
void waktuSekarang();
void netwoksBot();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(valveMasuk, OUTPUT);
  pinMode(valveKeluar, OUTPUT);
  digitalWrite(valveMasuk, HIGH);
  digitalWrite(valveKeluar, HIGH);

  rtc.setTime(40, 59, 23, 9, 8, 2023);

  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  LCD.init();
  LCD.backlight();

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
    LCD.print("HST: " + String(hari) + " Hari");

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

  case 4:
    LCD.setCursor(0, 1);
    LCD.print("Tinggi Air:" + String(hc.dist() - offset));
    netwoksBot();
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

void netwoksBot()
{
  if (myBot.getNewMessage(msg))
  {
    if (msg.text.equalsIgnoreCase("/cekhari"))
    {
      myBot.sendMessage(msg.sender.id, "Total hari budidaya: " + String(hari));
    }

    if (msg.text.equalsIgnoreCase("/cekjamkuras"))
    {
      myBot.sendMessage(msg.sender.id, "Pengurasan Harian Pukul 08:00 WIB sebesar 10%");
      myBot.sendMessage(msg.sender.id, "Pengurasan Mingguan Pukul 07:00 WIB sebesar 80%");
    }

    if (msg.text.equalsIgnoreCase("/cekjampakan"))
    {
      myBot.sendMessage(msg.sender.id, "Pemberian pakan sehari 3x pada jam 09:00 WIB, 15:00 WIB, 21:00 WIB, dan opsional untuk jam 03:00 WIB");
    }

    if (msg.text.equalsIgnoreCase("/kurasharian"))
    {
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Perintah diterima!");
      delay(1000);
      LCD.clear();
      task = 1;
      delay(10);
    }

    if (msg.text.equalsIgnoreCase("/kurasmingguan"))
    {
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("Perintah diterima!");
      delay(1000);
      LCD.clear();
      task = 2;
      delay(10);
    }

    if (msg.text.equalsIgnoreCase("/pengukuran"))
    {
      myBot.sendMessage(msg.sender.id, "Masuk menu pengukuran");
      LCD.clear();
      task = 4;
      delay(10);
    }

    if (msg.text.equalsIgnoreCase("/home"))
    {
      myBot.sendMessage(msg.sender.id, "Kembali ke home");
      LCD.clear();
      task = 0;
      delay(10);
    }
  }
}