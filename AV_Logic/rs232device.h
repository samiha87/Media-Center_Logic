#ifndef RS232DEVICE_H
#define RS232DEVICE_H

#include <QObject>
#include <QSerialPort>

class RS232Device : public QObject
{
    Q_OBJECT
public:
    explicit RS232Device(QObject *parent = nullptr);
    void setPort(QString rsPort);
    void setBaud(QSerialPort::BaudRate baudrate_);
    void setParity(QSerialPort::Parity parity_);
    void setStop(QSerialPort::StopBits stop_);
    void setDataBits(QSerialPort::DataBits databits_);
    void setFlowControl(QSerialPort::FlowControl flowControl_);
    bool openPort();
    bool closePort();

public slots:
    void send(QByteArray msg);
    void readData(QByteArray msg);
signals:
    void received(QByteArray msg);

private:
    void sendToDevice(const QByteArray &msg);
    QSerialPort::BaudRate baudrate;
    QSerialPort::Parity parityBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::DataBits dataBits;
    QSerialPort::FlowControl flowControl;
    QString portName;
    QSerialPort *serial = nullptr;

    bool connected = false;
};

#endif // RS232DEVICE_H
