#ifndef DISPLAYLOGIC_H
#define DISPLAYLOGIC_H

#include <QObject>
#include "displaydevice.h"

class DisplayLogic : public QObject
{
    Q_OBJECT
public:
    explicit DisplayLogic(QObject *parent = nullptr);
    void setDisplay(DisplayDevice *display);
    void setDriver(QObject *object);
    QObject *getDriver();
    DisplayDevice *getDisplay();
    void messageFromControl(QByteArray msg);

signals:
    void newMessage(QByteArray msg);

public slots:
    void statusChanged(QByteArray msg);

private:
    DisplayDevice *device;
    QObject *driver;

    bool latestPowerRequest;
    int latestPresetRequest;
    DisplayDevice::DisplayChannels latestInputRequest;
};

#endif // DISPLAYLOGIC_H
