#include "osmc.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QNetworkAccessManager>

OSMC::OSMC(QObject *parent) : QObject(parent)
{
    // We set volume to 50
    currentVolume = 50;
    networkManager = new QNetworkAccessManager();
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(reply(QNetworkReply*)));
}
// Volume control
// Volume example {"jsonrpc":"2.0","method":"Application.SetVolume","params":{"volume":60},"id":1}
//"{id: 1, jsonrpc: 2.0, method: Application.SetVolume, params: {volume: 95}}"    // Sent
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
    if(currentVolume++ > 99) currentVolume = 100;   // Vol max is 0, OSMC
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
    if(currentVolume-- < 1) currentVolume = 0;  // Vol min is 0, OSMC
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
    //QUrl serviceUrl = QUrl("127.0.0.1:4444");
    qDebug() << "OSMC::sendCommand() URL " << serviceUrl;
    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    networkManager->post(request, doc->toJson());
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
