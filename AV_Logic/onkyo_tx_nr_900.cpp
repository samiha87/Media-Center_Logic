#include "onkyo_tx_nr_900.h"
#include "rs232device.h"
#include "onkyors232commands.h"

Onkyo_AMP_RS232::Onkyo_AMP_RS232(QObject *parent) : QObject(parent)
{

}

void Onkyo_AMP_RS232::setName(QString name)
{
    deviceName = name;
}

QString Onkyo_AMP_RS232::getName()
{
    return deviceName;
}

void Onkyo_AMP_RS232::setPower(bool state)
{
    if(state) {

    } else {

    }
}
