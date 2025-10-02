#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte codigo[4]; // Array para almacenar los 4 bytes (32 bits)
bool bits[32];  // Array para almacenar los 32 bits del código ARINC

void bytesToBits(byte b1, byte b2, byte b3, byte b4, bool bits[]);
void mostrarEnLCD();
void enviarBitARINC(bool bitVal);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Esperando datos");
  
  pinMode(13, OUTPUT); // LED para bit "1"
  pinMode(12, OUTPUT); // LED para bit "0"
}

void loop() {
  // Leer los 4 bytes si están disponibles
  if (Serial.available() >= 4) {
    for (int i = 0; i < 4; i++) {
      codigo[i] = Serial.read();
    }
    
    // Convertir bytes a bits
    bytesToBits(codigo[0], codigo[1], codigo[2], codigo[3], bits);
    
    // Transmitir los 32 bits con parpadeo visible
    for(int i = 0; i < 32; i++){
      enviarBitARINC(bits[i]);
    }
    
    // Mostrar los primeros 8 bits en el LCD
    mostrarEnLCD();
    
    delay(500); // Pausa antes de próxima trama
  }
}

// Función para convertir 4 bytes a 32 bits
void bytesToBits(byte b1, byte b2, byte b3, byte b4, bool bits[]) {
  for (int i = 0; i < 8; i++) {
    bits[i] = (b1 >> (7 - i)) & 1;
    bits[i + 8] = (b2 >> (7 - i)) & 1;
    bits[i + 16] = (b3 >> (7 - i)) & 1;
    bits[i + 24] = (b4 >> (7 - i)) & 1;
  }
}

// Función para enviar un bit con parpadeo visible
void enviarBitARINC(bool bitVal) {
  if(bitVal == 1) {
    digitalWrite(13, HIGH);   // LED "1"
    Serial.print("1");         // Monitor serie
    delay(250);              // Mantener encendido 1/4 segundo
    digitalWrite(13, LOW);    
  } else {
    digitalWrite(12, HIGH);   // LED "0"
    Serial.print("0");         // Monitor serie
    delay(250);              // Mantener encendido 1/4 segundo
    digitalWrite(12, LOW);
  }
  
  delay(100); // Apagado de 0, segundos entre bits
}

// Función para mostrar los primeros 8 bits en LCD
void mostrarEnLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Codigo ARINC:");
  
  lcd.setCursor(0, 1);
  for (int i = 0; i < 8; i++) {
    lcd.print(bits[i] ? "1" : "0");
  }
  lcd.print("...");
}
