#ifndef DISPLAYDEVICE_H
#define DISPLAYDEVICE_H

// Qt Includes
#include <QObject>

class DisplayDevice
{
public:
    enum DisplayChannels {
        Channel_RGB,
        Channel_VIDEO,
        Channel_DIGITAL,
        Channel_STORAGE,
        Channel_NETWORK,
        Channel_HDMI
    };
    virtual void setPower(bool state) = 0;
    virtual void setVideoMute(bool state) = 0;
    virtual void setInput(DisplayChannels channel) = 0;
signals:
    virtual void statusChanged(QByteArray msg) = 0;
};

#endif // DISPLAYDEVICE_H
