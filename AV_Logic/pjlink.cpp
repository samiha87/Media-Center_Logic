#include "pjlink.h"

#include <qcryptographichash.h>
// https://pjlink.jbmia.or.jp/english/data/5-1_PJLink_eng_20131210.pdf

PJLink::PJLink(QObject *parent) : QObject(parent)
{
    port = PJLINK_PORT;
    sock = new TCPSocket();
    QObject::connect(sock, SIGNAL(response(QByteArray)), this, SLOT(response(QByteArray)));
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(requestStatus()));
    timer->start(1500);
    requestPoll = 0;
    requestedPowerState = false;
}

void PJLink::setPower(bool state) {
    if(state) {
        //send power on
        sendCommand("POWR 1");
        requestedPowerState = true;
    } else {
        sendCommand("POWR 0");
        // Send power off
        requestedPowerState = false;
    }
}

void PJLink::setVideoMute(bool state) {
    if(state) {
        // Set on
        sendCommand("AVMT 11");
    } else {
        // Set off
        sendCommand("AVMT 10");
    }
}

void PJLink::setAudioMute(bool state) {
    if(state) {
        // Set on
        sendCommand("AVMT 21");
    } else {
        // Set off
        sendCommand("AVMT 20");
    }
}

void PJLink::setAVMute(bool state) {
    if(state) {
        // Set on
        sendCommand("AVMT 31");
    } else {
        // Set off
        sendCommand("AVMT 30");
    }
}

void PJLink::setInput(Projector_Channels input) {
    switch (input) {
    case Channel_DIGITAL:
        sendCommand("INPT 31");
    break;
    case Channel_NETWORK:
        sendCommand("INPT 51");
    break;
    case Channel_RGB:
        sendCommand("INPT 11");
    break;
    case Channel_STORAGE:
        sendCommand("INPT 41");
    break;
    case Channel_VIDEO:
        sendCommand("INPT 22");
    break;
    }
}

void PJLink::requestStatus() {
    qDebug() << "PJLink requestStatus";
    switch (requestPoll) {
        case 0:
    case 1:
        requestStatusPwr();
        break;
    case 2:
        requestInput();
        break;
    case 3:
        requestAVMute();
        break;
    case 4:
        requestLamp();
        break;
    }
    requestPoll++;
    if(requestPoll > 4) {
        requestPoll = 0;
    }
}

void PJLink::requestStatusPwr() {
    sendCommand("POWR ?");
}

void PJLink::requestInput() {
    sendCommand("INPT ?");
}

void PJLink::requestAVMute() {
    // Request AV mute status
    sendCommand("AVMT ?");
}

void PJLink::requestLamp() {
    // Request lamp hours
    sendCommand("LAMP ?");
}

void PJLink::sendCommand(QString command) {
   //1. Connect and wait for response
    pendingCommand = "%1" + command;
    sock->connect(ipAddress, port);
}

void PJLink::setIpAddress(QString ip) {
    ipAddress = ip;
}

void PJLink::setPassword(QString pass) {
    password = pass;
}

void PJLink::setPort(quint16 port_) {
    port = port_;
}
void PJLink::response(QByteArray msg) {
    qDebug() << "PJLINK::response()" << msg;
    // Parse
    if(msg.contains("PJLINK 1")) {
        // Remove PJLINK 1 , 9 bits
        QByteArray random_number = msg.remove(0, 9);
        random_number.chop(1);
        qDebug() << "PJLink::response() Authentication number " << random_number;
        random_number.append(password.toUtf8());
        qDebug() << "PJLink::response() Random number is" << random_number;
        QByteArray hashedMessage = md5hash(random_number);
        hashedMessage.append(pendingCommand.toUtf8());
        hashedMessage.append('\r'); // \r is end byte, required for projector to understand when message ends
        qDebug() << "PJLink::response() send hashed message";
        sock->send(hashedMessage);
        pendingCommand.clear();
    } else if(msg.contains("ERR")) {
        sock->close();
    } else if (msg.contains("%1")) {
        // PJlink class 1
        msg = msg.remove(0, 2);
        if(msg.contains("POWR=")) {
            msg = msg.remove(0, 5);
            msg.chop(1);
            if(msg == "0") {
                qDebug("Proj power off");
                powerState = 0;
                if(requestedPowerState) sendCommand("POWR 1");  // Power should be true
                // start_byte, response, Projector, power, power state
                emit projectorStatus("Proj,Pwr,0");
            } else if (msg == "1") {
                qDebug("Proj power is on");
                powerState = 1;
                emit projectorStatus("Proj,Pwr,1");
                if(!requestedPowerState) sendCommand("POWR 0"); // Power should be false
            }
        }
        sock->close();
        if(msg.contains("AVMT=")) {
            msg = msg.remove(0, 5);
            msg.chop(1);
            projVolume = msg.toInt();
            QByteArray message;
            message.append("Proj,Vol,");
            if (projVolume < 0 || projVolume > 100) {
                qDebug() << "PJlink proj volume not correct " + QString::number(projVolume);
                sock->close();
                return;
            }
            message.append(QString::number(projVolume).toLocal8Bit());
            emit projectorStatus(message);
            sock->close();
        }

        if(msg.contains("INPT=")) {
            msg = msg.remove(0, 5);
            msg.chop(1);
            projInput = msg.toInt();
            QByteArray message;
            message.append("Proj,Inpt,");
            if (projInput < 0 || projInput > 100) {
                qDebug() << "PJlink proj input not correct " + QString::number(projInput);
                sock->close();
            return;
            }
            message.append(QString::number(projInput).toLocal8Bit());
            emit projectorStatus(message);
            sock->close();
        }

        if(msg.contains("LAMP=")) {
            msg = msg.remove(0, 5);
            msg.chop(3);
            projLamp = msg.toInt();
            QByteArray message;
            message.append("Proj,lamp,");
            if (projLamp < 0 || projLamp > 10000) {
                qDebug() << "PJlink proj lamp not correct " + QString::number(projLamp);

                sock->close();
                return;
            }
            message.append(QString::number(projLamp).toLocal8Bit());
            emit projectorStatus(message);
            sock->close();
            }
        }
    }

QByteArray PJLink::md5hash(QByteArray message) {
    return QCryptographicHash::hash(message,QCryptographicHash::Md5).toHex();
}
