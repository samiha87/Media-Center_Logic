#ifndef AUDIOLOGIC_H
#define AUDIOLOGIC_H

#include <QObject>
#include "AudioDevice.h"
#include "displaydevice.h"
class AudioLogic : public QObject
{
    Q_OBJECT
public:
    explicit AudioLogic(QObject *parent = nullptr);
    void setDevice(AudioDevice *device);
    // Means display device is set as audio output
    void setDisplayDevice(DisplayDevice *device);
    void setDriver(QObject *object);
    QObject *getDriver();
    void messageFromControl(QByteArray msg);

signals:
    void newMessage(QByteArray msg);

public slots:
    void statusChanged(QByteArray msg);
    void displayStatusChanged(QByteArray msg);    // Convert display audio feedback to audio feedback
private:
    AudioDevice *device;
    DisplayDevice *display;
    QObject *driver;
};

#endif // AUDIOLOGIC_H
