#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP32Time.h>
#include <EEPROM.h>

#define addrWaktuPakanPagi 0
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
char key;
int menu = 0, jumlahPakanPagi, jumlahPakanSiang, jumlahPakanSore, levelKurasHarian, levelKurasMingguan;
String waktuPakanPagi, waktuPakanSiang, waktuPakanSore, waktuKurasHarian, waktuKurasMingguan, bacaWaktuPakanPagi;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
ESP32Time rtc(3600);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.backlight();
  lcd.begin();
  rtc.setTime(0, 52, 22, 9, 8, 2023);
}

void loop() {
  key = customKeypad.getKey();
  Serial.println(key);

  switch (menu) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print(rtc.getTime("%a, %H:%M:%S"));
      delay(1000);

      if (key == '*') {
        lcd.clear();
        menu = 1;
        delay(100);
      }
      if(key == 'A'){
        lcd.clear();
        menu = 100;
        delay(100);
      }
      break;

    case 1:
      opsiSetting();
      break;

    case 2:
      settingPakan();
      break;

    case 3:
      settingKuras();
      break;

    case 4:
      settingWaktuPakan();
      break;

    case 5:
      settingJumlahPakan();
      break;

    case 6:
      settingWaktuPakanPagi();
      break;
  }
}

void opsiSetting() {
  lcd.setCursor(0, 0);
  lcd.print("1. Seting Pakan");
  lcd.setCursor(0, 1);
  lcd.print("2. Seting Kuras");

  if (key == '1') {
    lcd.clear();
    menu = 2;
    delay(100);
  }
  if (key == '2') {
    lcd.clear();
    menu = 3;
    delay(100);
  }
  if (key == '#') {
    lcd.clear();
    menu = 0;
    delay(100);
  }
}

void settingPakan() {
  lcd.setCursor(0, 0);
  lcd.print("1. Waktu Pakan");
  lcd.setCursor(0, 1);
  lcd.print("2. Jumlah Pakan");

  if (key == '1') {
    lcd.clear();
    menu = 4;
    delay(100);
  }
  if (key == '2') {
    lcd.clear();
    menu = 5;
    delay(100);
  }
  if (key == '#') {
    lcd.clear();
    menu = 1;
    delay(100);
  }
}

void settingKuras() {
  lcd.setCursor(0, 0);
  lcd.print("1. Waktu Kuras");
  lcd.setCursor(0, 1);
  lcd.print("2. Jumlah Kuras");

  if (key == '1') {
    lcd.clear();
    menu = 6;
    delay(100);
  }
  if (key == '2') {
    lcd.clear();
    menu = 7;
    delay(100);
  }
  if (key == '#') {
    lcd.clear();
    menu = 1;
    delay(100);
  }
}

void settingWaktuPakan() {
  lcd.setCursor(0, 0);
  lcd.print("1.Pagi");
  lcd.setCursor(8, 0);
  lcd.print("3.Sore");
  lcd.setCursor(0, 1);
  lcd.print("2.Siang");
  lcd.setCursor(8, 1);
  lcd.print("4.Malam");

  if (key == '1') {
    lcd.clear();
    menu = 6;
    delay(100);
  }
  if (key == '#') {
    lcd.clear();
    menu = 2;
    delay(100);
  }
}

void settingJumlahPakan() {
  lcd.setCursor(0, 0);
  lcd.print("1.Pagi");
  lcd.setCursor(8, 0);
  lcd.print("3.Sore");
  lcd.setCursor(0, 1);
  lcd.print("2.Siang");
  lcd.setCursor(8, 1);
  lcd.print("4.Malam");
}

void settingWaktuPakanPagi() {
  lcd.setCursor(0, 0);
  lcd.print("HH:MM:SS | Pagi");
  lcd.setCursor(0, 1);
  lcd.print(waktuPakanPagi);

  if (key != '*' && key != '#' && key != 'C' && key) {
    lcd.setCursor(0, 1);
    waktuPakanPagi += key;
  }

  if (key == '#') {
    waktuPakanPagi += ':';
  }

  if (key == '*') {
    eepromPut(addrWaktuPakanPagi, waktuPakanPagi, 4);
  }

  if (key == 'C') {
    waktuPakanPagi = "";
    eepromClear(addrWaktuPakanPagi, waktuPakanPagi, 4);
  }
}

void eepromPut(int addr, String data, int menuTujuan) {
  lcd.clear();
  lcd.print("saving time..");
  EEPROM.writeString(addr, data);
  delay(500);
  Serial.println(EEPROM.read(addr));
  lcd.clear();
  menu = menuTujuan;
}

void eepromClear(int addr, String data, int menuTujuan) {
  lcd.clear();
  lcd.print("clear..");
  EEPROM.put(addr, data);
  delay(500);
  lcd.clear();
  menu = menuTujuan;
}
