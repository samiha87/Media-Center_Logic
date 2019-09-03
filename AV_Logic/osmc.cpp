#include "osmc.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QNetworkAccessManager>

OSMC::OSMC(QObject *parent) : QObject(parent)
{
    // We set volume to 50
    currentVolume = 50;
}
// Volume control
// Volume example {"jsonrpc":"2.0","method":"Application.SetVolume","params":{"volume":60},"id":1}

void OSMC::setVolume(int vol) {
    currentVolume = vol;
    QJsonObject params;
    QJsonObject jObject;
    QJsonDocument jDocument;

    params.insert("volume",currentVolume);
    jObject.insert("jsonrpc", "2.0");   // OSMC RPC API version
    jObject.insert("method", "Application.SetVolume");
    jObject.insert("params", params);
    jObject.insert("id", 1);
    jDocument.setObject(jObject);
    qDebug() << "OSMC::setVolume() JSON= " << jDocument.toJson();
    sendCommand(&jDocument);
}
void OSMC::setVolumeUp() {
    currentVolume++;
    QJsonObject params;
    QJsonObject jObject;
    QJsonDocument jDocument;

    params.insert("volume",currentVolume);
    jObject.insert("jsonrpc", "2.0");   // OSMC RPC API version
    jObject.insert("method", "Application.SetVolume");
    jObject.insert("params", params);
    jObject.insert("id", 1);
    jDocument.setObject(jObject);
    qDebug() << "OSMC::setVolumeUp() JSON= " << jDocument.toJson();
    sendCommand(&jDocument);
}

void OSMC::setVolumeDown() {
    currentVolume++;
    QJsonObject params;
    QJsonObject jObject;
    QJsonDocument jDocument;

    params.insert("volume",currentVolume);
    jObject.insert("jsonrpc", "2.0");   // OSMC RPC API version
    jObject.insert("method", "Application.SetVolume");
    jObject.insert("params", params);
    jObject.insert("id", 1);
    jDocument.setObject(jObject);
    qDebug() << "OSMC::setVolumeDown() JSON= " << jDocument.toJson();
    sendCommand(&jDocument);
}

void OSMC::sendCommand(QJsonDocument *doc) {

    QUrl serviceUrl = QUrl("http://" + instanceAddress + ":" + QString::number(instancePort) + "/jsonrpc");
    qDebug() << "OSMC::sendCommand() URL " << serviceUrl;
    QNetworkRequest request(serviceUrl);
    QByteArray jsonData= doc->toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(jsonData.size()));
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
