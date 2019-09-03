#include "volumehandler.h"

VolumeHandler::VolumeHandler(QObject *parent) : QObject(parent)
{
    currentDevice = eVolumeRaspberryHDMI;
}

void VolumeHandler::setVolumeUp() {
    switch (currentDevice) {
        case eVolumeRaspberryHDMI:
        break;
    case eVolumeRaspberryHeadphones:
        break;
    case eVolumeAmplifier:
        break;
    }
}

void VolumeHandler::setVolumeDown() {

}

void VolumeHandler::setVolume() {

}

void VolumeHandler::setVolumeMute() {

}

void VolumeHandler::setVolumeControl(eVolumeControlDevices dev) {
    currentDevice = dev;
}
