// Qt Includes
#include <QtTest/QtTest>
// Project includes
#include "pjlink.h"
class PJLinkTester: public QObject
{
    Q_OBJECT
private slots:
    void testConnectionParameters();
};

void PJLinkTester::testConnectionParameters()
{
    // Start a local server to read pjlink driver input
    PJLink pjlink;
    pjlink.setPort(10001);
    QVERIFY(pjlink.getPort() == 10001);
    pjlink.setPassword("12345");
    QCOMPARE(pjlink.getPassword(), "12345");
    pjlink.setIpAddress("127.0.0.1");
    QCOMPARE(pjlink.getIpAddress(), "127.0.0.1");
}

QTEST_MAIN(PJLinkTester)
#include "pjlinktester.moc"
