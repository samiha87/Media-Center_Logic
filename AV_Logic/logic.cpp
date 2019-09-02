#include "logic.h"

#include "tcpsocket.h"

Logic::Logic(QObject *parent) : QObject(parent)
{
    pjlink = new PJLink(this);
    hwAdapter = new HardwareAdapter(this);

    pjlink->setPort(4352); // Default port for PJLink
    pjlink->setIpAddress("10.42.0.100");
    pjlink->setPassword("5233");
    // Transfer bluetooth messages coming from hardware layer to message parser and direct them to right modules
    QObject::connect(hwAdapter, SIGNAL(bleMessageRx(QByteArray)), this, SLOT(messageParser(QByteArray)));
    // Connect projector driver to this class and parse message.
    QObject::connect(pjlink, SIGNAL(projectorStatus(QByteArray)), this, SLOT(displayMessageParser(QByteArray)));
    // Connect outgoing dat
    QObject::connect(this, SIGNAL(hardwareTx(QByteArray)), hwAdapter, SLOT(hardwareTx(QByteArray)));
    // Connect to projector
    qDebug() << "Logic:: Starting()";
}

// Slots
void Logic::displayMessageParser(QByteArray msg) {
    qDebug() << "Logic::displayMessageParser " << msg;

}

void Logic::messageParser(QByteArray msg) {
    qDebug() << "Logic::messageParser() " << msg;
}
