#ifndef DISPLAYDEVICE_H
#define DISPLAYDEVICE_H

// Qt Includes
#include <QObject>

class DisplayDevice
{
public:
    enum DisplayChannels {
        Channel_HDMI_1,
        Channel_HDMI_2,
        Channel_COMPUTER_1,
        Channel_COMPUTER_2,
        Channel_VIDEO,
        Channel_COMPONENT,
        Channel_DIGITAL,
        Channel_STORAGE,
        Channel_LAN,
        Channel_USB_1,
        Channel_USB_2,
        Channel_S_VIDEO,
    };

    enum DisplayPreset {
        DISPLAY_PRESET_1,
        DISPLAY_PRESET_2,
        DISPLAY_PRESET_3
    };

    virtual void setPower(bool state) = 0;
    virtual void setVideoMute(bool state) = 0;
    virtual void setInput(DisplayChannels channel) = 0;
    virtual void setName(QString name) = 0;
    virtual QString getName() = 0;

    virtual void volUp() = 0;
    virtual void volDown() = 0;
    virtual void toggleMute() = 0;

    virtual void loadPreset(DisplayPreset preset) = 0;
signals:
    virtual void statusChanged(QByteArray msg) = 0;

protected:
    QString deviceName;
};
#endif // DISPLAYDEVICE_H
