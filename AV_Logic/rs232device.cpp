#include "rs232device.h"

RS232Device::RS232Device(QObject *parent) : QObject(parent)
{
    portName = "/dev/ttyUSB0";              // On raspberrypi default
    baudrate = QSerialPort::Baud9600;       // Baudrate 9600
    parityBits = QSerialPort::NoParity;     // None
    stopBits = QSerialPort::OneStop;        // Stop bits 1

    connect(serial, &QSerialPort::readyRead, this, &RS232Device::readData);
}

void RS232Device::setPort(QString rsPort)
{
    portName = rsPort;
}

void RS232Device::setBaud(QSerialPort::BaudRate baudrate_)
{
    baudrate = baudrate_;
}

void RS232Device::setParity(QSerialPort::Parity parity_)
{
    parityBits = parity_;
}

void RS232Device::setStop(QSerialPort::StopBits stop_)
{
    stopBits = stop_;
}

void RS232Device::setFlowControl(QSerialPort::FlowControl flowControl_)
{
    flowControl = flowControl_;
}
void RS232Device::send(QByteArray msg)
{
    sendToDevice(msg);
}

void RS232Device::sendToDevice(const QByteArray &msg)
{
    serial->write(msg);
}

bool RS232Device::openPort()
{
    connected = false;
    serial->setPortName(portName);
    serial->setBaudRate(baudrate);
    serial->setDataBits(dataBits);
    serial->setParity(parityBits);
    serial->setStopBits(stopBits);
    serial->setFlowControl(flowControl);
    serial->open(QIODevice::ReadWrite);

    return serial->isOpen();
}

bool RS232Device::closePort()
{
    if(serial->isOpen()) {
        serial->close();
    }
    return serial->isOpen();
}
