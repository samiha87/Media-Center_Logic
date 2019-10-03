#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H
// Qt Includes
#include <QObject>
class AudioDevice
{
public:
    enum AudioChannels {
       Channel_CD,
       Channel_DVD,
       Channel_NONE
   };
    virtual void setName(QString name) = 0;
    virtual QString getName() = 0;
    virtual void setPower(bool state) = 0;
    virtual void togglePower() = 0;
    virtual void setMute(bool state) = 0;
    virtual void setInput(AudioChannels channel) = 0;
    virtual void volUp() = 0;
    virtual void volDown() = 0;
    virtual void setVol(int vol) = 0;
    virtual void toggleMute() = 0;

signals:
    virtual void statusChanged(QByteArray msg) = 0;
protected:
    QString deviceName;

};
#endif // AUDIODEVICE_H
