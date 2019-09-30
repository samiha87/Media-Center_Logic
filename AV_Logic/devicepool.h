#ifndef DEVICEPOOL_H
#define DEVICEPOOL_H
/* DevicePool class stores all available devices for specific AV type
 * For example we store here video devices, and through videodevice This creates correct setup
 * If for examble we build audio device or change audio device, we need to delete old object and create a new one from available audio devices
 * This will allows managed object creation
 */
#include <QObject>

class DevicePool : public QObject
{
    Q_OBJECT
public:
    explicit DevicePool(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DEVICEPOOL_H
