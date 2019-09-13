#include <QtTest>

// add necessary includes here

class PJLinkTester : public QObject
{
    Q_OBJECT

public:
    PJLinkTester();
    ~PJLinkTester();

private slots:
    void test_case1();

};

PJLinkTester::PJLinkTester()
{

}

PJLinkTester::~PJLinkTester()
{

}

void PJLinkTester::test_case1()
{

}

QTEST_APPLESS_MAIN(PJLinkTester)

#include "tst_pjlinktester.moc"
