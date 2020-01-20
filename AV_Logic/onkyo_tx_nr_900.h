#ifndef ONKYO_AMP_RS232_H
#define ONKYO_AMP_RS232_H

#include <QObject>
#include <QTimer>

#include "AudioDevice.h"
#include "rs232device.h"

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
private:
    RS232Device serial;
    QTimer timer;
    QByteArray receiveBuffer;
    bool devicePower = false;
    bool deviceMute = false;
    int deviceVolume;

    int requestStatusPoll = 0;
    QByteArray generateCommand(QByteArray cmd, QByteArray parameter);
    void parseMessage(QByteArray data);
private slots:
    void fromDevice(QByteArray msg);
    void requestStatus();

};

#endif // ONKYO_TX_NR_900_H
