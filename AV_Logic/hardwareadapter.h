#ifndef HARDWAREADAPTER_H
#define HARDWAREADAPTER_H

#include <QObject>

class HardwareAdapter : public QObject
{
    Q_OBJECT
public:
    explicit HardwareAdapter(QObject *parent = nullptr);

signals:
    void bleMessageRx(QByteArray msg);
public slots:
    // SLot is for receiveng mesages from hardware layer
    void msgFromHardware(QByteArray msg);
};

#endif // HARDWAREADAPTER_H
