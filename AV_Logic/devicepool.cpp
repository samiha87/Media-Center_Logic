#include "devicepool.h"

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
    QObject *object;
    switch(dev) {
    case eAmplifier:
        break;
    case eDisplay:
        break;
    case eProjector:
        // Device name determines which type of control
        object = new ProjectorConfigurator();
        qobject_cast<ProjectorConfigurator *>(object)->setLanConfiguration("10.42.0.100", 0);
        qobject_cast<ProjectorConfigurator *>(object)->setAuthorization("", "5233");
        qobject_cast<ProjectorConfigurator *>(object)->setName(deviceName);
        displayDevices.append(object);
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
    QObject *object;

    switch(dev) {
    case eAmplifier:
         object = new Amplifier(this);
         qobject_cast<Amplifier *>(object)->setDeviceName(deviceName);
         audioDevices.append(object);
        break;
    case eDisplay:
        break;
    case eProjector:
        break;
    case eLights:
        break;
    }
}
