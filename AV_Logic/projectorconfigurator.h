#ifndef PROJECTORCONFIGURATOR_H
#define PROJECTORCONFIGURATOR_H

// Qt Includes
#include <QObject>

// Project Includes
#include "pjlink.h"
#include "displaydevice.h"

class ProjectorConfigurator : public QObject
{
public:
    enum ProjectControlTypes {
        ePanasonic,
        ePJLink,
        eRS232,
        eVISCA_LAN,
        eVISCA_RS232
    };

    ProjectorConfigurator(QObject *parent = nullptr);
    void setLanConfiguration(QString ipAddress, quint16 port);
    void setAuthorization(QString user, QString password);
    DisplayDevice *createProjectorConfiguration(QString deviceName, ProjectControlTypes type, QObject *connectTo);
    void clearConfiguration();
    void setName(QString name);

private:
    bool lanConfigurationFlag;
    bool authConfigurationFlag;

    QString instanceAddress;
    quint16 instancePort;
    QString instanceUser;
    QString instancePassword;

    QString instanceName;
};

#endif // PROJECTORCONFIGURATOR_H
