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
    void setVolumeUp();
    void setVolumeDown();
    void sendCommand(QJsonDocument *doc);
    void setAddress(QString address, qint16 port);
signals:

public slots:
    void reply(QNetworkReply *rep);
private:
    QString instanceAddress;
    qint16 instancePort;
};

#endif // OSMC_H
