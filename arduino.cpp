#include "Arduino.h"
#include <QDebug>
#include <QThread>  // pour QThread::msleep()

Arduino::Arduino()
{
    serial = new QSerialPort();
    arduino_is_available = false;
    arduino_port_name = "COM13";
}

int Arduino::connect_arduino()
{
    // 🔍 Option 1 : Détection automatique (recommandée)
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                qDebug() << "✅ Arduino détecté automatiquement sur :" << arduino_port_name;
                break;
            }
        }
    }

    // 🔧 Option 2 : Forcer manuellement un port (décommente pour tester)
    // arduino_port_name = "COM13";  // ← à adapter selon ton OS
    // arduino_is_available = true;
    // qDebug() << "🔧 Port forcé manuellement :" << arduino_port_name;

    // ⚠️ Si aucun Arduino détecté (et pas forcé), on tente quand même COM13
    if (!arduino_is_available) {
        // Essaye COM13 par défaut (utile pour les clones CH340)
        arduino_port_name = "COM13";
        arduino_is_available = true;
        qDebug() << "⚠️ Aucun Arduino détecté par VID/PID — tentative sur :" << arduino_port_name;
    }

    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);

            // ⏱️ Laisse le temps à l'Arduino de redémarrer après l'ouverture du port
            QThread::msleep(2000);

            qDebug() << "🔌 Port série ouvert avec succès sur :" << arduino_port_name;
            return 0;
        } else {
            qDebug() << "❌ Échec d'ouverture du port" << arduino_port_name
                     << "Erreur :" << serial->errorString();
        }
    }

    return 1;
}

int Arduino::close_arduino()
{
    if (serial && serial->isOpen()) {
        serial->close();
        arduino_is_available = false;
        qDebug() << "🔌 Port série fermé";
        return 0;
    }
    return 1; // déjà fermé
}

void Arduino::write_to_arduino(QByteArray data)
{
    if (serial && serial->isOpen()) {
        serial->write(data);
        serial->flush();
        qDebug() << "📤 Envoyé vers Arduino :" << data.trimmed();
    } else {
        qDebug() << "❌ Port série fermé — impossible d'envoyer :";
        qDebug() << "   Données ignorées :" << data;
    }
}

QByteArray Arduino::read_from_arduino()
{
    if (serial && serial->isOpen()) {
        QByteArray data = serial->readAll();
        if (!data.isEmpty()) {
            qDebug() << "📩 Reçu depuis Arduino :" << data.trimmed();
        }
        return data;
    }
    return QByteArray();
}

QSerialPort* Arduino::getserial()
{
    return serial;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

void Arduino::envoyer_habitant_score(int id, int score)
{
    if (!serial || !serial->isOpen()) {
        qDebug() << "❌ Impossible d'envoyer : port série fermé";
        return;
    }

    // Format compatible avec ton Arduino : "123:85\n"
    QString message = QString("%1:%2").arg(id).arg(score) + "\n";
    write_to_arduino(message.toUtf8());

    // Optionnel : attendre une réponse
    QThread::msleep(100);
    read_from_arduino(); // pour vider le buffer si réponse
}
