#ifndef ARDUINO_H
#define ARDUINO_H

// ============================
// INCLUDES
// ============================
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QString>
#include <QDebug>

// ============================
// CLASSE ARDUINO COMPLÈTE
// ============================
class Arduino : public QObject
{
    Q_OBJECT

public:
    // ============================
    // CONSTRUCTEUR & DESTRUCTEUR
    // ============================
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    // ============================
    // MÉTHODES DE CONNEXION
    // ============================

    /**
     * @brief Connecter à un port Arduino spécifique
     * @param portName Nom du port (ex: "COM3", "/dev/ttyUSB0")
     * @return true si connexion réussie, false sinon
     */
    bool connecter(const QString &portName);

    /**
     * @brief Connecter automatiquement à un Arduino détecté
     * @return 1 si connexion réussie, 0 sinon
     */
    int connect_arduino();

    /**
     * @brief Déconnecter l'Arduino
     */
    void deconnecter();

    /**
     * @brief Fermer la connexion (alias de deconnecter)
     * @return 1 si fermeture réussie, 0 sinon
     */
    int close_arduino();

    // ============================
    // MÉTHODES D'ÉTAT
    // ============================

    /**
     * @brief Vérifier si l'Arduino est connecté
     * @return true si connecté, false sinon
     */
    bool estConnecte() const;

    /**
     * @brief Vérifier la disponibilité de l'Arduino
     * @return true si disponible
     */
    bool isAvailable() const { return arduino_is_available; }

    // ============================
    // MÉTHODES DE COMMUNICATION
    // ============================

    /**
     * @brief Envoyer des données brutes vers Arduino
     * @param data Données à envoyer
     */
    void write_to_arduino(QByteArray data);

    /**
     * @brief Lire des données brutes depuis Arduino
     * @return Données lues
     */
    QByteArray read_from_arduino();

    // ============================
    // MÉTHODES SPÉCIFIQUES LCD
    // ============================

    /**
     * @brief Envoyer ID habitant et score vers LCD Arduino
     * @param idHabitant ID de l'habitant
     * @param score Score de satisfaction
     */
    void envoyer_habitant_score(int idHabitant, int score);

    /**
     * @brief Envoyer un message texte vers LCD
     * @param message Message à afficher
     */
    void envoyerMessageLCD(const QString &message);

    // ============================
    // MÉTHODES TEMPÉRATURE
    // ============================

    /**
     * @brief Obtenir la dernière température mesurée
     * @return Température en degrés Celsius
     */
    double obtenirTemperature() const;

    /**
     * @brief Demander une nouvelle lecture de température
     */
    void demanderTemperature();

    // ============================
    // ACCESSEURS
    // ============================

    /**
     * @brief Obtenir le pointeur vers le port série
     * @return Pointeur QSerialPort
     */
    QSerialPort* getserial();

    /**
     * @brief Obtenir le nom du port Arduino
     * @return Nom du port (ex: "COM3")
     */
    QString getarduino_port_name();

    // ============================
    // MÉTHODES STATIQUES
    // ============================

    /**
     * @brief Lister tous les ports série disponibles
     * @return Liste des noms de ports
     */
    static QStringList listerPorts();

    /**
     * @brief Détecter automatiquement un port Arduino
     * @return Nom du port détecté ou chaîne vide
     */
    static QString detecterPortArduino();

signals:
    // ============================
    // SIGNAUX
    // ============================

    /**
     * @brief Signal émis lors de la réception d'une température
     * @param temperature Température mesurée en °C
     */
    void temperatureRecue(double temperature);

    /**
     * @brief Signal émis lors de la réception de données brutes
     * @param data Données reçues
     */
    void donneesRecues(QByteArray data);

    /**
     * @brief Signal émis en cas d'erreur
     * @param error Message d'erreur
     */
    void erreurConnexion(QString error);

private slots:
    // ============================
    // SLOTS PRIVÉS
    // ============================

    /**
     * @brief Lire les données disponibles sur le port série
     */
    void lireDonnees();

    /**
     * @brief Gérer les erreurs du port série
     * @param error Code d'erreur
     */
    void gererErreur(QSerialPort::SerialPortError error);

private:
    // ============================
    // ATTRIBUTS PRIVÉS
    // ============================

    // Port série
    QSerialPort *serial;                    // Objet principal de communication

    // Identification Arduino Uno
    static const quint16 arduino_uno_vendor_id = 9025;   // VID Arduino
    static const quint16 arduino_uno_product_id = 67;    // PID Arduino Uno

    // État
    bool arduino_is_available;              // Arduino disponible
    QString arduino_port_name;              // Nom du port (ex: "COM3")

    // Données
    QByteArray data;                        // Buffer de données reçues
    double temperature;                     // Dernière température mesurée

    // Buffer de lecture
    QString bufferLecture;                  // Buffer pour lignes incomplètes

    // ============================
    // MÉTHODES PRIVÉES
    // ============================

    /**
     * @brief Analyser une ligne de données reçue
     * @param ligne Ligne à analyser
     */
    void analyserLigne(const QString &ligne);

    /**
     * @brief Configurer les paramètres du port série
     */
    void configurerPort();
};

#endif // ARDUINO_H
