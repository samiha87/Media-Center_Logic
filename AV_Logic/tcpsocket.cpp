#include "tcpsocket.h"


TCPSocket::TCPSocket(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    socket = nullptr;
    instanceAddress = "";
    instancePort = 0;
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(connect(instanceAddress, instancePort)));
}
// Public


void TCPSocket::send(QByteArray array) {
    if(socket->isOpen()) {
        send(array);
    }
}

void TCPSocket::close() {
    if(socket->isOpen()) {
        socket->close();
    }
}

// SLOTS
void TCPSocket::connect(QString address, quint16 port) {

    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call
    socket->connectToHost(address, port);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}
void TCPSocket::connected() {

}
// If disconnects try reconnecting after 10 second
// TODO check raspi qt modules, QTimer singleshot is missing
void TCPSocket::disconnected() {
    timer->start(10000);
}

void TCPSocket::bytesWritten(qint64 bytes) {
    Q_UNUSED(bytes);
}
// Called when anydata is incoming
void TCPSocket::readyRead() {
    QByteArray hwResponse = socket->readAll();
    qDebug() << "TCPSocket::readyRead() " << hwResponse;
    emit response(hwResponse);

}
