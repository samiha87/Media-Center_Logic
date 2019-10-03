#include "devicepool.h"
#include "displaydevice.h"

DevicePool::DevicePool(QObject *parent) : QObject(parent)
{

}

void DevicePool::createDevice(QString deviceName, deviceControlType control, deviceTypes dev) {
    switch(control) {
    case InfraRed:
        createIRDevice(deviceName, dev);
        break;
    case RS232:
        createRS232Device(deviceName, dev);
        break;
    case Ethernet:
        createLanDevice(deviceName, dev);
        break;
    }
}

void DevicePool::createLanDevice(QString deviceName, deviceTypes dev) {
    qDebug() << "DevicePool::createLanDevice()";
    DisplayDevice *device;
    DisplayLogic *dLogic;
    AudioLogic *aLogic;
    AudioDevice *aDev;
    ProjectorConfigurator proj;
    AudioConfigurator audioConfig;
    switch(dev) {
    case eAmplifier:
        // Create audio device, <deviceName> <Device type> <Control type>
        qDebug() << "DevicePool::createLanDevice() Amplifier";
        aLogic = new AudioLogic();
        aDev = audioConfig.createAudioConfiguration(deviceName, AudioConfigurator::eAudioAmplifier, AudioConfigurator::eAudioIR, aLogic );
        aDev->setName(deviceName);
        aLogic->setDevice(aDev);
        audioDevices.append(aLogic);
        break;
    case eDisplay:
        break;
    case eProjector:
        // Device name determines which type of control
        // If
        qDebug() << "DevicePool::createLanDevice() Projector";
        dLogic = new DisplayLogic();
        proj.setAuthorization("", "5233");
        proj.setLanConfiguration("10.42.0.100", 0);
        device = proj.createProjectorConfiguration(deviceName, ProjectorConfigurator::ePJLink, dLogic);
        dLogic->setDisplay(device);
        displayDevices.append(dLogic);
        break;
    case eLights:
        break;
    }
}

void DevicePool::createRS232Device(QString deviceName, deviceTypes dev) {
    switch(dev) {
    case eAmplifier:
        break;
    case eDisplay:
        break;
    case eProjector:
        break;
    case eLights:
        break;
    }
}

void DevicePool::createIRDevice(QString deviceName, deviceTypes dev) {
    AudioLogic *aLogic;
    AudioDevice *aDev;
    ProjectorConfigurator proj;
    AudioConfigurator audioConfig;
    switch(dev) {
    case eAmplifier:
        // Create audio device, <deviceName> <Device type> <Control type>
        qDebug() << "DevicePool::createIRDevice() Amplifier";
        aLogic = new AudioLogic();
        aDev = audioConfig.createAudioConfiguration(deviceName, AudioConfigurator::eAudioAmplifier, AudioConfigurator::eAudioIR, aLogic );
        aDev->setName(deviceName);
        aLogic->setDevice(aDev);
        audioDevices.append(aLogic);
        break;
    case eDisplay:
        break;
    case eProjector:
        break;
    case eLights:
        break;
    }
}
