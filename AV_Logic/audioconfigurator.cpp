#include "audioconfigurator.h"
#include "rs232device.h"
#include "onkyo_tx_nr_900.h"

AudioConfigurator::AudioConfigurator(QObject *parent) : QObject(parent)
{
    lanConfigurationFlag = false;
    authConfigurationFlag = false;

    instanceAddress = "";
    instancePort = 0;
    instanceUser = "";
    instancePassword = "";
}

void AudioConfigurator::setLanConfiguration(QString ipAddress, quint16 port) {
    lanConfigurationFlag = true;
    instanceAddress = ipAddress;
    if(port == 0 ) return;
    instancePort = port;
}

void AudioConfigurator::setAuthorization(QString user, QString password) {
    authConfigurationFlag = true;
    instanceUser = user;
    instancePassword = password;
}

AudioDevice *AudioConfigurator::createAudioConfiguration(QString deviceName, AudioDeviceTypes type, AudioControlTypes control, QObject *connectTo) {
    Amplifier *object;
    Onkyo_AMP_RS232 *onkyo; // TODO figure out way to integrate wiht amplifier class

    switch (type) {
    case (eAudioAmplifierGeneric):
        object = new Amplifier();
        object->setName(deviceName);
        QObject::connect(object, SIGNAL(statusChanged(QByteArray)), connectTo, SLOT(statusChanged(QByteArray)));
        return configureAmplifier(object, control);  // Configure created amplifier and return it to device pool
    case eAudioAmplifierOnkyo:
        onkyo = new Onkyo_AMP_RS232();
        onkyo->setName(deviceName);
        QObject::connect(onkyo, SIGNAL(statusChanged(QByteArray)), connectTo, SLOT(statusChanged(QByteArray)));
        return static_cast<AudioDevice *>(onkyo);
    case (eAudioTV):
        break;
    case (eAudioProjector):
        break;
    case (eAudioAmplifierMicrophones):
        break;
    case (eAudioAmplifierDSP):
        break;
    }
    // Configure object
    return nullptr;
}

void AudioConfigurator::clearConfiguration() {
    lanConfigurationFlag = false;
    authConfigurationFlag = false;

    instanceAddress = "";
    instancePort = 0x00;
    instanceUser = "";
    instancePassword = "";
}

void AudioConfigurator::setName(QString name) {
    instanceName = name;
}

AudioDevice *AudioConfigurator::configureAmplifier(Amplifier *amp, AudioControlTypes control) {
    IRDevice *irdev;
    RS232Device *rs232;
    switch(control) {
    case eAudioIR:
        irdev = new IRDevice();
        irdev->setDeviceName(amp->getName());
        amp->setIR(irdev);
        return static_cast<AudioDevice *>(amp);
    case eAudioLAN:
        break;
    case eAudioRS232:
        break;
    }
    return nullptr;
}
