#include "displaylogic.h"
#include <QDebug>

DisplayLogic::DisplayLogic(QObject *parent) : QObject(parent)
{
    device = nullptr;
}

void DisplayLogic::setDisplay(DisplayDevice *display) {
    device = display;
}

void DisplayLogic::messageFromControl(QByteArray msg) {
    // Check if we have power command
    if(device == nullptr) {
        qDebug() << "DisplayLogic::messageFromControl() device not initialized";
        return;
    }
    qDebug() << "Logic::displayParser() " << msg;
    if(msg.contains("Pwr")) {
        if(msg.contains("On"))  {
            qDebug() << "DisplayLogic::messageFromControl() Truning projector on";
            device->setPower(true);
        } else if(msg.contains("Off")) {
            qDebug() << "DisplayLogic::messageFromControl() Turn projector off";
            device->setPower(false);
        }
    }
}

void DisplayLogic::statusChanged(QByteArray msg) {
    qDebug() << "DisplayLogic::statusChanged() " << msg;
    emit newMessage(msg);
}
