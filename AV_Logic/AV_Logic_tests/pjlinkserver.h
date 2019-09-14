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

public slots:
    void doWork(const QString &parameter) {
        qDebug() << "Thread running";
        tcp = new TCPServer(4242);
        connect(tcp, SIGNAL(stateChanged()), this, SLOT(statusCheck()));
        tcp->setPassword("12345");
        tcp->setType(1);
        tcp->waitConnection();
    }

    void statusCheck() {
        qDebug() << "Thread status check";
    }

signals:
    void resultReady(const QString &result);
    void hashedConnectionState(bool status);

private:
    TCPServer *tcp;
};

#endif // PJLINKSERVER_H
