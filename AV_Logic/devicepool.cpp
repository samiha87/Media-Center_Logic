#include "devicepool.h"
#include "displaydevice.h"

DevicePool::DevicePool(QObject *parent) : QObject(parent)
{

}

void DevicePool::createDevice(QString deviceName, deviceControlType control, deviceTypes dev, driverTypes driverType) {
    switch(control) {
    case InfraRed:
        createIRDevice(deviceName, dev);
        break;
    case RS232:
        createRS232Device(deviceName, dev, driverType);
        break;
    case Ethernet:
        createLanDevice(deviceName, dev, driverType);
        break;
    }
}

void DevicePool::createLanDevice(QString deviceName, deviceTypes dev, driverTypes driverType) {
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
        switch (driverType) {
            case eDriverPJLink:
            break;
        case eDriverPanasonic:
            break;
        case eDriverHitachi:
            break;
        case eDriverSony:
            break;
        case eDriverOnkyo:
            break;
        }
        aLogic = new AudioLogic();
        aDev = audioConfig.createAudioConfiguration(deviceName, AudioConfigurator::eAudioAmplifierGeneric, AudioConfigurator::eAudioIR, aLogic );
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
        switch (driverType) {
        case eDriverPJLink:
            proj.setLanConfiguration("10.42.0.100", 0);
            proj.setAuthorization("", "5233");
            device = proj.createProjectorConfiguration(deviceName, ProjectorConfigurator::ePJLink, dLogic);
            break;
        case eDriverPanasonic:
            device = proj.createProjectorConfiguration(deviceName, ProjectorConfigurator::ePJLink, dLogic);
            break;
        case eDriverHitachi:
            proj.setLanConfiguration("10.42.0.100", 0);
            device = proj.createProjectorConfiguration(deviceName, ProjectorConfigurator::eHitachi, dLogic);
            break;
        case eDriverSony:
            device = proj.createProjectorConfiguration(deviceName, ProjectorConfigurator::ePJLink, dLogic);
            break;
        }
        device->setName(deviceName);
        dLogic->setDisplay(device);
        displayDevices.append(dLogic);
        break;
    case eLights:
        break;
    case eAudioViaProjector:
        // Create audio device, <deviceName> <Device type> <Control type>
        qDebug() << "DevicePool::createLanDevice() Amplifier";
        aLogic = new AudioLogic();
        // Check if current device exist as Display
        bool found = false;
        for(auto &c: displayDevices) {
            if(c->getDisplay()->getName() == deviceName) {
                QObject::connect(c, SIGNAL(newMessage(QByteArray)), aLogic, SLOT(displayStatusChanged(QByteArray)));
                aLogic->setDisplayDevice(c->getDisplay());
                found = true;
            }
        }
        if(found) audioDevices.append(aLogic);
        break;
    }
}

void DevicePool::createRS232Device(QString deviceName, deviceTypes dev, driverTypes driverType) {
    AudioLogic *aLogic;
    AudioDevice *aDev;
    AudioConfigurator audioConfig;
    aLogic = new AudioLogic();

    switch(dev) {
    case eAmplifier:
        // Create audio device, <deviceName> <Device type> <Control type>
        qDebug() << "DevicePool::createRS232Device() Amplifier";
        switch (driverType) {
            case eDriverPJLink:
            break;
        case eDriverPanasonic:
            break;
        case eDriverHitachi:
            break;
        case eDriverSony:
            break;
        case eDriverOnkyo:
            qDebug() << "DevicePool::createRS232Device() Amplifier, Onkyo";
            aDev = audioConfig.createAudioConfiguration(deviceName, AudioConfigurator::eAudioAmplifierOnkyo, AudioConfigurator::eAudioRS232, aLogic);
            break;
        }

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
        aDev = audioConfig.createAudioConfiguration(deviceName, AudioConfigurator::eAudioAmplifierGeneric, AudioConfigurator::eAudioIR, aLogic );
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
