#ifndef IRCONTROL_H
#define IRCONTROL_H

#include <QObject>
/* Brief commands
 * Print devices irsend -d /var/run/lirc/lircd-lirc0 LIST "" ""
 * Send command irsend -d /var/run/lirc/lircd-lirc0 SEND_ONCE <device_name> <device_command>
*/
class IRDevice {
public:
    enum IRCommands{
        IRGPowerOn,
        IRGPowerOff,
        IRGPowerToggle,
        IRVideoInputHDMI1,
        IRVideoInputVGA1,
        IRAudioVolUp,
        IRAudioVolDown,
        IRAudioVolMuteToggle
    };

    IRDevice();
    // Set device name, this needs to be equal to irsend <device_name>
    void setDeviceName(QString name);
    void sendCommand(IRCommands cmd);

    // These are arbitrary values with IR devices you cannot request status
    // Status need to be memorized and assumed that user doesn't
    // control the device from itself
    bool getPower();
    QString getVGA();
    bool getAudioMute();
    int getVolume();

signals:

public slots:

private:
    void sendCmdToBash(QString cmd);
    QString deviceName;

    bool currentPower;
    int currentVolume;
};

#endif // IRCONTROL_H
