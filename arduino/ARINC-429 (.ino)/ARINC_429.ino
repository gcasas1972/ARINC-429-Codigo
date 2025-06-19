#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t codigo[4];
unsigned long velocidad = 9600;
bool codigoValido = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Esperando datos");
}

void loop() {

  if (Serial.available() >= 8) {
    for (int i = 0; i < 4; i++) {
      codigo[i] = Serial.read();
    }

    velocidad = 0;
    for (int i = 0; i < 4; i++) {
      velocidad |= ((unsigned long)Serial.read()) << (8 * i);
    }

    uint16_t suma = codigo[0] + codigo[1] + codigo[2] + codigo[3];
    codigoValido = (suma % 256 == 0);

    mostrarEnLCD();

    if (codigoValido) {
      Serial1.end();
      Serial1.begin(velocidad);

      for (int i = 0; i < 4; i++) {
        Serial1.write(codigo[i]);
      }
    }
  }
}

void mostrarEnLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Codigo:");
  for (int i = 0; i < 4; i++) {
    lcd.print(" ");
    if (codigo[i] < 16) lcd.print("0");
    lcd.print(codigo[i], HEX);
  }

  lcd.setCursor(0, 1);
  if (codigoValido) {
    lcd.print("OK a ");
    lcd.print(velocidad);
  } else {
    lcd.print("Codigo invalido");
  }
}
