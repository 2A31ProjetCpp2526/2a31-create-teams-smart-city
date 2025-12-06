/*
 * Code Arduino pour afficher ID_HABITANT et SCORE sur LCD 2x16 via I2C
 * Connexions: LCD I2C sur A4 (SDA) et A5 (SCL)
 * 
 * Bibliothèque requise: LiquidCrystal_I2C
 * Installation: Outils > Gérer les bibliothèques > Rechercher "LiquidCrystal_I2C"
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialiser le LCD avec l'adresse I2C (généralement 0x27 ou 0x3F)
// Format: LiquidCrystal_I2C lcd(adresse, colonnes, lignes);
// Si 0x27 ne fonctionne pas, essayer 0x3F ou scanner avec un sketch I2C scanner
LiquidCrystal_I2C lcd(0x27, 16, 2); // Changez 0x27 en 0x3F si nécessaire

String buffer = "";
int idHabitant = 0;
int score = 0;
bool nouvelleDonnee = false;
unsigned long dernierAffichage = 0;

void setup() {
  // Initialiser la communication série à 9600 bauds
  Serial.begin(9600);
  while (!Serial) {
    ; // Attendre que le port série soit prêt
  }
  
  // Initialiser le bus I2C
  Wire.begin();
  
  // Initialiser le LCD
  lcd.init();
  lcd.backlight();
  
  // Afficher un message de démarrage
  lcd.setCursor(0, 0);
  lcd.print("Systeme pret");
  lcd.setCursor(0, 1);
  lcd.print("En attente...");
  
  Serial.println("LCD initialise - En attente de donnees...");
  Serial.println("Format attendu: ID_HABITANT:XXX:SCORE:YYY");
  
  delay(2000);
  lcd.clear();
  
  // Afficher un message par défaut
  lcd.setCursor(0, 0);
  lcd.print("Attente donnees");
  lcd.setCursor(0, 1);
  lcd.print("...");
}

void loop() {
  // Lire les données depuis le port série
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    if (c == '\n' || c == '\r') {
      // Fin de ligne reçue, traiter les données
      if (buffer.length() > 0) {
        Serial.print("Donnee recue: ");
        Serial.println(buffer);
        traiterDonnees();
        buffer = "";
      }
    } else if (c >= 32 && c <= 126) { // Caractères imprimables uniquement
      buffer += c;
    }
    
    // Limiter la taille du buffer pour éviter les débordements
    if (buffer.length() > 100) {
      buffer = "";
      Serial.println("Buffer trop long, reset");
    }
  }
  
  // Afficher les données si une nouvelle donnée a été reçue
  if (nouvelleDonnee) {
    afficherSurLCD();
    nouvelleDonnee = false;
    dernierAffichage = millis();
  }
  
  // Si aucune donnée depuis 30 secondes, afficher un message d'attente
  if (millis() - dernierAffichage > 30000 && dernierAffichage > 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Attente donnees");
    lcd.setCursor(0, 1);
    lcd.print("...");
    dernierAffichage = 0;
  }
  
  delay(10);
}

void traiterDonnees() {
  // Format attendu: "ID_HABITANT:123:SCORE:85"
  // ou format simplifié: "123:85"
  
  Serial.print("Traitement du buffer: ");
  Serial.println(buffer);
  
  int indexId = buffer.indexOf("ID_HABITANT:");
  int indexScore = buffer.indexOf("SCORE:");
  
  if (indexId != -1 && indexScore != -1) {
    // Format complet: "ID_HABITANT:123:SCORE:85"
    int debutId = indexId + 12; // Longueur de "ID_HABITANT:"
    int finId = buffer.indexOf(':', debutId);
    
    int debutScore = indexScore + 6; // Longueur de "SCORE:"
    
    if (finId != -1) {
      String strId = buffer.substring(debutId, finId);
      String strScore = buffer.substring(debutScore);
      
      idHabitant = strId.toInt();
      score = strScore.toInt();
      
      Serial.print("ID extrait: ");
      Serial.print(idHabitant);
      Serial.print(", Score extrait: ");
      Serial.println(score);
      
      if (idHabitant > 0 && score >= 0) {
        nouvelleDonnee = true;
      } else {
        Serial.println("Erreur: ID ou Score invalide");
      }
    } else {
      Serial.println("Erreur: Format incorrect (separateur manquant)");
    }
  } else {
    // Format simplifié: "123:85"
    int indexSeparateur = buffer.indexOf(':');
    if (indexSeparateur != -1) {
      String strId = buffer.substring(0, indexSeparateur);
      String strScore = buffer.substring(indexSeparateur + 1);
      
      idHabitant = strId.toInt();
      score = strScore.toInt();
      
      Serial.print("Format simplifie - ID: ");
      Serial.print(idHabitant);
      Serial.print(", Score: ");
      Serial.println(score);
      
      if (idHabitant > 0 && score >= 0) {
        nouvelleDonnee = true;
      } else {
        Serial.println("Erreur: ID ou Score invalide");
      }
    } else {
      Serial.println("Erreur: Format non reconnu");
    }
  }
}

void afficherSurLCD() {
  lcd.clear();
  
  // Ligne 1: "ID d'habitant X"
  lcd.setCursor(0, 0);
  String ligne1 = "ID habitant " + String(idHabitant);
  // Tronquer si trop long (16 caractères max)
  if (ligne1.length() > 16) {
    ligne1 = ligne1.substring(0, 16);
  }
  lcd.print(ligne1);
  
  // Ligne 2: "Score = XX"
  lcd.setCursor(0, 1);
  String ligne2 = "Score = " + String(score);
  // Tronquer si trop long (16 caractères max)
  if (ligne2.length() > 16) {
    ligne2 = ligne2.substring(0, 16);
  }
  lcd.print(ligne2);
  
  // Envoyer une confirmation sur le port série
  Serial.print("LCD mis a jour: ID=");
  Serial.print(idHabitant);
  Serial.print(", Score=");
  Serial.println(score);
  Serial.print("Ligne 1: ");
  Serial.println(ligne1);
  Serial.print("Ligne 2: ");
  Serial.println(ligne2);
}

