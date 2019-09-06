#ifndef OSMC_H
#define OSMC_H

#include <QObject>
#include <QJsonDocument>
#include <QNetworkReply>

class OSMC : public QObject
{
    Q_OBJECT
public:
    explicit OSMC(QObject *parent = nullptr);
    // Sets volume to wanted value
    void setVolume(int vol);
    // Increases volume with one step
    void setVolumeUp();
    // Deacreases volume with one step
    void setVolumeDown();
    // Sets OSMC volume mute state
    void setVolumeMute(bool choice);
    // Set OSMC address and port
    void setAddress(QString address, qint16 port);
    // Returns OSMC current volume
    int getVolume();
signals:

public slots:
    void reply(QNetworkReply *rep);
private:
    // Send http post
    void sendCommand(QJsonDocument *doc);

    QString instanceAddress;
    qint16 instancePort;
    // Stores OSMC current volume
    int currentVolume;
    QNetworkAccessManager *networkManager;


};

#endif // OSMC_H
