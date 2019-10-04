#include "audiologic.h"
#include "qdebug.h"

AudioLogic::AudioLogic(QObject *parent) : QObject(parent)
{
    display = nullptr;
}

void AudioLogic::setDevice(AudioDevice *device_) {
    device = device_;
}

void AudioLogic::setDisplayDevice(DisplayDevice *device) {
    display = device;
}

void AudioLogic::setDriver(QObject *object){
    driver = object;
}

QObject *AudioLogic::getDriver() {
    return driver;
}

void AudioLogic::messageFromControl(QByteArray msg) {
    // Check if we have power command
    if(display == nullptr && device == nullptr) {
        qDebug() << "AudioLogic::messageFromControl() device not initialized";
        return;
    }
    if(msg.contains("Vol")) {
        if(msg.contains("Up"))  {
            qDebug() << "AudioLogic::messageFromControl() Truning volume up";
            if(display) display->volUp();
            else device->volUp();
        } else if(msg.contains("Down")) {
            qDebug() << "AudioLogic::messageFromControl() Turn volume down";
            if(display) display->volDown();
            else device->volDown();
        } else if(msg.contains("Mute")) {
            qDebug() << "AudioLogic::messageFromControl() Toggle Mute";
            if(display) display->toggleMute();
            else device->toggleMute();
        }
    }
}

void AudioLogic::statusChanged(QByteArray msg) {
    qDebug() << "AudioLogic::statusChanged() " << msg;
    emit newMessage(msg);
}

/*   QByteArray array;
    array.append("#r,Audio,Vol,");  // Start byte, Reply message, Device type, Volume
    array.append("Level=" + QByteArray::number(osmc.getVolume()));
    if(volMute)array.append(",Mute=On");
    else array.append(",Mute=Off");
    array.append("*");  // End byte
*/
void AudioLogic::displayStatusChanged(QByteArray msg) {
    qDebug() << "AudioLogic::displayStatusChanged() " << msg;
    bool sendForward = false;
    if(msg.contains("Proj,Vol,Mute,1")) {
        msg.replace("Proj,Vol,Mute,1", "#r,Audio,Vol,Mute=On");
        msg.append("*");
        sendForward = true;
    } else if(msg.contains("Proj,Vol,Mute,0")) {
        msg.replace("Proj,Vol,Mute,0", "#r,Audio,Vol,Mute=Off");
        msg.append("*");
        sendForward = true;
    } else if(msg.contains("Proj,Vol")) {
        msg.replace("Proj,Vol,", "#r,Audio,Vol,Level=");
        msg.append("*");
        sendForward = true;
    }
    if(sendForward) emit newMessage(msg);
}

