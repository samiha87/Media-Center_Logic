#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>

#include "pjlink.h"
#include "hardwareadapter.h"

class Logic : public QObject
{
    Q_OBJECT
public:
    explicit Logic(QObject *parent = nullptr);

signals:

public slots:
private:
    HardwareAdapter *hwAdapter;
    PJLink *pjlink;
};

#endif // LOGIC_H
