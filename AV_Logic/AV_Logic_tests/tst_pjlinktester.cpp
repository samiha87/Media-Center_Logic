// Qt Includes
#include <QtTest>
#include <QThread>

// Project Includes
#include "../pjlink.h"
#include "pjlinktestserver.h"
#include "pjlinkserver.h"
// add necessary includes here

// Qt Includes
#include <QObject>

class PJClient: public QObject
{
    Q_OBJECT

public slots:
    void doWork(const QString &parameter) {
        pj = new PJLink();
        pj->setIpAddress("127.0.0.1");    // Localhost
        pj->setPort(4242);
        pj->setPassword("12345");
        pj->setPower(true);             // Send command power on to tcp
    }

signals:
    void resultReady(const QString &result);
private:
    PJLink *pj;
};

class PJLinkTester : public QObject
{
    Q_OBJECT
    QThread serverThread;
    QThread clientThread;
public:
    PJLinkTester();
    ~PJLinkTester();
signals:
    void operate(const QString &);
    void runClient(const QString &);
private slots:
    void testConnectionParameters();
    void testConnectingHost();
};

PJLinkTester::PJLinkTester()
{

}

PJLinkTester::~PJLinkTester()
{

}

void PJLinkTester::testConnectionParameters()
{
    PJLink pj;
    pj.setPort(10001);
    QVERIFY(pj.getPort() == 10001);
    pj.setPassword("12345");
    QCOMPARE(pj.getPassword(), "12345");
    pj.setIpAddress("127.0.0.1");
    QCOMPARE(pj.getIpAddress(), "127.0.0.1");
}

void PJLinkTester::testConnectingHost() {
// Host a server
    PJLinkServer *pjThread = new PJLinkServer();
    pjThread->moveToThread(&serverThread);
    connect(this, SIGNAL(operate(const QString)), pjThread, SLOT(doWork(const QString)));
    serverThread.start();
    emit operate("");    // Start thread
// Start client
    QThread::sleep(3);  // Give time for server to start
    PJClient *pjClientThread = new PJClient();
    pjClientThread->moveToThread(&clientThread);
    connect(this, SIGNAL(runClient(const QString)), pjClientThread, SLOT(doWork(const QString)));
    clientThread.start();
    emit runClient("now");    // Start thread
    QThread::sleep(1); // wait for 2 seconds
    // Check if hashed message was sucessfull

    serverThread.quit();
    serverThread.wait();
    clientThread.quit();
    clientThread.wait();
}

QTEST_MAIN(PJLinkTester)

#include "tst_pjlinktester.moc"
