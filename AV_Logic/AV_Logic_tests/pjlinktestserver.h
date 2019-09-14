#ifndef TCPSERVER_H
#define TCPSERVER_H
// Qt Includes
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(quint16 port, QObject *parent = nullptr);
    // Password need to match client
    void waitConnection();
    bool hasReply();
    void setPassword(QString pass);
    bool isRunning();
    void setType(int type_);

signals:
    void stateChanged();

private slots:
    void sessionOpened();
    // Once session opens, send PJLink1 and random key
    void sendRandomKey();
    void newConnection();
    void readyRead();

private:
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;
    QString password = "";
    bool running = false;
    bool reply = false;
    bool connectionOk = false;
    int type;   // Contains type of pjlink
    QByteArray randomKey = "";
};

#endif // TCPSERVER_H
