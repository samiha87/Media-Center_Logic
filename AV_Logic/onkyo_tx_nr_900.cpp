#include "onkyo_tx_nr_900.h"

#include "onkyors232commands.h"
#include <QSerialPort>
#include <QDebug>

Onkyo_AMP_RS232::Onkyo_AMP_RS232(QObject *parent) : QObject(parent)
{
    serial.setBaud(QSerialPort::Baud9600);
    serial.setPort("/dev/ttyUSB0");
    serial.setStop(QSerialPort::OneStop);
    serial.setParity(QSerialPort::NoParity);
    serial.setDataBits(QSerialPort::Data8);
    bool status = serial.openPort();
    qDebug() << "Onkyo_AMP_RS232::Onkyo_AMP_RS232() Open serial port() " << QString::number(status);

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(requestStatus()));
    QObject::connect(&serial, SIGNAL(received(QByteArray)), this, SLOT(fromDevice(QByteArray)));

    timer.start(3000);
}

void Onkyo_AMP_RS232::setName(QString name)
{
    deviceName = name;
}

QString Onkyo_AMP_RS232::getName()
{
    return deviceName;
}

void Onkyo_AMP_RS232::setPower(bool state)
{
    if(state) {
        serial.send(generateCommand(POWER, ON));
    } else {
        serial.send(generateCommand(POWER, OFF));
    }
}

void Onkyo_AMP_RS232::togglePower()
{
    if(devicePower) {
        serial.send(generateCommand(POWER, ON));
    } else {
        serial.send(generateCommand(POWER, OFF));
    }
}

bool Onkyo_AMP_RS232::getPower()
{
    return devicePower;
}

void Onkyo_AMP_RS232::setMute(bool state)
{
    if(state) serial.send(generateCommand(AUDIO_MUTE, ON));
    else serial.send(generateCommand(AUDIO_MUTE, OFF));
}

void Onkyo_AMP_RS232::toggleMute()
{
    if(deviceMute) serial.send(generateCommand(AUDIO_MUTE, ON));
    else serial.send(generateCommand(AUDIO_MUTE, OFF));
}

void Onkyo_AMP_RS232::setInput(AudioChannels channel)
{
    switch (channel) {
        case AudioChannels::Channel_CD:
            serial.send(generateCommand(INPUT, INPUT_CD));
        break;
    case AudioChannels::Channel_DVD:
        serial.send(generateCommand(INPUT, INPUT_DVD));
        break;
    case AudioChannels::Channel_PHONO:
        serial.send(generateCommand(INPUT, INPUT_PHONO));
        break;
    case AudioChannels::Channel_NONE:
        break;
    }
}

void Onkyo_AMP_RS232::volUp()
{
    serial.send(generateCommand(AUDIO_MASTER_VOL, AUDIO_VOL_UP));
}

void Onkyo_AMP_RS232::volDown()
{
    serial.send(generateCommand(AUDIO_MASTER_VOL, AUDIO_VOL_DOWN));
}

void Onkyo_AMP_RS232::setVol(int vol)
{
    serial.send(generateCommand(AUDIO_MASTER_VOL, QByteArray::number(vol)));
}

QByteArray Onkyo_AMP_RS232::generateCommand(QByteArray cmd, QByteArray parameter)
{
    QByteArray message;
    message.append(startByte);
    message.append(cmd);
    message.append(parameter);
    message.append(endByte);
    return message;
}

void Onkyo_AMP_RS232::fromDevice(QByteArray msg)
{
    // Parse message
    qDebug() << "Onkyo_AMP_RS232::fromDevice() " << msg;
}

void Onkyo_AMP_RS232::requestStatus()
{
    qDebug() << "Onkyo_AMP_RS232::requestStatus()";
    switch (requestStatusPoll) {
    case 0:
        serial.send(generateCommand(POWER, QUERY));
    break;
    case 1:
        serial.send(generateCommand(AUDIO_MUTE, QUERY));
        break;
    case 2:
        serial.send(generateCommand(LISTEN_MODE, QUERY));
        break;
    case 3:
        serial.send(generateCommand(AUDIO_MASTER_VOL, QUERY));
        break;
    case 4:
        serial.send(generateCommand(INPUT, QUERY));
        break;
    default:
        break;
    }
    requestStatusPoll++;
    if(requestStatusPoll > 4) requestStatusPoll = 0;
}
