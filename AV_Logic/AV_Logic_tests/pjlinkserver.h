#ifndef PJLINKSERVER_H
#define PJLINKSERVER_H
// Qt Includes
#include <QObject>
#include <QThread>
// Project Includes
#include "pjlinktestserver.h"

class PJLinkServer: public QObject
{
    Q_OBJECT
public:
    bool isConnected() {
        return connected;
    }
public slots:
    void doWork(const QString &parameter) {
        qDebug() << "Thread running";
        tcp = new TCPServer(4242);
        connect(tcp, SIGNAL(stateChanged()), this, SLOT(connectionOk()));
        tcp->setPassword("12345");
        tcp->setType(1);
        tcp->waitConnection();
    }

    void connectionOk() {
        qDebug() << "PJLinkServer::Connection ok";
        emit connectionTestedOk();
        connected = true;
    }
signals:
    void connectionTestedOk();
signals:
    void resultReady(const QString &result);
    void hashedConnectionState(bool status);

private:
    TCPServer *tcp;
    bool connected = false;
};

#endif // PJLINKSERVER_H
