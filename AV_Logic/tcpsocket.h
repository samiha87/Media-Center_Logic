#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class TCPSocket : public QObject
{
    Q_OBJECT
public:
    explicit TCPSocket(QObject *parent = 0);;
    // Connect to desired address and port
    void connect(QString address, int port);
    // Send message to the socket
    void send(QByteArray array);
    // Close socket connection
    void close();

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
signals:
    // Emit a response signal when message retrieved from socket
    void response(QByteArray array);
private:
    QTcpSocket *socket;
};

#endif // TCPSOCKET_H
