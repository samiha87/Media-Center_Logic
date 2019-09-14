// Qt Include
#include <QDebug>
#include <QTime>

// Project Include
#include "pjlinktestserver.h"

TCPServer::TCPServer(quint16 port, QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, port)) {
        qDebug() <<"TCPServer::TCPServer() Couldn't host server";
    } else {
        qDebug() << "TCPServer:TCPServer() Hosted succesfuly " << server->serverAddress().toString() << ":" << QString::number(server->serverPort());
    }
}
/*
* Connect
* Response PJLINK 1 00003a9e
* MD5 hash(00003a9e1323)
  Send: 3f89cff497594c407e98ab892561b87a%1POWR 1\r
  Response %1POWR=ERR3
  Responded error because projector was already on
*/
void TCPServer::newConnection() {
    running = true;
    QTcpSocket *socket = server->nextPendingConnection();
    // Respond PJLink type
    QByteArray msg;
    // Build response example. Response PJLINK 1 00003a9e
    msg.append("PJLINK ");
    msg.append(QString::number(type).toLocal8Bit());    // Type of PJLink 1 or 2
    msg.append(" ");
    QTime time = QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()));
    QByteArray randomKey;
    for(int i = 0; i < 9; i++) {
        int n = qrand() % 16;
        randomKey.append(QString::number(n, 16));
    }
    msg.append(randomKey);
    qDebug() << "TCPServer::TCPServer() Sending " << msg;
    if(!socket->isOpen())  qDebug() << "TCPServer::TCPServer() Socket not open";
    if(!socket->isWritable())  qDebug() << "TCPServer::TCPServer() Socket not writeable";
    socket->write(msg);
    socket->flush();
    socket->waitForBytesWritten();
}

bool TCPServer::isRunning() {
    return running;
}

void TCPServer::setType(int type_) {
    type = type_;
}

void TCPServer::setPassword(QString pass) {
    password = pass;
}

void TCPServer::sessionOpened() {

}

void TCPServer::sendRandomKey() {
    qDebug() << "TCPServer::sendRandomKey()";
}

void TCPServer::waitConnection() {
    if(server->waitForNewConnection(5000)) {
        qDebug() << "TCPServer:TCPServer() New connection found";
    } else {
        qDebug() << "TCPServer:TCPServer() No connections found";
    }
}

bool TCPServer::hasReply() {
    if(reply) {
        reply = false;
        return true;
    }
}


