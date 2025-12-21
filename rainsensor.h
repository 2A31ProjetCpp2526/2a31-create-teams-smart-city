#ifndef RAINSENSOR_H
#define RAINSENSOR_H
#include <QDebug>
#include<arduino.h>
#include <QObject>
#include <QSerialPort>
#include <QTimer>

// Classe RainSensor pour interfacer avec un Arduino exécutant le code de détection de pluie
// Cette classe utilise QSerialPort pour se connecter au port série de l'Arduino
// et lit les messages imprimés par le sketch Arduino fourni.
// Elle émet un signal lorsque l'état de la pluie change.

class RainSensor : public QObject
{
    Q_OBJECT

public:
    explicit RainSensor(QObject *parent = nullptr);
    ~RainSensor();

    // Méthode pour se connecter au port série de l'Arduino
    bool connectToArduino(const QString &portName, int baudRate = 9600);

    // Méthode pour déconnecter
    void disconnectFromArduino();

    // Vérifier si connecté
    bool isConnected() const;

signals:
    // Signal émis lorsque l'état de la pluie change
    // true si pluie détectée, false sinon
    void rainStateChanged(bool rainDetected);
    // In RainSensor.h

    void rainDetected(bool rain);
    void rainVolumeUpdated(float liters);



    // When your sensor reads rain, emit the signal:
    //emit rainDetected(true);  // or false when no rain


private slots:
    // Slot appelé lorsque des données sont disponibles sur le port série
    void onReadyRead();

private:
    QSerialPort *serialPort;
    QTimer *timer;  // Timer pour gérer les lectures périodiques si nécessaire
    bool lastRainState;
    float totalLitres;
};

#endif // RAINSENSOR_H
