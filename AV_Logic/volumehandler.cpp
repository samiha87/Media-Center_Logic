#include "volumehandler.h"

VolumeHandler::VolumeHandler(QObject *parent) : QObject(parent)
{
    currentDevice = eVolumeRaspberryHDMI;
    osmc.setAddress("10.42.0.232", 80);
    setDefaults();
}

void VolumeHandler::setDefaults() {
    osmc.setVolume(50);
    volMute = false;
    osmc.setVolumeMute(false);
}

void VolumeHandler::setVolumeUp() {
    QByteArray msg;
    switch (currentDevice) {
        case eVolumeRaspberryHDMI:
            // Form correct message
            osmc.setVolumeUp();
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
            osmc.setVolumeDown();
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

void VolumeHandler::setVolume(int volLvl) {
    QByteArray msg;
    switch (currentDevice) {
        case eVolumeRaspberryHDMI:
            // Form correct message
            osmc.setVolume(volLvl);
        break;
    case eVolumeRaspberryHeadphones:
         msg = "#HW,HP,Vol,Set=" + QByteArray::number(volLvl) + "*" ;
         emit volumeChanged(msg);
        // Do nothing
        break;
    case eVolumeAmplifier:
        break;
    }
}

void VolumeHandler::setVolumeMute(bool choice) {
    Q_UNUSED(choice);
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

void VolumeHandler::toggleVolumeMute() {
    QByteArray msg;
    switch (currentDevice) {
        case eVolumeRaspberryHDMI:
            if(volMute) {
                volMute = !volMute;
                osmc.setVolumeMute(false);
             } else {
                volMute = !volMute;
                 osmc.setVolumeMute(true);
              }
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
