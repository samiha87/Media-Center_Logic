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
    void messageFromControl(QByteArray msg);

signals:
    void newMessage(QByteArray msg);

public slots:
    void statusChanged(QByteArray msg);

private:
      DisplayDevice *device;
};

#endif // DISPLAYLOGIC_H
