#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte codigo[4]; //byte tiene 8 bits

// Array para almacenar los 32 bits es el codigo ARINC
bool bits[32];  //es un array que tiene 32 bits, el

unsigned long velocidad = 9600;
bool codigoValido = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Esperando datos");
  //defino estos 2 pines como las salidas
  pinMode(13,OUTPUT); // para los 1 (unos)
  pinMode(12,OUTPUT); // para los 0 (ceros)

}

void loop() {
//aca leistes los 4 bytes
  if (Serial.available() >= 4) {
    for (int i = 0; i < 4; i++) {
      codigo[i] = Serial.read();
    }
//despues que los leistes, se guardo el codigo arinc 429 en la varible bits
bytesToBits(codigo[0], codigo[1], codigo[2], codigo[3], bits);

//esto no lo haria asi, la velocidad debe ser enviada por otro lado
//para pensar....

//en esta parte se debe trnasmitir la in inormacion los 32 bits

// hay que hacer un ciclo de los 32 bits y un delay de 80 microsegundos
delayMicroseconds(80);
//
 mostrarEnLCD();

//recorrerlo con un ciclo
for(int i=0;i<33;i++){
  if(bits[0]==1)
    digitalWrite(13,HIGH);
  else
    digitalWrite(12,HIGH);
  delay(500);
}



//todo esto a revisar......
/*
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
*/

void mostrarEnLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Codigo:");
  for (int i = 0; i < 4; i++) {
    lcd.print(" ");
    if (codigo[i] < 16)
      lcd.print("0");
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
