#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte codigo[4]; // Array para almacenar los 4 bytes (32 bits)
bool bits[32];  // Array para almacenar los 32 bits del código ARINC

unsigned long velocidad = 9600;
bool codigoValido = false;

// Declaración anticipada de funciones
void bytesToBits(byte b1, byte b2, byte b3, byte b4, bool bits[]);
void mostrarEnLCD();
void enviarBitARINC(bool bitVal);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Esperando datos");
  
  // Definir pines como salidas
  pinMode(13, OUTPUT); // para los 1 (unos)
  pinMode(12, OUTPUT); // para los 0 (ceros)
}

void loop() {
  // Leer los 4 bytes si están disponibles
  if (Serial.available() >= 4) {
    for (int i = 0; i < 4; i++) {
      codigo[i] = Serial.read();
    }
    
    // Convertir bytes a bits
    bytesToBits(codigo[0], codigo[1], codigo[2], codigo[3], bits);
    
    // Transmitir los 32 bits con el timing adecuado para ARINC-429
    for(int i = 0; i < 32; i++){
      enviarBitARINC(bits[i]);
      delayMicroseconds(80); // Retardo entre bits
    }
    
    // Mostrar información en LCD
    mostrarEnLCD();
    
    // Pequeña pausa antes de posible siguiente transmisión
    delay(100);
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

// Función para enviar un bit por el bus ARINC-429
void enviarBitARINC(bool bitVal) {
  if(bitVal == 1) {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
  } else {
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
  }
}

// Función para mostrar información en la LCD
void mostrarEnLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Codigo ARINC:");
  
  lcd.setCursor(0, 1);
  // Mostrar los primeros 8 bits (por limitación de espacio)
  for (int i = 0; i < 8; i++) {
    lcd.print(bits[i] ? "1" : "0");
  }
  lcd.print("...");
}