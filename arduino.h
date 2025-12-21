#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class Arduino : public QObject
{
    Q_OBJECT

public:
    Arduino();
    int connect_arduino();
    void close_arduino();
    QByteArray read_from_arduino();
    void write_to_arduino(const QByteArray &data);

    QSerialPort* getserial();

private:
    QSerialPort *serial;
    QString portname;
    bool arduino_is_available;

    static const quint16 arduino_uno_vendor_id = 0x2341;
    static const quint16 arduino_uno_product_id = 0x0043;
};

#endif // ARDUINO_H
