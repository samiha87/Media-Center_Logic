#ifndef AMPLIFIER_H
#define AMPLIFIER_H
// Qt Includes
#include <QObject>
// Project includes
#include "irdevice.h"

class Amplifier : public QObject, public IRDevice
{
    Q_OBJECT
public:
    explicit Amplifier(QObject *parent = nullptr);
signals:

public slots:
};

#endif // AMPLIFIER_H
