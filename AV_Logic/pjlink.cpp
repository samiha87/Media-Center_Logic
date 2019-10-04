#include "pjlink.h"

#include <qcryptographichash.h>
// Class 1 https://pjlink.jbmia.or.jp/english/data/5-1_PJLink_eng_20131210.pdf
// Class 2 https://pjlink.jbmia.or.jp/english/data_cl2/PJLink_5-1.pdf
PJLink::PJLink(QObject *parent) : QObject(parent)
{
    userName = "";
    port = PJLINK_PORT;
    sock = new TCPSocket(this);
    connect(sock, SIGNAL(response(QByteArray)), this, SLOT(response(QByteArray)));
    timer = new QTimer();
    connectionTimer = new QTimer(); // If no message has been received from project in time X. Mark it as disconnected.
    connect(timer, SIGNAL(timeout()), this, SLOT(requestStatus()));
    connect(connectionTimer, SIGNAL(timeout()), this, SLOT(checkConnectionStatus()));
    timer->start(2500);
    connectionTimer->start(10000);    // Check connection every 10s
    requestPoll = 0;
    requestedPowerState = false;
    connected = false;
}

void PJLink::setPower(bool state) {
    if(state) {
        //send power on
        sendCommand("POWR 1", 1);
        requestedPowerState = true;
    } else {
        sendCommand("POWR 0", 1);
        // Send power off
        requestedPowerState = false;
    }
}

void PJLink::setVideoMute(bool state) {
    if(state) {
        // Set on
        sendCommand("AVMT 11", 1);
    } else {
        // Set off
        sendCommand("AVMT 10", 1);
    }
}

void PJLink::setAudioMute(bool state) {
    if(state) {
        // Set on
        sendCommand("AVMT 21", 1);
    } else {
        // Set off
        sendCommand("AVMT 20", 1);
    }
}

void PJLink::setAVMute(bool state) {
    if(state) {
        // Set on
        sendCommand("AVMT 31", 1);
    } else {
        // Set off
        sendCommand("AVMT 30", 1);
    }
}

void PJLink::setInput(DisplayDevice::DisplayChannels input) {
    switch (input) {
    case Channel_DIGITAL:
        sendCommand("INPT 31", 1);
    break;
    case Channel_LAN:
        sendCommand("INPT 51", 1);
    break;
    case Channel_COMPUTER_1:
        sendCommand("INPT 11", 1);
    break;
    case Channel_STORAGE:
        sendCommand("INPT 41", 1);
    break;
    case Channel_VIDEO:
        sendCommand("INPT 22", 1);
    break;
    case Channel_HDMI_1:
        sendCommand("INPT 22", 1);
    break;
    }
}

void PJLink::checkConnectionStatus() {
    // Check if we have connected to projector
    // If not send response to Application that we have no connection
    if(!connected) {
        qDebug() << "PJLink::checkConnectionStatus() we have no connection";
        emit statusChanged("Proj,Conn,0");
    } else {
        emit statusChanged("Proj,Conn,1");
    }
    connected = false;
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
    case 5:
        requestVol();
        break;
    }
    requestPoll++;
    if(requestPoll > 4) {
        requestPoll = 0;
    }
}

void PJLink::requestStatusPwr() {
    sendCommand("POWR ?", 1);
}

void PJLink::requestInput() {
    sendCommand("INPT ?", 1);
}

void PJLink::requestAVMute() {
    // Request AV mute status
    sendCommand("AVMT ?", 1);
}

void PJLink::requestLamp() {
    // Request lamp hours
    sendCommand("LAMP ?", 1);
}

void PJLink::requestVol() {
    // Request lamp hours
    sendCommand("SVOL ?", 2);
}

void PJLink::sendCommand(QString command, int version) {
   //1. Connect and wait for response

    pendingCommand = "%" +QString::number(version) + command;
    sock->connect(ipAddress, port);
}

void PJLink::setAddress(QString ip) {
    ipAddress = ip;
}

QString PJLink::getIpAddress() {
    return ipAddress;
}
void PJLink::setUser(QString user) {
    userName = user;
}
void PJLink::setPassword(QString pass) {
    password = pass;
}

QString PJLink::getPassword() {
    return password;
}

void PJLink::setPort(quint16 port_) {
    port = port_;
}

quint16 PJLink::getPort() {
    return port;
}

void PJLink::setName(QString name) {
    deviceName = name;
}

QString PJLink::getName() {
    return deviceName;
}
void PJLink::response(QByteArray msg) {
    qDebug() << "PJLINK::response()" << msg;
    // Parse
    if(msg.contains("PJLINK 1")) {
        connected = true;
        authenticateVersion1(msg);
    } else if (msg.contains("PJLINK 2")) {
        connected = true;
        authenticateVersion2(msg);
    } else if(msg.contains("ERR")) {
        sock->close();
    } else if (msg.contains("%1")) {
        parseVersion1(msg);
    } else if(msg.contains("%2")) {
        parseVersion2(msg);
    }
}

void PJLink::loadPreset(DisplayPreset preset) {
    Q_UNUSED(preset)
}

void PJLink::authenticateVersion1(QByteArray msg) {
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
}

void PJLink::authenticateVersion2(QByteArray msg) {
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
}

QByteArray PJLink::md5hash(QByteArray message) {
    return QCryptographicHash::hash(message,QCryptographicHash::Md5).toHex();
}
// SVOL 0 to decrease, SVOL 1 to increase
void PJLink::volUp() {
     sendCommand("SVOL 1", 2);
}
void PJLink::volDown() {
    sendCommand("SVOL 0", 2);
}
void PJLink::toggleMute() {

    if(proVolumeMute) {
        sendCommand("AVMT 20", 1);
        proVolumeMute = false;
    } else {
        sendCommand("AVMT 21", 1);
        proVolumeMute = true;
    }
}

void PJLink::parseVersion1(QByteArray msg) {
    // PJlink class 1
    msg = msg.remove(0, 2);
    if(msg.contains("POWR=")) {
        msg = msg.remove(0, 5);
        msg.chop(1);
        if(msg == "0") {
            qDebug("Proj power off");
            powerState = 0;
            if(requestedPowerState) sendCommand("POWR 1", 1);  // Power should be true
            // start_byte, response, Projector, power, power state
            emit statusChanged("Proj,Pwr,0");
        } else if (msg == "1") {
            qDebug("Proj power is on");
            powerState = 1;
            emit statusChanged("Proj,Pwr,1");
            if(!requestedPowerState) sendCommand("POWR 0", 1); // Power should be false
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
        emit statusChanged(message);
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
        emit statusChanged(message);
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
        emit statusChanged(message);
        sock->close();
     }
}

void PJLink::parseVersion2(QByteArray msg) {
    qDebug() << "PJLink::parseVersion2() " << msg;
    if(msg.contains("SVOL=")) {
        msg = msg.remove(0, 5);
        msg.chop(3);
        projVolume = msg.toInt();
        QByteArray message;
        message.append("Proj,vol,");
        if (projVolume < 0 || projVolume > 10000) {
            qDebug() << "PJlink proj lamp not correct " + QString::number(projVolume);
            sock->close();
            return;
        }
        message.append(QString::number(projVolume).toLocal8Bit());
        emit statusChanged(message);
        sock->close();
     }
    sock->close();
}
