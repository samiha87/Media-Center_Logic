#include "tcpsocket.h"

TCPSocket::TCPSocket(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
}

void TCPSocket::connect(QString address, int port) {

}

void TCPSocket::send(QByteArray array) {

}

void TCPSocket::close() {

}
