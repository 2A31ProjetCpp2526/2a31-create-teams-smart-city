#include "arduino.h"

// ============================
// CONSTRUCTEUR
// ============================
Arduino::Arduino(QObject *parent)
    : QObject(parent)
    , serial(new QSerialPort(this))
    , arduino_is_available(false)
    , temperature(0.0)
{
    // Connexion des signaux du port série
    connect(serial, &QSerialPort::readyRead, this, &Arduino::lireDonnees);
    connect(serial, &QSerialPort::errorOccurred, this, &Arduino::gererErreur);
}

// ============================
// DESTRUCTEUR
// ============================
Arduino::~Arduino()
{
    deconnecter();
}

// ============================
// CONNECTER (VERSION 1 - par nom de port)
// ============================
bool Arduino::connecter(const QString &portName)
{
    if(serial->isOpen()) {
        qDebug() << "⚠️ Port déjà ouvert, déconnexion...";
        serial->close();
    }

    arduino_port_name = portName;
    serial->setPortName(portName);
    configurerPort();

    if(serial->open(QIODevice::ReadWrite)) {
        arduino_is_available = true;
        qDebug() << "✅ Arduino connecté sur" << portName;
        return true;
    } else {
        arduino_is_available = false;
        QString erreur = serial->errorString();
        qDebug() << "❌ Erreur connexion:" << erreur;
        emit erreurConnexion(erreur);
        return false;
    }
}

// ============================
// CONNECTER (VERSION 2 - automatique)
// ============================
int Arduino::connect_arduino()
{
    // Parcourir tous les ports disponibles
    foreach(const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if(serial_port_info.hasVendorIdentifier() &&
            serial_port_info.hasProductIdentifier()) {

            if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {

                arduino_port_name = serial_port_info.portName();
                arduino_is_available = true;

                qDebug() << "✅ Arduino Uno détecté sur" << arduino_port_name;

                return connecter(arduino_port_name) ? 1 : 0;
            }
        }
    }

    qDebug() << "❌ Aucun Arduino Uno détecté";
    arduino_is_available = false;
    return 0;
}

// ============================
// CONFIGURER PORT
// ============================
void Arduino::configurerPort()
{
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

// ============================
// DÉCONNECTER
// ============================
void Arduino::deconnecter()
{
    if(serial->isOpen()) {
        serial->close();
        arduino_is_available = false;
        qDebug() << "🔌 Arduino déconnecté";
    }
}

int Arduino::close_arduino()
{
    deconnecter();
    return 1;
}

// ============================
// ÉTAT
// ============================
bool Arduino::estConnecte() const
{
    return serial->isOpen();
}

// ============================
// COMMUNICATION - ÉCRITURE
// ============================
void Arduino::write_to_arduino(QByteArray data)
{
    if(serial->isOpen()) {
        serial->write(data);
        serial->flush();
        qDebug() << "📤 Envoyé vers Arduino:" << data;
    } else {
        qDebug() << "❌ Port non ouvert, impossible d'envoyer";
    }
}

// ============================
// COMMUNICATION - LECTURE
// ============================
QByteArray Arduino::read_from_arduino()
{
    if(serial->isOpen() && serial->canReadLine()) {
        data = serial->readLine();
        return data;
    }
    return QByteArray();
}

// ============================
// LCD - ENVOYER HABITANT ET SCORE
// ============================
void Arduino::envoyer_habitant_score(int idHabitant, int score)
{
    if(!serial->isOpen()) {
        qDebug() << "❌ Arduino non connecté";
        return;
    }

    // Format: "ID:123,SCORE:85\n"
    QString message = QString("ID:%1,SCORE:%2\n")
                          .arg(idHabitant)
                          .arg(score);

    QByteArray data = message.toUtf8();
    serial->write(data);
    serial->flush();

    qDebug() << "📤 LCD:" << message.trimmed();
}

// ============================
// LCD - ENVOYER MESSAGE
// ============================
void Arduino::envoyerMessageLCD(const QString &message)
{
    if(!serial->isOpen()) {
        qDebug() << "❌ Arduino non connecté";
        return;
    }

    QString msg = "MSG:" + message + "\n";
    QByteArray data = msg.toUtf8();
    serial->write(data);
    serial->flush();

    qDebug() << "📤 Message LCD:" << message;
}

// ============================
// TEMPÉRATURE - OBTENIR
// ============================
double Arduino::obtenirTemperature() const
{
    return temperature;
}

// ============================
// TEMPÉRATURE - DEMANDER
// ============================
void Arduino::demanderTemperature()
{
    if(serial->isOpen()) {
        serial->write("GET_TEMP\n");
        serial->flush();
    }
}

// ============================
// LIRE DONNÉES (SLOT PRINCIPAL)
// ============================
void Arduino::lireDonnees()
{
    if(!serial->canReadLine()) {
        // Pas de ligne complète, ajouter au buffer
        bufferLecture += QString::fromUtf8(serial->readAll());
        return;
    }

    // Lire toutes les lignes disponibles
    while(serial->canReadLine()) {
        QString ligne = bufferLecture + QString::fromUtf8(serial->readLine()).trimmed();
        bufferLecture.clear();

        if(!ligne.isEmpty()) {
            qDebug() << "📩 Reçu:" << ligne;
            analyserLigne(ligne);

            // Émettre signal données brutes
            emit donneesRecues(ligne.toUtf8());
        }
    }
}

// ============================
// ANALYSER LIGNE
// ============================
void Arduino::analyserLigne(const QString &ligne)
{
    // ⭐ TEMPÉRATURE (format: TEMP:22.5)
    if(ligne.startsWith("TEMP:")) {
        bool ok;
        double temp = ligne.mid(5).toDouble(&ok);

        if(ok) {
            temperature = temp;
            qDebug() << "🌡️ Température:" << temp << "°C";
            emit temperatureRecue(temp);
        } else {
            qDebug() << "⚠️ Erreur conversion température";
        }
    }

    // 🌀 VENTILATEUR (format: FAN:ON ou FAN:OFF)
    else if(ligne.startsWith("FAN:")) {
        QString etat = ligne.mid(4);
        qDebug() << "🌀 Ventilateur:" << etat;
    }

    // 📟 RÉPONSE LCD (format: LCD:OK)
    else if(ligne.startsWith("LCD:")) {
        QString reponse = ligne.mid(4);
        qDebug() << "📟 LCD:" << reponse;
    }

    // 📡 MESSAGES SYSTÈME
    else if(ligne.contains("Smart City") ||
             ligne.contains("initialise") ||
             ligne.contains("Demarrage")) {
        qDebug() << "📡 Arduino:" << ligne;
    }

    // ❓ AUTRE
    else {
        qDebug() << "❓ Message non reconnu:" << ligne;
    }
}

// ============================
// GÉRER ERREUR
// ============================
void Arduino::gererErreur(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::NoError) {
        return;
    }

    QString messageErreur;

    switch(error) {
    case QSerialPort::DeviceNotFoundError:
        messageErreur = "Périphérique non trouvé";
        break;
    case QSerialPort::PermissionError:
        messageErreur = "Permission refusée (port déjà utilisé?)";
        break;
    case QSerialPort::OpenError:
        messageErreur = "Impossible d'ouvrir le port";
        break;
    case QSerialPort::WriteError:
        messageErreur = "Erreur d'écriture";
        break;
    case QSerialPort::ReadError:
        messageErreur = "Erreur de lecture";
        break;
    case QSerialPort::ResourceError:
        messageErreur = "Ressource indisponible (Arduino débranché?)";
        arduino_is_available = false;
        break;
    case QSerialPort::TimeoutError:
        messageErreur = "Timeout";
        break;
    default:
        messageErreur = "Erreur inconnue";
    }

    qDebug() << "❌ Erreur port série:" << messageErreur;
    emit erreurConnexion(messageErreur);
}

// ============================
// ACCESSEURS
// ============================
QSerialPort* Arduino::getserial()
{
    return serial;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

// ============================
// MÉTHODES STATIQUES
// ============================
QStringList Arduino::listerPorts()
{
    QStringList ports;

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QString description = QString("%1 (%2)")
        .arg(info.portName())
            .arg(info.description());
        ports << description;
        qDebug() << "Port:" << description;
    }

    return ports;
}

QString Arduino::detecterPortArduino()
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if(info.hasVendorIdentifier() && info.hasProductIdentifier()) {
            if(info.vendorIdentifier() == arduino_uno_vendor_id &&
                info.productIdentifier() == arduino_uno_product_id) {
                qDebug() << "✅ Arduino détecté:" << info.portName();
                return info.portName();
            }
        }
    }

    qDebug() << "❌ Aucun Arduino détecté";
    return QString();
}
