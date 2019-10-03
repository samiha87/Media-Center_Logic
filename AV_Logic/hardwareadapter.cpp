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
        qDebug() << "HardwareAdapter::msgFromHardware() sending " << msg;
        emit bleMessageRx(msg);
    }
}

void HardwareAdapter::hardwareTx(QByteArray msg) {
    tcp->send(msg);
}

// Start a python based hardware layer -> This is not best way to do this
void HardwareAdapter::startHardwareLayer() {
  /*  QString path = "/opt/hardwarelayer/";
    qDebug() << "HardwareAdapter::startHardwareLayer() " << path;
    QString  command("python");
    QStringList params = QStringList() << "HardwareHandler.py";
    process = new QProcess(this);
    qDebug() << "HardwareAdapter::starting QProcess";
    process->startDetached(command, params, path, nullptr);
    process->waitForReadyRead(10000);
    qDebug() << "HardwareAdapter::startHardwareLayer() " << process->readAll();
    // Connect to hardware layer
    process->close();
    process->deleteLater(); */
    tcp->connect("localhost", 10000);
}

void HardwareAdapter::stopHardwareLayer() {
    QString path = "/opt/hardwarelayer/";
    qDebug() << "HardwareAdapter::stopHardwarelayer() ";
    QString  command("sh");
    QStringList params = QStringList() << "kill $(ps aux | grep '[p]ython csp_build.py' | awk '{print $2}')";
    process = new QProcess(this);
    qDebug() << "HardwareAdapter::starting QProcess";
    process->startDetached(command, params, path, nullptr);
    process->waitForFinished();
    qDebug() << "HardwareAdapter::startHardwareLayer() " << process->readAll();
    // Connect to hardware layer
    process->close(); // Kills the process
    process->deleteLater();
}
