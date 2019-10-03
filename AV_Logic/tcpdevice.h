#ifndef TCPDEVICE_H
#define TCPDEVICE_H
#include<QString>

class TCPDevice
{
public:
    // Not necessary in every model
    virtual void setUser(QString user) = 0;
    virtual void setPassword(QString pass) = 0;
    // Not necessary in every model
    virtual void setPort(quint16 port) = 0;
    virtual void setAddress(QString address) = 0;
};

#endif // TCPDEVICE_H
