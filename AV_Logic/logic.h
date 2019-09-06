#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QTimer>

#include "pjlink.h"
#include "hardwareadapter.h"
#include "volumehandler.h"

class Logic : public QObject
{
    Q_OBJECT
public:
    explicit Logic(QObject *parent = nullptr);

signals:
    void hardwareTx(QByteArray msg);

public slots:
    void displayMessageParser(QByteArray msg);
    void messageParser(QByteArray msg);

private:
    HardwareAdapter *hwAdapter;
    PJLink *pjlink;
    VolumeHandler *volHandler;
    QByteArray makeMessage(QByteArray input);
    // Parses messages pointed for display. Contains display logic.
    void displayParser(QByteArray msg);
    // Parses messages pointed for volume. Contains volume logic.
    void volumeParser(QByteArray msg);
    // Auto shutdown. This is called when shutdownt timer reaches timeout
    // Shuts whole system down
    void systemAutoShutdown();
    // Timer is started when projector is detected open.
    // After x amount of time projector will automatically be shutdown
    QTimer *shutdownTimer;
};

#endif // LOGIC_H
