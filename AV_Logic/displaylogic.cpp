#include "displaylogic.h"
#include <QDebug>

DisplayLogic::DisplayLogic(QObject *parent) : QObject(parent)
{
    device = nullptr;
    latestPowerRequest = false;
    latestInputRequest = DisplayDevice::Channel_HDMI_1;
}

void DisplayLogic::setDisplay(DisplayDevice *display) {
    device = display;
}

void DisplayLogic::setDriver(QObject *object) {
    driver = object;
}

QObject *DisplayLogic::getDriver() {
    return driver;
}

DisplayDevice *DisplayLogic::getDisplay() {

    return device;
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
            latestPowerRequest = true;
        } else if(msg.contains("Off")) {
            qDebug() << "DisplayLogic::messageFromControl() Turn projector off";
            device->setPower(false);
            latestPowerRequest = false;
        }
    }
    // Proj,Inpt,<int>*
    if(msg.contains("Inpt")) {
        qDebug() << "DisplayLogic::messageFromControl() Truning projector on";
        msg.replace("Proj,inpt=", "");
        msg.chop('*');
        int choice = msg.toInt();
        latestInputRequest = static_cast<DisplayDevice::DisplayChannels>(choice);
        switch(latestInputRequest) {
        case DisplayDevice::Channel_HDMI_1:
                device->setInput(DisplayDevice::Channel_HDMI_1);
            break;
        case DisplayDevice::Channel_HDMI_2:
            break;
        }
    }
}

void DisplayLogic::statusChanged(QByteArray msg) {
    qDebug() << "DisplayLogic::statusChanged() " << msg;
    // Analyze
    if(msg.contains("Proj,Pwr,0") && latestPowerRequest) {
        device->setPower(true);
    }
    if (msg.contains("Proj,Pwr,1") && !latestPowerRequest) {
        device->setPower(false);
    }
    emit newMessage(msg);
}
