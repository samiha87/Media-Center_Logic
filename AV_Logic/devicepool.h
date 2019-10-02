#ifndef DEVICEPOOL_H
#define DEVICEPOOL_H
/* DevicePool class stores all available devices for specific AV type
 * For example we store here video devices, and through videodevice This creates correct setup
 * If for examble we build audio device or change audio device, we need to delete old object and create a new one from available audio devices
 * This will allows managed object creation
 */
#include <QObject>
#include "amplifier.h"
#include "projectorconfigurator.h"

class DevicePool : public QObject
{
    enum deviceControlType {
        InfraRed,
        RS232,
        Ethernet
    };

    enum deviceTypes{
        eAmplifier,
        eDisplay,
        eProjector,
        eLights,
    };

    Q_OBJECT
    void createDevice(QString deviceName, deviceControlType control, deviceTypes dev);
public:
    explicit DevicePool(QObject *parent = nullptr);
    QList<QObject *> audioDevices;
    QList<QObject *> displayDevices;
    QList<QObject *> lightDevices;

signals:

private:
    void createLanDevice(QString deviceName, deviceTypes dev);
    void createRS232Device(QString deviceName, deviceTypes dev);
    void createIRDevice(QString deviceName, deviceTypes dev);
public slots:
};

#endif // DEVICEPOOL_H
