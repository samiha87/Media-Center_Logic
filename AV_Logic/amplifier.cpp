#include "amplifier.h"

Amplifier::Amplifier(QObject *parent) : QObject(parent)
{
}

void Amplifier::setName(QString name) {
    deviceName = name;
}

QString Amplifier::getName() {
    return deviceName;
}

void Amplifier::setPower(bool state) {
    if(state) irDevice->sendCommand(IRDevice::IRGPowerOn);
    else irDevice->sendCommand(IRDevice::IRGPowerOff);
}

void Amplifier::togglePower() {
    irDevice->sendCommand(IRDevice::IRGPowerToggle);
}

bool Amplifier::getPower() {
    return true;
}

void Amplifier::setVol(int vol) {
    Q_UNUSED(vol)
}

void Amplifier::volUp() {
    irDevice->sendCommand(IRDevice::IRAudioVolUp);
}

void Amplifier::volDown() {
    irDevice->sendCommand(IRDevice::IRAudioVolDown);
}

void Amplifier::setMute(bool state) {
    Q_UNUSED(state)
    // IR dosen't allow set states
    irDevice->sendCommand(IRDevice::IRAudioVolMuteToggle);
}

void Amplifier::toggleMute() {
    irDevice->sendCommand(IRDevice::IRAudioVolMuteToggle);
}

void Amplifier::setInput(AudioChannels channel) {
    switch (channel) {
    case AudioDevice::Channel_CD:
        //irDevice->sendCommand(IRDevice::IR)
        break;
    case AudioDevice::Channel_DVD:
        break;
    case AudioDevice::Channel_NONE:
        break;
    }
}

void Amplifier::setIR(IRDevice *ir) {
    irDevice = ir;
}
