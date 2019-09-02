#include "logic.h"

#include "tcpsocket.h"

Logic::Logic(QObject *parent) : QObject(parent)
{
    pjlink = new PJLink(this);
    hwAdapter = new HardwareAdapter(this);

    pjlink->setPort(4352); // Default port for PJLink
    pjlink->setIpAddress("10.42.0.100");

    QObject::connect(hwAdapter, SIGNAL(bleMessageRx(QByteArray)), this, SLOT(messageParser(QByteArray)));
    QObject::connect(pjlink, SIGNAL(projectorStatus(QByteArray)), this, SLOT(displayMessageParser(QByteArray)));
    QObject::connect(this, SIGNAL(hardwareTx(QByteArray)), hwAdapter, SLOT(hardwareTx(QByteArray)));
}

// Slots
void Logic::displayMessageParser(QByteArray msg) {

}

void Logic::messageParser(QByteArray msg) {

}
