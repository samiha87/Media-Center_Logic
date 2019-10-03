#include "logic.h"

#include "tcpsocket.h"

Logic::Logic(QObject *parent) : QObject(parent)
{
    // Create display
    devPool.createDevice("Hitachi", devPool.Ethernet, devPool.eProjector);
    // Create audio device
    devPool.createDevice("Sony", devPool.InfraRed, devPool.eAmplifier);
    // Create lights device
   // devPool.createDevice("Dummy", devPool.Ethernet, devPool.eLights);

    hwAdapter = new HardwareAdapter(this);
    volHandler = new VolumeHandler(this);
    shutdownTimer = new QTimer(this);

    // Transfer bluetooth messages coming from hardware layer to message parser and direct them to right modules
    QObject::connect(hwAdapter, SIGNAL(bleMessageRx(QByteArray)), this, SLOT(messageParser(QByteArray)));
    // Connect projector driver to this class and parse message.
    QObject::connect(devPool.displayDevices[0], SIGNAL(newMessage(QByteArray)), this, SLOT(displayMessageParser(QByteArray)));
    // Connect outgoing data
    QObject::connect(this, SIGNAL(hardwareTx(QByteArray)), hwAdapter, SLOT(hardwareTx(QByteArray)));
    // Connect Volume Handler to hardware tx
    QObject::connect(volHandler, SIGNAL(volumeChanged(QByteArray)), hwAdapter, SLOT(hardwareTx(QByteArray)));
    qDebug() << "Logic:: Starting()";
}

QByteArray Logic::makeMessage(QByteArray input) {
    QByteArray result = "";
    if(input.contains("Proj")) {
        qDebug() << "Logic::makeMessage() Message from projector";
        result +="#r," + input + "*";
    }
    return result;
}
// Slots
void Logic::displayMessageParser(QByteArray msg) {
    qDebug() << "Logic::displayMessageParser " << msg;
    // Check power status
    if(msg.contains("Pwr=On")) {
        // Always when power is set on start a timer. After 3H shutdown projector
        shutdownTimer->start(10800000); // 3H * 60min * 60s * 1000mS = 10800000
    }
    if(msg.contains("Pwr=Off")) shutdownTimer->stop();
    QByteArray builtMsg = makeMessage(msg); // Turn message into correct format for hardware layer
    emit hardwareTx(builtMsg);
}

void Logic::messageParser(QByteArray msg) {
    qDebug() << "Logic::messageParser() " << msg;
    if(msg.contains("Proj")) {
        msg = msg.remove(0,4);// Remove #Proj
        for(auto &a: devPool.displayDevices) {
            a->messageFromControl(msg);
        }
    }

    if(msg.contains("Audio")) {
        qDebug() << "Logic::messageParser() Volume adjustment";
        msg = msg.remove(0, 3); // remove vol
        volumeParser(msg);
    }
}

void Logic::volumeParser(QByteArray msg) {
    if(msg.contains("Vol")) {
        if(msg.contains("Up"))  {
            qDebug() << "Logic::displayParser() Truning projector on";
            volHandler->setVolumeUp();
        } else if(msg.contains("Down")) {
            qDebug() << "Logic::displayParser() Turn projector off";
            volHandler->setVolumeDown();
        } else if(msg.contains("Mute")) {
            qDebug() << "Logic::displayParser() Toggle Mute";
            volHandler->toggleVolumeMute();
        }
    }
}

void Logic::systemAutoShutdown() {
    pjlink->setPower(false);
}
