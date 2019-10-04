#include "tcpsocket.h"


TCPSocket::TCPSocket(QObject *parent) : QObject(parent)
{
    socket = nullptr;
    instanceAddress = "";
    instancePort = 0;
    timer = new QTimer(this);
    socket = new QTcpSocket(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(reConnect()));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

// Public
void TCPSocket::connect(QString address, quint16 port) {
    if(socket == nullptr) return;
    socket->connectToHost(address, port);
    qDebug() << "TCPSocket::connect() " << address << " : " << QString::number(port);
    instanceAddress = address;
    instancePort = port;
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
       // QTimer::singleShot(10000, this, SLOT(reCreateConnection()));
        if(socket != nullptr) {
            // Recreate connection after x amount of time
        }
        // Connect to host
    }
}

void TCPSocket::send(QByteArray array) {
    if(socket == nullptr) {
        qDebug() << "TCPSocket::send() Socket is null";
        return;
    }
    if(!socket->isWritable())qDebug() << "TCPSocket::send() Socket not writable";
    if(!socket->isOpen()) {
        qDebug() << "TCPSocket::send() Socket not open";
        reConnect();
    }
    if(socket->isOpen() && socket->isWritable()) {
        //qDebug() << "TCPSocket::send bytearray " << hex << array;
        socket->write(array);
    }
}

void TCPSocket::close() {
    if(socket == nullptr) return;
        socket->close();
}

// SLOTS
void TCPSocket::reConnect() {
    connect(instanceAddress, instancePort);
}

void TCPSocket::connected() {
    //qDebug() << "TCPSocket::connected()";
    emit connectedToHost();
}
// If disconnects try reconnecting after 10 second
// TODO check raspi qt modules, QTimer singleshot is missing
void TCPSocket::disconnected() {
    //timer->start(10000);
}

void TCPSocket::bytesWritten(qint64 bytes) {
    Q_UNUSED(bytes)
}

// Called when anydata is incoming
void TCPSocket::readyRead() {
    if(socket == nullptr) return;
    QByteArray hwResponse = socket->readAll();
    //qDebug() << "TCPSocket::readyRead() " << hwResponse;
    emit response(hwResponse);
}
