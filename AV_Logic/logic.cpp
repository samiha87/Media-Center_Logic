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

QByteArray Logic::makeMessage(QString input) {
    QByteArray result;
    if(input.contains("Proj")) {
        qDebug() << "Logic::makeMessage() Message from projector";

    }
    return result;
}
// Slots
void Logic::displayMessageParser(QByteArray msg) {
    qDebug() << "Logic::displayMessageParser " << msg;
    QByteArray builtMsg = makeMessage(msg); // Turn message into correct format for hardware layer
    emit hardwareTx(builtMsg);
}

void Logic::messageParser(QByteArray msg) {
    qDebug() << "Logic::messageParser() " << msg;
    if(msg.contains("Proj")) {
        msg = msg.remove(0,4);// Remove #Proj
        displayParser(msg);    // Process display message and act accordingly
    }
}

void Logic::displayParser(QByteArray msg) {
    // Check if we have power command
    qDebug() << "Logic::displayParser() " << msg;
    if(msg.contains("Pwr")) {
        if(msg.contains("On"))  {
            qDebug() << "Logic::displayParser() Truning projector on";
            pjlink->setPower(true);
        } else if(msg.contains("Off")) {
            qDebug() << "Logic::displayParser() Turn projector off";
            pjlink->setPower(false);
        }
    }
}
