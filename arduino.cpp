#include "arduino.h"
#include <QDebug>

Arduino::Arduino()
{
    arduino_is_available = false;
    portname = "COM6";
    serial = new QSerialPort;
}

int Arduino::connect_arduino()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if (info.hasVendorIdentifier() && info.hasProductIdentifier())
        {
            if (info.vendorIdentifier() == arduino_uno_vendor_id &&
                info.productIdentifier() == arduino_uno_product_id)
            {
                portname = info.portName();
                arduino_is_available = true;
            }
        }
    }

    if (!arduino_is_available)
    {
        qDebug() << "Arduino not available!";
        return 1;  // error
    }

    serial->setPortName(portname);
    if (!serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "Failed to open port:" << portname;

        return 1;  // error opening
    }

    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    qDebug() << "Arduino connected on port:" << portname;


    return 0; // success
}

void Arduino::close_arduino()
{
    if (serial->isOpen()) serial->close();
}

QByteArray Arduino::read_from_arduino()
{
    if (serial->isReadable())
        return serial->readAll();

    return QByteArray();
}

void Arduino::write_to_arduino(const QByteArray &data)
{
    if (serial->isWritable())
        serial->write(data);
}

QSerialPort* Arduino::getserial()
{
    return serial;
}
//if (!rain.connectToArduino("COM3", 9600)) {


