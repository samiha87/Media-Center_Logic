#include "irdevice.h"
#include <QProcess>
#include <qdebug.h>
IRDevice::IRDevice()
{

}
void IRDevice::setDeviceName(QString name) {
    deviceName = name;
}
void IRDevice::sendCommand(IRCommands cmd) {
    // Device name is called internally in sendCmdToBash function
    switch (cmd) {
    case IRGPowerOn: {
        sendCmdToBash("POWER_OM");
        break;
    }
    case IRGPowerOff: {
        sendCmdToBash("POWER_OFF");
        break;
    }
    case IRGPowerToggle: {
        sendCmdToBash("POWER_TOGGLE");
        break;
    }
    case IRVideoInputHDMI1: {
        sendCmdToBash("INPUT_HDMI1");
        break;
    }
    case IRVideoInputVGA1: {
        sendCmdToBash("INPUT_VGA1");
        break;
    }
    case IRAudioVolUp: {
        sendCmdToBash("AUDIO_VOL_UP");
        //sendCmdToBash("KEY_UP");  // Testing purposes
        break;
    }
    case IRAudioVolDown: {
        sendCmdToBash("AUDIO_VOL_DOWN");
        break;
    }
    case IRAudioVolMuteToggle: {
        sendCmdToBash("AUDIO_VOL_MUTE");
        break;
    }

    }
}

bool IRDevice::getPower() {
    return currentPower;
}

int IRDevice::getVolume() {
    return currentVolume;
}

void IRDevice::sendCmdToBash(QString cmd) {
    qDebug() << "IRDEvice::sendCmdToBash() " << cmd;
    QString command = "irsend -d /var/run/lirc/lircd-lirc0 SEND_ONCE "
            + deviceName + " " + cmd;
    QProcess::execute(command);
}
