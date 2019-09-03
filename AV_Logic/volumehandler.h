#ifndef VOLUMEHANDLER_H
#define VOLUMEHANDLER_H

#include <QObject>
#include "osmc.h"

class VolumeHandler : public QObject
{
    Q_OBJECT
public:
    enum eVolumeControlDevices {
        eVolumeRaspberryHDMI,
        eVolumeRaspberryHeadphones,
        eVolumeAmplifier,
    };
    // Constructor
    explicit VolumeHandler(QObject *parent = nullptr);
    // Set default settings and volume level to volume devices
    void setDefaults();
    // Increases volume by 1
    void setVolumeUp();
    // Deaceases volume by 1
    void setVolumeDown();
    //Set volume lvl
    void setVolume(int volLvl);
    // Set volume mute
    void setVolumeMute(bool choice);

    void setVolumeControl(eVolumeControlDevices dev);    // Default Raspberry Pi
signals:
    void volumeChanged(QByteArray msg);
private:
    eVolumeControlDevices currentDevice;    // Default Raspberry Pi
    OSMC osmc;

public slots:
};

#endif // VOLUMEHANDLER_H
