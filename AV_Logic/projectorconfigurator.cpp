#include "projectorconfigurator.h"
#include "displaylogic.h"

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
    qDebug() << "ProjectorcConfigurator::createProjectorConfiguration()";
    PJLink *object;
    HITACHI_CP_WU8450 *hobject;
    switch (type) {
    case (ePanasonic):
        break;
    case (ePJLink):
        if(!lanConfigurationFlag) {
            qDebug() << "ProjectorcConfigurator::createProjectorConfiguration() Lan not configured";
            return nullptr;
        }
        object = new PJLink();
        if(instancePort > 0) qobject_cast<PJLink *>(object)->setPort(instancePort);
        qobject_cast<PJLink *>(object)->setAddress(instanceAddress);
        if(authConfigurationFlag) qobject_cast<PJLink *>(object)->setPassword(instancePassword);
        clearConfiguration();
        qobject_cast<DisplayLogic *>(connectTo)->setDriver(object);
        QObject::connect(qobject_cast<PJLink *>(object), SIGNAL(statusChanged(QByteArray)), connectTo, SLOT(statusChanged(QByteArray)));
        // Connect to upper classes
         qDebug() << "ProjectorcConfigurator::createProjectorConfiguration() Returning PJLink device";
        return static_cast<DisplayDevice *>(object);
    case (eHitachi):
        if(!lanConfigurationFlag) {
            qDebug() << "ProjectorcConfigurator::createProjectorConfiguration() Lan not configured";
            return nullptr;
        }
        hobject = new HITACHI_CP_WU8450();
        if(instancePort > 0) hobject->setPort(instancePort);
        hobject->setAddress(instanceAddress);
        if(authConfigurationFlag) {
            hobject->setAutheticationRequired(true);
            hobject->setPassword(instancePassword);
        }
        clearConfiguration();
        qobject_cast<DisplayLogic *>(connectTo)->setDriver(hobject);
        QObject::connect(hobject, SIGNAL(statusChanged(QByteArray)), connectTo, SLOT(statusChanged(QByteArray)));
        // Connect to upper classes
         qDebug() << "ProjectorcConfigurator::createProjectorConfiguration() Returning Hitachi device";
        return static_cast<DisplayDevice *>(hobject);
    case (eRS232):
        break;
    case (eVISCA_LAN):
        break;
    case (eVISCA_RS232):
        break;
    }
     qDebug() << "ProjectorcConfigurator::createProjectorConfiguration() Failed, returning nullptr";
    return nullptr;
}

void ProjectorConfigurator::setName(QString name) {
    instanceName = name;
}
