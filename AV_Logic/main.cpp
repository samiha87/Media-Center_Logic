#include <QCoreApplication>

#include "logic.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Logic logic;

    return a.exec();
}
