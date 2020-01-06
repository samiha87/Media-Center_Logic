#ifndef ONKYO_AMP_RS232_H
#define ONKYO_AMP_RS232_H

#include <QObject>

#include "AudioDevice.h"

class Onkyo_AMP_RS232 : public QObject, public AudioDevice
{
    Q_OBJECT
public:
    explicit Onkyo_AMP_RS232(QObject *parent = nullptr);

    void setName(QString name) override;
    QString getName() override;

    void setPower(bool state) override;
    void togglePower() override;
    bool getPower() override;
    void setMute(bool state) override;
    void toggleMute() override;
    void setInput(AudioChannels channel) override;
    void volUp() override;
    void volDown() override;
    void setVol(int vol) override;

signals:
    void statusChanged(QByteArray msg) override;
};

#endif // ONKYO_TX_NR_900_H
