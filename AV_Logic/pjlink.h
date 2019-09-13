#ifndef PJLINK_H
#define PJLINK_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QByteArray>
#include <QTimer>

#include "tcpsocket.h"

#define PJLINK_PORT 4352
#define PJLINK_CLASS '1'

/* Authentication test on ubuntu
 * 1. Download packet sender
 * 2. Connect to PJLink projector
 * 3. send packet with persistent connection
 * 4. PJlink will reply with PJLINK <class> <random number> <CR>
 * 5. Create MD5 hash of wth the random number and password.
 *    https://www.md5hashgenerator.com/
 *    MD5( (random number) + your password).
 *    The hashed string is constructed as follows <random_numer><your_password>
 * 6. Send the hashed string and command in same packet to Projector
 * 7. Projetor response random response, if PJLINK ERRA is returend. Hashed string was wrong.
 * Example, Password 1323, command %1POWR 1
 * Connect
 * ResponsePJLINK 1 00003a9e
 * MD5 hash(00003a9e1323)
   Send: 3f89cff497594c407e98ab892561b87a%1POWR 1\r
   Response %1POWR=ERR3
   Responded error because projector was already on
*/

class PJLink : public QObject
{
    Q_OBJECT

    enum PJLINK_Response_codes {
        Successfull_execution,
        Undefinied_command,
        Out_of_parameter,
        Unavailable_time,
        Projector_failure
    };

    enum Projector_Channels {
        Channel_RGB,
        Channel_VIDEO,
        Channel_DIGITAL,
        Channel_STORAGE,
        Channel_NETWORK
    };

public:
    explicit PJLink(QObject *parent = nullptr);
    // Set powerstate, true = on, false = off
    void setPower(bool state);
    // Set video mute, true mute on, false mute off
    void setVideoMute(bool state);
    // Set audio mute, true mute on, false mute off
    void setAudioMute(bool state);
    // Set audio and video mute. ture mute on, false mute off
    void setAVMute(bool state);
    // Select projected input, choices in enum
    void setInput(Projector_Channels input);
    // Set projector ip address
    void setIpAddress(QString ip);
    // Return current ip address
    QString getIpAddress();
    // Set PJLink password
    void setPassword(QString pass);
    // Return PJLink password
    QString getPassword();
    // Set PJLink port address, default is 4352
    void setPort(quint16 port_);
    // Return PJLink port address
    quint16 getPort();
signals:
    void projectorStatus(QByteArray msg);
public slots:
    void response(QByteArray msg);
    void requestStatusPwr();
    void requestStatus();
    void requestInput();
    void requestAVMute();
    void requestLamp();
private:
    QString ipAddress;
    QString password;
    QString pendingCommand;
    QTimer *timer;

    bool connected;
    quint16 port;
    TCPSocket *sock;

    void sendCommand(QString cmd);

    QByteArray md5hash(QByteArray message);

    int lampHours;
    bool powerState;
    int requestPoll;
    int projVolume = 0;
    int projInput = 0;
    int projLamp = 0;
    bool requestedPowerState;
};

#endif // PJLINK_H
