#ifndef AMPLIFIER_H
#define AMPLIFIER_H
// Qt Includes
#include <QObject>
// Project includes
#include "irdevice.h"
#include "AudioDevice.h"

class Amplifier : public QObject, public AudioDevice
{
    Q_OBJECT
public:
    explicit Amplifier(QObject *parent = nullptr);
    void setName(QString name) override;
    QString getName() override;
    void setPower(bool state) override;
    void togglePower() override;
    bool getPower() override;
    void setMute(bool state) override;
    void setInput(AudioChannels channel) override;
    void volUp() override;
    void volDown() override;
    void setVol(int vol) override;
    void toggleMute() override;

    void setIR(IRDevice *ir);
signals:
    virtual void statusChanged(QByteArray msg) override;

signals:

public slots:
private:
    IRDevice *irDevice;
};

#endif // AMPLIFIER_H
