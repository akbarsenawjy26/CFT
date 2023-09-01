#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Time.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Keypad.h>
#include <HCSR04.h>

int lcdColumns = 16, lcdRows = 2, pos = 0, task = 0, nilaiKurasHarian, nilaiKurasMingguan, nilaiPengisianPenuh, jumlahBukaan;
const byte oneWireBus = 15, buzzer = 2, valveMasuk = 5, valveKeluar = 4, trigger = 19, echo = 18, ROWS = 4, COLS = 4;
float suhuTimba1, suhuTimba2, suhuTimba3, suhuTimba4, suhuTimba5;
String waktuPakanPagi, waktuPakanSiang, waktuPakanSore, WaktuPakanMalam, waktuKurasHarian, waktuKurasMingguan;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {13, 12, 14, 27}, colPins[COLS] = {26, 25, 33, 32};
char key;
unsigned long waktuSekarang, waktuTerakhir, waktuJeda = 1000;

ESP32Time rtc(3600);
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
Servo myservo;
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
HCSR04 hc(trigger, echo);

DeviceAddress timba1 = { 0x28, 0xFF, 0x77, 0x62, 0x40, 0x17, 0x4, 0x31 };
DeviceAddress timba2 = { 0x28, 0xFF, 0xB4, 0x6, 0x33, 0x17, 0x3, 0x4B };
DeviceAddress timba3 = { 0x28, 0xFF, 0xA0, 0x11, 0x33, 0x17, 0x3, 0x96 };
DeviceAddress timba4 = { 0x28, 0xFF, 0xB4, 0x6, 0x33, 0x17, 0x3, 0x4B };
DeviceAddress timba5 = { 0x28, 0xFF, 0xA0, 0x11, 0x33, 0x17, 0x3, 0x96 };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  rtc.setTime(0, 52, 22, 9, 8, 2023);

  lcd.init();
  lcd.backlight();

  sensors.begin();

  myservo.attach(23);

  pinMode(buzzer, OUTPUT);
  pinMode(valveMasuk, OUTPUT);
  pinMode(valveKeluar, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors.requestTemperatures();
  key = customKeypad.getKey();
  suhuTimba1 = sensors.getTempC(timba1);
  suhuTimba2 = sensors.getTempC(timba2);
  suhuTimba3 = sensors.getTempC(timba3);
  suhuTimba4 = sensors.getTempC(timba4);
  suhuTimba5 = sensors.getTempC(timba5);
  waktuPakanPagi = "09:00:00";
  waktuPakanSiang = "15:00:00";
  waktuPakanSore = "21:00:00";
  WaktuPakanMalam = "03:00:00";
  waktuSekarang = millis();
  waktuKurasHarian = "08:00:00";
  waktuKurasMingguan = "Sun, 07:00:00";

  switch (task) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print(rtc.getTime("%a, %H:%M:%S"));
      delay(1000);

      if (rtc.getTime() == waktuKurasHarian) {
        lcd.clear();
        task = 1;
        delay(10);
      }

      if (rtc.getTime() == waktuKurasMingguan) {
        lcd.clear();
        task = 2;
        delay(10);
      }

      if(rtc.getTime() == waktuPakanPagi || rtc.getTime() == waktuPakanSiang || rtc.getTime() == WaktuPakanMalam){
        lcd.clear();
        task = 4;
        delay(10);
      }
      break;

    case 1:
      lcd.setCursor(0,0);
      lcd.print("Kuras Harian");
      lcd.setCursor(0,1);
      lcd.print("Tinggi Air:" + String(hc.dist()));
      
      digitalWrite(valveKeluar, HIGH);

      if(hc.dist() <= nilaiKurasHarian){
        digitalWrite(valveKeluar, LOW);
        lcd.clear();
        task = 3;
        delay(10);
      }
      break;

    case 2:
      lcd.setCursor(0,0);
      lcd.print("Kuras Mingguan");
      lcd.setCursor(0,1);
      lcd.print("Tinggi Air:" + String(hc.dist()));
      
      digitalWrite(valveKeluar, HIGH);

      if(hc.dist() <= nilaiKurasMingguan){
        digitalWrite(valveKeluar, LOW);
        lcd.clear();
        task = 3;
        delay(10);
      }
      break;

    case 3:
      lcd.setCursor(0,0);
      lcd.print("Isi Air");
      lcd.setCursor(0,1);
      lcd.print("Tinggi Air:" + String(hc.dist()));

      digitalWrite(valveMasuk, HIGH);

      if(hc.dist() >= nilaiPengisianPenuh){
        digitalWrite(valveMasuk, LOW);
        lcd.clear();
        task = 0;
        delay(10);
      }
      break;

    case 4:
      lcd.setCursor(0,0);
      lcd.print("Waktu Pakan!");
      for(int i = 0; i < jumlahBukaan; i++){
        myservo.write(45);
        delay(100);
        myservo.write(0);
        delay(100);
      }
      break;
  }
}
