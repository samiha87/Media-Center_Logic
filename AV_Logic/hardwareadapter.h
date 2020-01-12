#ifndef HARDWAREADAPTER_H
#define HARDWAREADAPTER_H

#include <QObject>
#include <QProcess>

#include "tcpsocket.h"

class HardwareAdapter : public QObject
{
    Q_OBJECT
public:
    explicit HardwareAdapter(QObject *parent = nullptr);
    ~HardwareAdapter();

signals:
    void bleMessageRx(QByteArray msg);
    void tcpMessageRx(QByteArray msg);
public slots:
    // SLot is for receiveng mesages from hardware layer
    void msgFromHardware(QByteArray msg);
    // Messages ment to be forwarded for hardware layer
    void hardwareTx(QByteArray msg);

private:
    TCPSocket *tcp;
    QProcess *process;
    // Start python based hardware layer
    void startHardwareLayer();
    // Uses bash to kill runiing python
    void stopHardwareLayer();
};

#endif // HARDWAREADAPTER_H
