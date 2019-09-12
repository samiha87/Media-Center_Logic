#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class TCPSocket : public QObject
{
    Q_OBJECT
public:
    explicit TCPSocket(QObject *parent = nullptr);
    // Connect to desired address and port
    void connect(QString address, quint16 port);
    // Send message to the socket
    void send(QByteArray array);
    // Close socket connection
    void close();

public slots:
    void reConnect();
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
signals:
    // Emit a response signal when message retrieved from socket
    void response(QByteArray array);
private:
    QTcpSocket *socket;
    QTimer *timer;

    QString instanceAddress;
    quint16 instancePort;
};

#endif // TCPSOCKET_H
