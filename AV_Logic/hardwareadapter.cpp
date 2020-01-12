#include "hardwareadapter.h"
#include <QProcess>
#include <QDebug>
#include <QCoreApplication>

HardwareAdapter::HardwareAdapter(QObject *parent) : QObject(parent)
{
    tcp = new TCPSocket(this);
    startHardwareLayer();   //Start python layer
    process = nullptr;
    // Connect socket with harware reader slot
    QObject::connect(tcp, SIGNAL(response(QByteArray)), this, SLOT(msgFromHardware(QByteArray)));
}

HardwareAdapter::~HardwareAdapter() {
    stopHardwareLayer();
}

void HardwareAdapter::msgFromHardware(QByteArray msg) {
    // Parse mesage
    if (!msg.contains('#') || !msg.contains('*')) {
        qDebug() << "HardwareAdapter::msgFromHardware() Invalid msg";
        return;
    }

    // Check message type
    if(msg.contains("BLE")) {
        qDebug() << "HardwareAdapter::msgFromHardware() Bluetooth message received";
        qDebug() << "Message: " << msg;
        msg = msg.remove(0, 4); // Remove #BLE from message
        msg.chop(1);    // Remove end byte * from array
        qDebug() << "HardwareAdapter::msgFromHardware(), Bluetooth sending " << msg;
        emit bleMessageRx(msg);
    }

    // Check message type
    if(msg.contains("TCP")) {
        qDebug() << "HardwareAdapter::msgFromHardware() TCP message received";
        qDebug() << "Message: " << msg;
        msg = msg.remove(0, 4); // Remove #TCP from message
        msg.chop(1);    // Remove end byte * from array
        qDebug() << "HardwareAdapter::msgFromHardware(), TCP sending " << msg;
        emit tcpMessageRx(msg);
    }
}

void HardwareAdapter::hardwareTx(QByteArray msg) {
    tcp->send(msg);
}

// Start a python based hardware layer -> This is not best way to do this
void HardwareAdapter::startHardwareLayer()
{
    stopHardwareLayer();
    QString path = "/opt/hardwarehandler/";
    tcp->enableReconnect(10000);
    qDebug() << "HardwareAdapter::startHardwareLayer() " << path;
    QString  command("python");
    QStringList params = QStringList() << "/opt/hardwarehandler/HardwareHandler.py";
    process = new QProcess(this);
    qDebug() << "HardwareAdapter::starting QProcess";
    process->startDetached(command, params, path, nullptr);
    process->waitForReadyRead(30000);
    qDebug() << "HardwareAdapter::startHardwareLayer() " << process->readAll();
    // Connect to hardware layer
    process->close();
    process->deleteLater();
    qDebug() << "HardwareAdapter::startHardwareLayer() Connecting to hardwarelayer";
    tcp->connect("127.0.0.1", 10000);

}

void HardwareAdapter::stopHardwareLayer()
{
    QProcess killHW;
    qDebug() << "HardwareAdapter::stopHardwarelayer()";
    killHW.start("sh", QStringList()<< "-c" << "kill -9 $(ps aux | grep hardwarehandler | awk '{print $2}')");
    if(!killHW.waitForStarted()) qDebug() << "HardwareAdapter::stopHardwareLayer() Failed to open process";
    killHW.waitForFinished();
    killHW.close();


}
