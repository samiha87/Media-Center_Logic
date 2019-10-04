#ifndef HITACHI_CP_WU8450_H
#define HITACHI_CP_WU8450_H
/* Hitachi can be driven from ports 23 and 9715
 * Authentication on port 23 default disabled, on 9715 it's enabled
 * Connection will stay open max 30 seconds
 */
#include <QObject>
#include <QTimer>

#include "tcpdevice.h"
#include "displaydevice.h"
#include "tcpsocket.h"
#define HITACHI_CONTROL_PORT_1 23
#define HITACHI_CONTROL_PORT_2 9715


class HITACHI_CP_WU8450 : public QObject, public TCPDevice, public DisplayDevice
{
    Q_OBJECT
public:
    enum HITACHI_RESPONSE_CODES {
        HITACHI_ACK_REPLY,
        HITACHI_NAK_REPLY,
        HITACHI_ERROR_REPLY,
        HITACHI_DATA_REPLY,
        HITACHI_AUTHENTICATION_ERROR_REPLY,
        HITACHI_PROJECTOR_BUSY
    };
    explicit HITACHI_CP_WU8450(QObject *parent = nullptr);
    void setPower(bool state) override;
    // Set video mute, true mute on, false mute off
    void setVideoMute(bool state) override;
    // Set audio mute, true mute on, false mute off
    void setInput(DisplayChannels input) override;
    // Set projector ip address
    void setAddress(QString ip) override;
    void setUser(QString user) override;
    // Set PJLink password
    void setPassword(QString pass) override;
    // Set PJLink port address, default is 4352
    void setPort(quint16 port_) override;
    void setName(QString name) override;
    QString getName() override;

    // Audio
    void volUp() override;
    void volDown() override;
    void toggleMute() override;

    void loadPreset(DisplayPreset preset) override;

    void setAutheticationRequired(bool state);
signals:
    void statusChanged(QByteArray msg) override;
public slots:
    void response(QByteArray msg);
    void connectedToHost();
    void requestStatus();

private:
    enum HITACHI_COMMANDS {
        HITACHI_SET_PWR_ON,
        HITACHI_SET_PWR_OFF,
        HITACHI_REQUEST_PWR,
        HITACHI_LOAD_LENS_1,
        HITACHI_LOAD_LENS_2,
        HITACHI_LOAD_LENS_3,
        HITACHI_SET_VOL_UP,
        HITACHI_SET_VOL_DOWN,
        HITACHI_SET_VOL_MUTE_ON,
        HITACHI_SET_VOL_MUTE_OFF,
        HITACHI_SET_BLANK_ON,
        HITACHI_SET_BLANK_OFF,
        HITACHI_REQUEST_LAMP_STATUS_LOW,
        HITACHI_REQUEST_LAMP_STATUS_HIGH,
        HITACHI_REQUEST_FILTER_STATUS_LOW,
        HITACHI_REQUEST_FILTER_STATUS_HIGH,
        HITACHI_SET_INPUT_COMPUTER1,
        HITACHI_SET_INPUT_COMPUTER2,
        HITACHI_SET_INPUT_COMPONENT,
        HITACHI_SET_INPUT_S_VIDEO,
        HITACHI_SET_INPUT_VIDEO,
        HITACHI_SET_INPUT_HDMI1,
        HITACHI_SET_INPUT_HDMI2,
        HITACHI_SET_INPUT_USB_TYPE_A,
        HITACHI_SET_INPUT_USB_TYPE_B,
        HITACHI_SET_INPUT_LAN,
        HITACHI_REQUEST_INPUT,
        HITACHI_REQUEST_VOLUME,
        HITACHI_REQUEST_VOLUME_MUTE
    };
    void sendCommand(HITACHI_COMMANDS cmd);
    void send(QByteArray msg);
    void requestPwr();
    void requestInput();
    void requestLampHigh();
    void requestLampLow();
    void requestFilterHigh();
    void requestFilterLow();
    void requestVol();
    void requestVolMute();

    bool authenticationRequired;
    QString ipAddress;
    QString password;
    QByteArray pendingCommand;
    QString userName;
    QTimer *timer;
    QTimer *connectionTimer;

    bool connected;
    quint16 port;
    TCPSocket *sock;

    int lampHours;
    bool powerState;
    int requestPoll = 0;
    int projVolume = 0;
    bool projVolumeMute;
    int projInput = 0;
    int projLamp = 0;
    bool requestedPowerState;

    QByteArray msg;
    HITACHI_COMMANDS latestRequest;
    int volIncreasePoll;
    int volDecreasePoll;
};

#endif // HITACHI_CP_WU8450_H
