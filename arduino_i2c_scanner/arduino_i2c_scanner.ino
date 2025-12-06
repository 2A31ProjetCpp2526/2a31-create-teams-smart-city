/*
 * Scanner I2C pour trouver l'adresse du module LCD
 * Utilisez ce code pour trouver l'adresse I2C de votre LCD
 * avant d'utiliser arduino_lcd.ino
 */

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial); // Attendre que le port série soit prêt
  
  Serial.println("\nScanner I2C");
  Serial.println("Recherche des peripheriques I2C...\n");
}

void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Peripherique I2C trouve a l'adresse 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) {
      Serial.print("Erreur inconnue a l'adresse 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("Aucun peripherique I2C trouve\n");
  }
  else {
    Serial.println("Scan termine.\n");
  }
  delay(5000);           // Attendre 5 secondes avant de rescanner
}

