#include "arduino.h"
#include <QDebug>

Arduino::Arduino()
{
    arduino_is_available = false;
    portname = "";
    serial = new QSerialPort;

    qDebug() << "[Arduino] Initialisation de la classe Arduino.";
}

int Arduino::connect_arduino()
{
    qDebug() << "[Arduino] Recherche des ports disponibles...";

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << " - Port trouvé:" << info.portName();

        // Vérification Vendor ID & Product ID
        if (info.hasVendorIdentifier() && info.hasProductIdentifier())
        {
            qDebug() << "   Vendor ID:" << info.vendorIdentifier()
            << "| Product ID:" << info.productIdentifier();

            if (info.vendorIdentifier() == arduino_uno_vendor_id &&
                info.productIdentifier() == arduino_uno_product_id)
            {
                portname = info.portName();
                arduino_is_available = true;

                qDebug() << "[Arduino] Arduino UNO détecté sur:" << portname;
            }
        }
        else
        {
            qDebug() << "   Aucun Vendor/Product ID → périphérique ignoré.";
        }
    }

    if (!arduino_is_available)
    {
        qDebug() << "[Arduino] ERREUR: Aucun Arduino UNO détecté.";
        return 1;  // erreur : non détecté
    }

    // Tentative de connexion
    serial->setPortName(portname);
    qDebug() << "[Arduino] Tentative d'ouverture du port:" << portname;

    if (!serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "[Arduino] ERREUR: Impossible d'ouvrir le port:"
                 << portname
                 << " | Raison:" << serial->errorString();

        return 2;  // erreur ouverture
    }

    // Paramétrage
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    qDebug() << "[Arduino] Connexion établie avec succès sur:" << portname;
    return 0; // succès
}

void Arduino::close_arduino()
{
    if (serial->isOpen())
    {
        serial->close();
        qDebug() << "[Arduino] Port série fermé.";
    }
    else
    {
        qDebug() << "[Arduino] Aucune connexion à fermer (port déjà fermé).";
    }
}

QByteArray Arduino::read_from_arduino()
{
    if (serial->isReadable())
    {
        QByteArray data = serial->readAll();
        qDebug() << "[Arduino] Données reçues:" << data;
        return data;
    }

    qDebug() << "[Arduino] Port non lisible → aucune donnée reçue.";
    return QByteArray();
}

void Arduino::write_to_arduino(const QByteArray &data)
{
    if (serial->isWritable())
    {
        serial->write(data);
        qDebug() << "[Arduino] Données envoyées:" << data;
    }
    else
    {
        qDebug() << "[Arduino] ERREUR: Impossible d'écrire sur le port.";
    }
}

QSerialPort* Arduino::getserial()
{
    return serial;
}
