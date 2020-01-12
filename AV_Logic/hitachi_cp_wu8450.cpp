#include "hitachi_cp_wu8450.h"
#include <QDebug>
HITACHI_CP_WU8450::HITACHI_CP_WU8450(QObject *parent) : QObject(parent)
{
    qDebug() << "HITACHI_CP_WU8450 Driver created";
    userName = "";
    port = HITACHI_CONTROL_PORT_1;
    sock = new TCPSocket(this);
    connect(sock, SIGNAL(response(QByteArray)), this, SLOT(response(QByteArray)));
    connect(sock, SIGNAL(connectedToHost()), this, SLOT(connectedToHost()));
    timer = new QTimer();
    connectionTimer = new QTimer(); // If no message has been received from project in time X. Mark it as disconnected.
    connect(timer, SIGNAL(timeout()), this, SLOT(requestStatus()));
    connect(connectionTimer, SIGNAL(timeout()), this, SLOT(checkConnectionStatus()));
    timer->start(2000);
    connectionTimer->start(10000);    // Check connection every 10s
    requestPoll = 0;
    requestedPowerState = false;
    connected = false;
    authenticationRequired = false;
    projVolumeMute = false;
    volIncreasePoll = 0;
    volDecreasePoll = 0;
    loadLensExecute = false;
}

void HITACHI_CP_WU8450::setPower(bool state) {

    if(state) sendCommand(HITACHI_SET_PWR_ON);
    else sendCommand(HITACHI_SET_PWR_OFF);
}

void HITACHI_CP_WU8450::setVideoMute(bool state) {
    if(state) sendCommand(HITACHI_SET_BLANK_ON);
    else sendCommand(HITACHI_SET_BLANK_OFF);
}

void HITACHI_CP_WU8450::setInput(DisplayChannels input) {
    switch (input) {
    case Channel_COMPUTER_1:
        sendCommand(HITACHI_SET_INPUT_COMPUTER1);
        break;
    case Channel_VIDEO:
        sendCommand(HITACHI_SET_INPUT_VIDEO);
        break;
    case Channel_DIGITAL:
        sendCommand(HITACHI_SET_INPUT_COMPONENT);
        break;
    case Channel_STORAGE:
        sendCommand(HITACHI_SET_INPUT_USB_TYPE_A);
        break;
    case Channel_HDMI_1:
        sendCommand(HITACHI_SET_INPUT_HDMI1);
        break;
    case Channel_LAN:
        sendCommand(HITACHI_SET_INPUT_LAN);
        break;
    }
}

void HITACHI_CP_WU8450::setAddress(QString ip) {
    ipAddress = ip;
}

void HITACHI_CP_WU8450::setUser(QString user) {
    userName = user;
}

void HITACHI_CP_WU8450::setPassword(QString pass) {
    password = pass;
}

void HITACHI_CP_WU8450::setPort(quint16 port_) {
    if(port_ == 0) return;
    port = port_;
}

void HITACHI_CP_WU8450::setName(QString name) {
    deviceName = name;
}

QString HITACHI_CP_WU8450::getName() {
    return deviceName;
}

void HITACHI_CP_WU8450::requestPwr() {
    sendCommand(HITACHI_REQUEST_PWR);
}

void HITACHI_CP_WU8450::requestInput() {
    sendCommand(HITACHI_REQUEST_INPUT);
}

void HITACHI_CP_WU8450::requestLampLow() {
    sendCommand(HITACHI_REQUEST_LAMP_STATUS_LOW);
}

void HITACHI_CP_WU8450::requestLampHigh() {
    sendCommand(HITACHI_REQUEST_LAMP_STATUS_LOW);
}

void HITACHI_CP_WU8450::requestFilterLow() {
    sendCommand(HITACHI_REQUEST_FILTER_STATUS_LOW);
}

void HITACHI_CP_WU8450::requestFilterHigh() {
    sendCommand(HITACHI_REQUEST_FILTER_STATUS_HIGH);
}

void HITACHI_CP_WU8450::requestVol() {
    sendCommand(HITACHI_REQUEST_VOLUME);
}

void HITACHI_CP_WU8450::requestVolMute() {
    sendCommand(HITACHI_REQUEST_VOLUME_MUTE);
}

void HITACHI_CP_WU8450::volUp() {
    volIncreasePoll = 5;
    sendCommand(HITACHI_SET_VOL_UP);
}

void HITACHI_CP_WU8450::volDown() {
    volDecreasePoll = 5;
    sendCommand(HITACHI_SET_VOL_DOWN);
}

void HITACHI_CP_WU8450::toggleMute() {
    if(projVolumeMute) {
        projVolumeMute = false;
        sendCommand(HITACHI_SET_VOL_MUTE_OFF);
    } else {
        projVolumeMute = true;
        sendCommand(HITACHI_SET_VOL_MUTE_ON);
    }
}

void HITACHI_CP_WU8450::loadPreset(DisplayPreset preset) {
    loadLensExecute = true;
    switch (preset) {
    case DISPLAY_PRESET_1:
        sendCommand(HITACHI_LOAD_LENS_1);
        break;
    case DISPLAY_PRESET_2:
        sendCommand(HITACHI_LOAD_LENS_2);
        break;
    case DISPLAY_PRESET_3:
        sendCommand(HITACHI_LOAD_LENS_3);
        break;
    }
}

void HITACHI_CP_WU8450::sendCommand(HITACHI_CP_WU8450::HITACHI_COMMANDS cmd) {
    QByteArray header{"\xbe\xef\x03\x06\x00"};
    QByteArray ACTION_WRITE{"\x01\x00"};
    QByteArray ACTION_READ{"\x02\x00"};
    QByteArray TYPE_POWER{"\x00\x60"};
    QByteArray TYPE_VOLUME{"\x50\x20"};
    QByteArray TYPE_VOLUME_MUTE{"\x02\x20"};
    QByteArray TYPE_INPUT{"\x00\x20"};
    latestRequest = cmd;
    int msgSize = 13;
    // Command,               Header-----------------------,----CRC----, --Action--,----Type---, Setting Code
    char SET_POWER_ON[]     = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xba, 0xd2, 0x01, 0x00, 0x00, 0x60, 0x01, 0x00};
    char SET_POWER_OFF[]    = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x2a, 0xd3, 0x01, 0x00, 0x00, 0x60, 0x00, 0x00};
    char REQUEST_POWER[]    = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x19, 0xd3, 0x02, 0x00, 0x00, 0x60, 0x00, 0x00};
    // Command,               Header-----------------------,----CRC----, --Action--,----Type---, Setting Code
    char SET_VOL_UP[]       = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xab, 0xc3, 0x04, 0x00, 0x50, 0x20, 0x00, 0x00};
    char SET_VOL_DOWN[]     = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x7a, 0xc2, 0x05, 0x00, 0x50, 0x20, 0x00, 0x00};
    char SET_VOL_MUTE_ON[]  = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xd6, 0xd2, 0x01, 0x00, 0x02, 0x20, 0x01, 0x00};
    char SET_VOL_MUTE_OFF[] = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x46, 0xd3, 0x01, 0x00, 0x02, 0x20, 0x00, 0x00};
    char REQUEST_VOL[]      = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xcd, 0xc3, 0x02, 0x00, 0x50, 0x20, 0x00, 0x00};
    char REQUEST_VOL_MUTE[] = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x75, 0xd3, 0x02, 0x00, 0x02, 0x20, 0x00, 0x00};
    // Command,                       Header-----------------------,----CRC----, --Action--,----Type----, Setting Code
    char LOAD_LENS_1[]              = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x4b, 0x92, 0x01, 0x00, 0x07, 0x24, 0x00, 0x00};
    char LOAD_LENS_2[]              = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xdb, 0x93, 0x01, 0x00, 0x07, 0x24, 0x01, 0x00};
    char LOAD_LENS_3[]              = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x2b, 0x93, 0x02, 0x00, 0x07, 0x24, 0x02, 0x00};
    char LOAD_LENS_EXECUTE[]        = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xe8, 0x90, 0x06, 0x00, 0x08, 0x24, 0x00, 0x00};
    // Command,               Header------------------------,----CRC----, --Action--,----Type----, Setting Code
    char SET_BLANK_ON[]      = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x6b, 0xd9, 0x01, 0x00, 0x20, 0x30, 0x01, 0x00};
    char SET_BLANK_OFF[]     = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xfb, 0xd8, 0x01, 0x00, 0x20, 0x30, 0x00, 0x00};
    // Command,                      Header------------------------,----CRC----, --Action--,----Type----, Setting Code
    char SET_INPUT_HDMI_1[]         = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x0e, 0xd2, 0x01, 0x00, 0x00, 0x20, 0x03, 0x00};
    char SET_INPUT_HDMI_2[]         = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x6e, 0xd6, 0x01, 0x00, 0x00, 0x20, 0x0d, 0x00};
    char SET_INPUT_COMPUTER_1[]     = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xfe, 0xd2, 0x01, 0x00, 0x00, 0x20, 0x00, 0x00};
    char SET_INPUT_COMPUTER_2[]     = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x3e, 0xd0, 0x01, 0x00, 0x00, 0x20, 0x04, 0x00};
    char SET_INPUT_VIDEO[]          = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x6e, 0xd3, 0x01, 0x00, 0x00, 0x20, 0x01, 0x00};
    char SET_INPUT_SVIDEO[]         = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x9e, 0xd3, 0x01, 0x00, 0x00, 0x20, 0x02, 0x00};
    char SET_INPUT_USB_A[]          = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x5e, 0xd1, 0x01, 0x00, 0x00, 0x20, 0x06, 0x00};
    char SET_INPUT_USB_B[]          = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xfe, 0xd7, 0x01, 0x00, 0x00, 0x20, 0x0c, 0x00};
    char SET_INPUT_LAN[]            = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xce, 0xd5, 0x01, 0x00, 0x00, 0x20, 0x0b, 0x00};
    char SET_INPUT_COMPONENT[]      = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xae, 0xd1, 0x01, 0x00, 0x00, 0x20, 0x05, 0x00};
    char REQUEST_INPUT[]            = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xcd, 0xd2, 0x02, 0x00, 0x00, 0x20, 0x00, 0x00};
    // Command,                      Header------------------------,----CRC----, --Action--,----Type----, Setting Code
    char REQUEST_LAMP_LOW[]         = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xc2, 0xff, 0x02, 0x00, 0x90, 0x10, 0x00, 0x00};
    char REQUEST_LAMP_HIGH[]        = {0xbe, 0xef, 0x03, 0x06, 0x00, 0x2a, 0xfd, 0x02, 0x00, 0x9e, 0x10, 0x00, 0x00};
    // Command,                      Header------------------------,----CRC----, --Action--,----Type----, Setting Code
    char REQUEST_FILTER_LOW[]       = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xc2, 0xf0, 0x02, 0x00, 0xa0, 0x10, 0x00, 0x00};
    char REQUEST_FILTER_HIGH[]      = {0xbe, 0xef, 0x03, 0x06, 0x00, 0xd6, 0xfc, 0x02, 0x00, 0x9f, 0x10, 0x00, 0x00};


    sock->close();
    switch (cmd) {
        case HITACHI_SET_PWR_ON:
        send(QByteArray(reinterpret_cast<char *>(SET_POWER_ON), msgSize));
        break;
    case HITACHI_SET_PWR_OFF:
        send(QByteArray(reinterpret_cast<char *>(SET_POWER_OFF), msgSize));
        break;
    case HITACHI_REQUEST_PWR:
        send(QByteArray(reinterpret_cast<char *>(REQUEST_POWER), msgSize));
        break;
    case HITACHI_SET_VOL_UP:
        send(QByteArray(reinterpret_cast<char *>(SET_VOL_UP), msgSize));
        break;
    case HITACHI_SET_VOL_DOWN:
        send(QByteArray(reinterpret_cast<char *>(SET_VOL_DOWN), msgSize));
        break;
    case HITACHI_SET_VOL_MUTE_ON:
        send(QByteArray(reinterpret_cast<char *>(SET_VOL_MUTE_ON), msgSize));
        break;
    case HITACHI_SET_VOL_MUTE_OFF:
        send(QByteArray(reinterpret_cast<char *>(SET_VOL_MUTE_OFF), msgSize));
        break;
    case HITACHI_REQUEST_VOLUME_MUTE:
        send(QByteArray(reinterpret_cast<char *>(REQUEST_VOL_MUTE), msgSize));
        break;
    case HITACHI_REQUEST_VOLUME:
        send(QByteArray(reinterpret_cast<char *>(REQUEST_VOL), msgSize));
        break;
    case HITACHI_LOAD_LENS_1:
        qDebug() << "HITACHI_CP_WU8450::sendCommand() Load lens 1";
        send(QByteArray(reinterpret_cast<char *>(LOAD_LENS_1), msgSize));
        break;
    case HITACHI_LOAD_LENS_2:
        qDebug() << "HITACHI_CP_WU8450::sendCommand() Load lens 2";
        send(QByteArray(reinterpret_cast<char *>(LOAD_LENS_2), msgSize));
        break;
    case HITACHI_LOAD_LENS_3:
        qDebug() << "HITACHI_CP_WU8450::sendCommand() Load lens 3";
        send(QByteArray(reinterpret_cast<char *>(LOAD_LENS_3), msgSize));
        break;
    case HITACHI_LOAD_LENS_EXECUTE:
        loadLensExecute = false;
        qDebug() << "HITACHI_CP_WU8450::sendCommand() Load lens execute";
        send(QByteArray(reinterpret_cast<char *>(LOAD_LENS_EXECUTE), msgSize));
        break;
    case HITACHI_SET_BLANK_ON:
        send(QByteArray(reinterpret_cast<char *>(SET_BLANK_ON), msgSize));
        break;
    case HITACHI_SET_BLANK_OFF:
        send(QByteArray(reinterpret_cast<char *>(SET_BLANK_OFF), msgSize));
        break;
    case HITACHI_REQUEST_INPUT:
        send(QByteArray(reinterpret_cast<char *>(REQUEST_INPUT), msgSize));
        break;
    case HITACHI_SET_INPUT_LAN:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_LAN), msgSize));
        break;
    case HITACHI_SET_INPUT_HDMI1:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_HDMI_1), msgSize));
        break;
    case HITACHI_SET_INPUT_HDMI2:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_HDMI_2), msgSize));
        break;
    case HITACHI_SET_INPUT_VIDEO:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_VIDEO), msgSize));
        break;
    case HITACHI_SET_INPUT_S_VIDEO:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_SVIDEO), msgSize));
        break;
    case HITACHI_SET_INPUT_COMPONENT:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_COMPONENT), msgSize));
        break;
    case HITACHI_SET_INPUT_COMPUTER1:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_COMPUTER_1), msgSize));
        break;
    case HITACHI_SET_INPUT_COMPUTER2:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_COMPUTER_2), msgSize));
        break;
    case HITACHI_SET_INPUT_USB_TYPE_A:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_USB_A), msgSize));
        break;
    case HITACHI_SET_INPUT_USB_TYPE_B:
        send(QByteArray(reinterpret_cast<char *>(SET_INPUT_USB_B), msgSize));
        break;
    case HITACHI_REQUEST_LAMP_STATUS_HIGH:
        send(QByteArray(reinterpret_cast<char *>(REQUEST_LAMP_HIGH), msgSize));
        break;
    case HITACHI_REQUEST_LAMP_STATUS_LOW:
        send(QByteArray(reinterpret_cast<char *>(REQUEST_LAMP_LOW), msgSize));
        break;
    case HITACHI_REQUEST_FILTER_STATUS_HIGH:
        send(QByteArray(reinterpret_cast<char *>(REQUEST_FILTER_HIGH), msgSize));
        break;
    case HITACHI_REQUEST_FILTER_STATUS_LOW:
        send(QByteArray(reinterpret_cast<char *>(REQUEST_FILTER_LOW), msgSize));
        break;
    }
}

void HITACHI_CP_WU8450::setAutheticationRequired(bool state) {
    authenticationRequired = state;
}

void HITACHI_CP_WU8450::response(QByteArray msg) {
    qDebug() << "HITACHI_CP_WU8450::response() " << msg;
    if(loadLensExecute) {
        sendCommand(HITACHI_LOAD_LENS_EXECUTE);
    }
    if(volIncreasePoll > 0) {
        volIncreasePoll--;
        sendCommand(HITACHI_SET_VOL_UP);
        return;
    }
    if(volDecreasePoll > 0) {
        volDecreasePoll--;
        sendCommand(HITACHI_SET_VOL_DOWN);
        return;
    }
    // Parse received message
    switch (latestRequest) {
    case HITACHI_REQUEST_PWR:
        // POWER off response "\x1D\x00\x00"
        // POWER on response "\x1D\x01\x00"
        if(msg.at(0) != 0x1d) return;
        if(msg.at(1) == 0x00) {
            powerState = false;
            qDebug() << "HITACHI_CP_WU8450::response() Power is off";
            emit statusChanged("Proj,Pwr,0");
        } else if(msg.at(1) == 0x01) {
            powerState = true;
            qDebug() << "HITACHI_CP_WU8450::response() Power is on";
            emit statusChanged("Proj,Pwr,1");
        } else if(msg.at(1) == 0x02) {
            qDebug() << "HITACHI_CP_WU8450::response() Cooling down";
            emit statusChanged("Proj,Pwr,2");
        }
        break;
    case HITACHI_REQUEST_INPUT:
        if(msg.at(0) != 0x1d) return;
        if(msg.at(1) == 0x0d) {
            qDebug() << "HITACHI_CP_WU8450::response() Input HDMI 2";
            emit statusChanged("Proj,Inpt,HDMI_2");
        } else if(msg.at(1) == 0x0b) {
            qDebug() << "HITACHI_CP_WU8450::response() Input LAN";
            emit statusChanged("Proj,Inpt,LAN");
        } else if(msg.at(1) == 0x03) {
            qDebug() << "HITACHI_CP_WU8450::response() Input HDMI 1";
            emit statusChanged("Proj,Inpt,HDMI_1");
        } else if(msg.at(1) == 0x00) {
            qDebug() << "HITACHI_CP_WU8450::response() Input COMPUTER IN 1";
            emit statusChanged("Proj,Inpt,COMPUTER_IN_1");
        } else if(msg.at(1) == 0x04) {
            qDebug() << "HITACHI_CP_WU8450::response() Input COMPUTER IN 2";
            emit statusChanged("Proj,Inpt,COMPUTER_IN_2");
        } else if(msg.at(1) == 0x06) {
            qDebug() << "HITACHI_CP_WU8450::response() Input USB TYPE A";
            emit statusChanged("Proj,Inpt,USB_TYPE_A");
        }  else if(msg.at(1) == 0x0c) {
            qDebug() << "HITACHI_CP_WU8450::response() Input USB TYPE B";
            emit statusChanged("Proj,Inpt,USB_TYPE_B");
        }  else if(msg.at(1) == 0x05) {
            qDebug() << "HITACHI_CP_WU8450::response() Input COMPONENT";
            emit statusChanged("Proj,Inpt,COMPONENT");
        }  else if(msg.at(1) == 0x02) {
            qDebug() << "HITACHI_CP_WU8450::response() Input S-VIDEO";
            emit statusChanged("Proj,Inpt,S_VIDEO");
        }  else if(msg.at(1) == 0x01) {
            qDebug() << "HITACHI_CP_WU8450::response() Input VIDEO";
            emit statusChanged("Proj,Inpt,VIDEO");
        }
        break;
    case HITACHI_REQUEST_VOLUME:
        if(msg.at(0) != 0x1d) return;
        // Convert value to integer
        projVolume = static_cast<int>(msg.at(1));
        // Min = 0, Max = 0
        // Turn to 0 to 100
        projVolume = static_cast<int>(static_cast<float>(projVolume) * static_cast<float>(static_cast<float>(100)/static_cast<float>(48)));
        qDebug() << "HITACHI_CP_WU8450::response() Volume=" << QString::number(projVolume);
        emit statusChanged("Proj,Vol," + QByteArray::number(projVolume));
        break;
    case HITACHI_REQUEST_LAMP_STATUS_LOW:
        break;
    case HITACHI_REQUEST_LAMP_STATUS_HIGH:
        break;
    case HITACHI_REQUEST_FILTER_STATUS_LOW:
        break;
    case HITACHI_REQUEST_FILTER_STATUS_HIGH:
        break;
    case HITACHI_REQUEST_VOLUME_MUTE:
        if(msg.at(0) != 0x1d) return;
        if(msg.at(1) == 0x00) {
            projVolumeMute = false;
            emit statusChanged("Proj,Vol,Mute,0");
        } else if(msg.at(1) == 0x01) {
            projVolumeMute = true;
            emit statusChanged("Proj,Vol,Mute,1");
        }
        break;
    }
    sock->close();
}

void HITACHI_CP_WU8450::send(QByteArray msg) {
    pendingCommand = msg;
    sock->connect(ipAddress, port);
}

void HITACHI_CP_WU8450::connectedToHost() {
    qDebug() << "HITACHI_CP_WU8450::connectedToHost()";
    if(!authenticationRequired) {
        qDebug() << "HITACHI_CP_WU8450::connectedToHost() No auth";
        sock->send(pendingCommand);
        pendingCommand.clear();
    }
}

void HITACHI_CP_WU8450::requestStatus()
{
    switch (requestPoll) {
    case 1:
        qDebug() << "HITACHI_CP_WU8450::requestStatus() Power";
        requestPwr();
        break;
    case 2:
        qDebug() << "HITACHI_CP_WU8450::requestStatus() Volume";
        requestVol();
        break;
    case 3:
        qDebug() << "HITACHI_CP_WU8450::requestStatus() Input";
        requestInput();
        break;
    case 4:
        qDebug() << "HITACHI_CP_WU8450::requestStatus() Lamp low";
        requestLampLow();
        break;
    case 5:
        qDebug() << "HITACHI_CP_WU8450::requestStatus() Lamp high";
        requestLampHigh();
        break;
    case 6:
        qDebug() << "HITACHI_CP_WU8450::requestStatus() Filter low";
        requestFilterLow();
        break;
    case 7:
        qDebug() << "HITACHI_CP_WU8450::requestStatus() Filter high";
        requestFilterHigh();
        break;
    case 8:
        qDebug() << "HITACHI_CP_WU8450::requestStatus() Volume mute";
        requestVolMute();
        break;
    }
    if(requestPoll > 8) requestPoll = 0;
    else requestPoll++;
}
