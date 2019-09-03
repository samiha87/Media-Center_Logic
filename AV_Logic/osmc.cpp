#include "osmc.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QNetworkAccessManager>

OSMC::OSMC(QObject *parent) : QObject(parent)
{

}
void OSMC::setVolumeUp() {

}

void OSMC::setVolumeDown() {

}

void OSMC::sendCommand(QJsonDocument *doc) {

    QUrl serviceUrl = QUrl("https://" + instanceAddress + ":" + QString::number(instancePort));
    QNetworkRequest request(serviceUrl);
    QByteArray jsonData= doc->toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(jsonData.size()));
    QNetworkAccessManager networkManager;

    QObject::connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(reply(QNetworkReply*)));
    networkManager.post(request, jsonData);
}

void OSMC::setAddress(QString address, qint16 port) {
    instanceAddress = address;
    instancePort = port;
}

void OSMC::reply(QNetworkReply *rep)
{
    QString strReply = static_cast<QString>(rep->readAll());
    qDebug()<<"Test: "<<strReply;
}
