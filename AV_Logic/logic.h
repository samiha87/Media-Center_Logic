#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>

#include "pjlink.h"
#include "hardwareadapter.h"

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
    QByteArray makeMessage(QString input);
    void displayParser(QByteArray msg);
};

#endif // LOGIC_H
