#ifndef AUDIOLOGIC_H
#define AUDIOLOGIC_H

#include <QObject>
#include "AudioDevice.h"

class AudioLogic : public QObject
{
    Q_OBJECT
public:
    explicit AudioLogic(QObject *parent = nullptr);
    void setDevice(AudioDevice *device);
    void messageFromControl(QByteArray msg);

signals:
    void newMessage(QByteArray msg);

public slots:
    void statusChanged(QByteArray msg);
private:
    AudioDevice *device;
};

#endif // AUDIOLOGIC_H
