#include "hardwareadapter.h"
#include <QDebug>

HardwareAdapter::HardwareAdapter(QObject *parent) : QObject(parent)
{

}

void HardwareAdapter::msgFromHardware(QByteArray msg) {
    // Parse mesage
    if (!msg.contains('#') || !msg.contains('*')) {
        qDebug() << "HardwareAdapter::msgFromHardware() Invalid msg";
        return;
    }

    // Check message type
    if(msg.contains("BLE")) {
        qDebug() << "HardwareAdapter::msgFromHardware() Bluetooth message received";
        qDebug() << "Message: " << msg;
        msg = msg.remove(0, 4); // Remove #BLE from message
        msg.chop(1);    // Remove end byte * from array
        emit bleMessageRx(msg);
    }
}
