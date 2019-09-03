#ifndef VOLUMEHANDLER_H
#define VOLUMEHANDLER_H

#include <QObject>

class VolumeHandler : public QObject
{
    Q_OBJECT
public:
    enum eVolumeControlDevices {
        eVolumeRaspberryHDMI,
        eVolumeRaspberryHeadphones,
        eVolumeAmplifier,
    };

    explicit VolumeHandler(QObject *parent = nullptr);
    void setVolumeUp();
    void setVolumeDown();
    void setVolume(QByteArray volLvl);
    void setVolumeMute(bool choice);

    void setVolumeControl(eVolumeControlDevices dev);    // Default Raspberry Pi
signals:
    void volumeChanged(QByteArray msg);
private:
    eVolumeControlDevices currentDevice;    // Default Raspberry Pi

public slots:
};

#endif // VOLUMEHANDLER_H
