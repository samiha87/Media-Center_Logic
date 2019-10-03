#ifndef AUDIOCONFIGURATOR_H
#define AUDIOCONFIGURATOR_H
// Qt Includes
#include <QObject>
// Project Includes
#include <AudioDevice.h>
#include "amplifier.h"

class AudioConfigurator : public QObject
{
public:
    enum AudioControlTypes {
        eAudioIR,
        eAudioLAN
    };

    enum AudioDeviceTypes {
        eAudioAmplifier,
        eAudioTV,
        eAudioProjector,
        eAudioAmplifierMicrophones,
        eAudioAmplifierDSP
    };

    AudioConfigurator(QObject *parent = nullptr);
    void setLanConfiguration(QString ipAddress, quint16 port);
    void setAuthorization(QString user, QString password);
    AudioDevice *createAudioConfiguration(QString deviceName, AudioDeviceTypes type, AudioControlTypes control, QObject *connectTo);
    void clearConfiguration();
    void setName(QString name);
signals:

public slots:
private:
    AudioDevice *configureAmplifier(Amplifier *amp, AudioControlTypes control);
    bool lanConfigurationFlag;
    bool authConfigurationFlag;

    QString instanceAddress;
    quint16 instancePort;
    QString instanceUser;
    QString instancePassword;

    QString instanceName;
};

#endif // AUDIOCONFIGURATOR_H
