#include "volumehandler.h"

VolumeHandler::VolumeHandler(QObject *parent) : QObject(parent)
{
    currentDevice = eVolumeRaspberryHDMI;
}

void VolumeHandler::setVolumeUp() {
    QByteArray msg;
    switch (currentDevice) {
        case eVolumeRaspberryHDMI:
            // Form correct message
            msg = "#HW,Vol,Up*";
            emit volumeChanged(msg);
        break;
    case eVolumeRaspberryHeadphones:
         msg = "#HW,HP,Vol,Up*";
         emit volumeChanged(msg);
        // Do nothing
        break;
    case eVolumeAmplifier:
        break;
    }
}

void VolumeHandler::setVolumeDown() {
    QByteArray msg;
    switch (currentDevice) {
        case eVolumeRaspberryHDMI:
            // Form correct message
            msg = "#HW,Vol,Down*";
            emit volumeChanged(msg);
        break;
    case eVolumeRaspberryHeadphones:
         msg = "#HW,HP,Vol,Down*";
         emit volumeChanged(msg);
        // Do nothing
        break;
    case eVolumeAmplifier:
        break;
    }
}

void VolumeHandler::setVolume(QByteArray volLvl) {
    QByteArray msg;
    switch (currentDevice) {
        case eVolumeRaspberryHDMI:
            // Form correct message
            msg = "#HW,Vol,Set=" + volLvl + "*" ;
            emit volumeChanged(msg);
        break;
    case eVolumeRaspberryHeadphones:
         msg = "#HW,HP,Vol,Set=" + volLvl + "*" ;
         emit volumeChanged(msg);
        // Do nothing
        break;
    case eVolumeAmplifier:
        break;
    }
}

void VolumeHandler::setVolumeMute(bool choice) {
    QByteArray msg;
    switch (currentDevice) {
        case eVolumeRaspberryHDMI:
            // Form correct message
            msg = "#HW,Vol,Mute=On*";
            emit volumeChanged(msg);
        break;
    case eVolumeRaspberryHeadphones:
         msg = "#HW,HP,Vol,Mute=On*";
         emit volumeChanged(msg);
        // Do nothing
        break;
    case eVolumeAmplifier:
        break;
    }
}

void VolumeHandler::setVolumeControl(eVolumeControlDevices dev) {
    currentDevice = dev;
}
