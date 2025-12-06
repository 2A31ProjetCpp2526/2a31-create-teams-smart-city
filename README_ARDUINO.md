# Configuration Arduino LCD pour Affichage Habitant/Score

## Description
Ce système permet d'afficher l'ID d'habitant et son score de satisfaction sur un écran LCD 2x16 connecté à une carte Arduino via I2C.

## Matériel requis
- Carte Arduino Uno (ou compatible)
- Module LCD 2x16 avec interface I2C
- Câbles de connexion
- Bibliothèque LiquidCrystal_I2C pour Arduino

## Connexions matérielles
- **SDA (LCD)** → **A4 (Arduino)**
- **SCL (LCD)** → **A5 (Arduino)**
- **VCC (LCD)** → **5V (Arduino)**
- **GND (LCD)** → **GND (Arduino)**

## Installation

### 1. Bibliothèque Arduino
1. Ouvrir l'IDE Arduino
2. Aller dans **Croquis > Inclure une bibliothèque > Gérer les bibliothèques**
3. Rechercher "LiquidCrystal_I2C"
4. Installer la bibliothèque de **Frank de Brabander** (version récente)

### 2. Code Arduino
1. Ouvrir le fichier `arduino_lcd.ino` dans l'IDE Arduino
2. Vérifier l'adresse I2C du LCD (généralement 0x27 ou 0x3F)
   - Si le LCD ne s'affiche pas, modifier la ligne :
     ```cpp
     LiquidCrystal_I2C lcd(0x27, 16, 2);
     ```
     en essayant `0x3F` à la place de `0x27`
3. Téléverser le code sur la carte Arduino

### 3. Configuration Qt
Le code Qt est déjà configuré pour :
- Détecter automatiquement la carte Arduino
- Se connecter au port série à 9600 bauds
- Envoyer les données au format : `ID_HABITANT:123:SCORE:85`

## Utilisation

### Dans l'application Qt
1. Lancer l'application
2. Aller dans la section "Satisfaction"
3. Entrer un ID d'habitant et cliquer sur "Afficher satisfaction"
4. Les données seront automatiquement envoyées à l'Arduino et affichées sur le LCD

### Format d'affichage LCD
- **Ligne 1** : "ID habitant X"
- **Ligne 2** : "Score = XX"

## Dépannage

### Le LCD ne s'affiche pas
1. Vérifier les connexions (SDA/SCL, VCC/GND)
2. Vérifier l'adresse I2C (utiliser un scanner I2C si nécessaire)
3. Vérifier que la bibliothèque LiquidCrystal_I2C est bien installée

### L'Arduino ne reçoit pas les données
1. Vérifier que le port série est correctement détecté
2. Ouvrir le moniteur série Arduino (9600 bauds) pour voir les messages de debug
3. Vérifier que la connexion série est ouverte dans l'application Qt

### Le score affiché est incorrect
1. Vérifier que l'habitant a bien des scores dans la table SATISFACTION
2. Le système calcule la moyenne des scores de satisfaction
3. Si aucun score n'existe, le système calcule un score basé sur les demandes résolues

## Format des données
Le code Arduino accepte deux formats :
- Format complet : `ID_HABITANT:123:SCORE:85`
- Format simplifié : `123:85`

## Notes techniques
- Vitesse de communication série : 9600 bauds
- Protocole I2C pour le LCD
- Le code Arduino gère automatiquement le rafraîchissement de l'affichage

