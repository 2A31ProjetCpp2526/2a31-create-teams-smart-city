#include "rainsensor.h"
#include <QDebug>
#include <QSerialPort>

RainSensor::RainSensor(QObject *parent)
    : QObject(parent),
    serialPort(new QSerialPort(this)),
    lastRainState(false),
    totalLitres(0.0f)
{
    connect(serialPort, &QSerialPort::readyRead,
            this, &RainSensor::onReadyRead);

    qDebug() << "[RainSensor] Initialisé.";
}

RainSensor::~RainSensor()
{
    disconnectFromArduino();
}

bool RainSensor::connectToArduino(const QString &portName, int baudRate)
{
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);

    if (!serialPort->open(QIODevice::ReadOnly)) {
        qDebug() << "[RainSensor] Erreur:" << serialPort->errorString();
        return false;
    }

    return true;
}

void RainSensor::disconnectFromArduino()
{
    if (serialPort->isOpen())
        serialPort->close();
}

void RainSensor::onReadyRead()
{
    while (serialPort->canReadLine()) {

        QString line = QString::fromUtf8(serialPort->readLine()).trimmed();
        qDebug() << "[RainSensor] Reçu:" << line;

        if (line.startsWith("RAIN:")) {
            bool rainDetected = (line.split(":")[1] == "1");

            if (rainDetected != lastRainState) {
                lastRainState = rainDetected;
                emit rainStateChanged(rainDetected);
            }
        }

        else if (line.startsWith("LITERS:")) {
            totalLitres = line.split(":")[1].toFloat();
            emit rainVolumeUpdated(totalLitres);
        }
    }
}
