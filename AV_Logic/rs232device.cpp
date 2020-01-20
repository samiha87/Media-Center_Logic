#include "rs232device.h"
#include <QDebug>

RS232Device::RS232Device(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort();
    portName = "/dev/ttyUSB0";              // On raspberrypi default
    baudrate = QSerialPort::Baud9600;       // Baudrate 9600
    parityBits = QSerialPort::NoParity;     // None
    stopBits = QSerialPort::OneStop;        // Stop bits 1

    QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
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

void RS232Device::setDataBits(QSerialPort::DataBits databits_)
{
    dataBits = databits_;
}

void RS232Device::setFlowControl(QSerialPort::FlowControl flowControl_)
{
    flowControl = flowControl_;
}
void RS232Device::send(QByteArray msg)
{
    sendToDevice(msg);
}

void RS232Device::readData()
{

    const QByteArray msg = serial->readAll();
    qDebug() << "RS232Device::readData() " << msg;
    emit received(msg);
}

void RS232Device::sendToDevice(const QByteArray &msg)
{
    qDebug() << "RS232Device::sendToDevice() " << msg;
    serial->write(msg);
}

bool RS232Device::openPort()
{
    qDebug() << "RS232Device::openPort() ";
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
