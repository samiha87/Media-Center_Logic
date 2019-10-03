#include "audiologic.h"
#include "qdebug.h"

AudioLogic::AudioLogic(QObject *parent) : QObject(parent)
{

}

void AudioLogic::setDevice(AudioDevice *device_) {
    device = device_;
}

void AudioLogic::messageFromControl(QByteArray msg) {
    // Check if we have power command
    if(device == nullptr) {
        qDebug() << "AudioLogic::messageFromControl() device not initialized";
        return;
    }
    if(msg.contains("Vol")) {
        if(msg.contains("Up"))  {
            qDebug() << "AudioLogic::messageFromControl() Truning volume up";
            device->volUp();
        } else if(msg.contains("Down")) {
            qDebug() << "AudioLogic::messageFromControl() Turn volume down";
            device->volDown();
        } else if(msg.contains("Mute")) {
            qDebug() << "AudioLogic::messageFromControl() Toggle Mute";
            device->toggleMute();
        }
    }
}

void AudioLogic::statusChanged(QByteArray msg) {
    qDebug() << "DisplayLogic::statusChanged() " << msg;
    emit newMessage(msg);
}
