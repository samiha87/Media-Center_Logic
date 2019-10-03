#include "projectorconfigurator.h"

ProjectorConfigurator::ProjectorConfigurator(QObject *parent) : QObject(parent)
{
    lanConfigurationFlag = false;
    authConfigurationFlag = false;

    instanceAddress = "";
    instancePort = 0;
    instanceUser = "";
    instancePassword = "";
}

void ProjectorConfigurator::setLanConfiguration(QString ipAddress, quint16 port) {
    lanConfigurationFlag = true;
    instanceAddress = ipAddress;
    if(port == 0 ) return;
    instancePort = port;
}

void ProjectorConfigurator::setAuthorization(QString user, QString password) {
    authConfigurationFlag = true;
    instanceUser = user;
    instancePassword = password;
}

void ProjectorConfigurator::clearConfiguration() {
    lanConfigurationFlag = false;
    authConfigurationFlag = false;

    instanceAddress = "";
    instancePort = 0x00;
    instanceUser = "";
    instancePassword = "";
}
// Configure object here and return configured object to factory
// Factory is only allowed to acces common functions
DisplayDevice *ProjectorConfigurator::createProjectorConfiguration(QString deviceName, ProjectControlTypes type, QObject *connectTo) {
    PJLink *object;
    switch (type) {
    case (ePanasonic):
        break;
    case (ePJLink):
        if(!lanConfigurationFlag) return nullptr;
        object = new PJLink();
        if(instancePort > 0) qobject_cast<PJLink *>(object)->setPort(instancePort);
        qobject_cast<PJLink *>(object)->setAddress(instanceAddress);
        if(authConfigurationFlag) qobject_cast<PJLink *>(object)->setPassword(instancePassword);
        clearConfiguration();
        QObject::connect(qobject_cast<PJLink *>(object), SIGNAL(statusChanged(QByteArray)), connectTo, SLOT(statusChanged(QByteArray)));
        // Connect to upper classes
        return static_cast<DisplayDevice *>(object);
    case (eRS232):
        break;
    case (eVISCA_LAN):
        break;
    case (eVISCA_RS232):
        break;
    }
    return nullptr;
}

void ProjectorConfigurator::setName(QString name) {
    instanceName = name;
}
