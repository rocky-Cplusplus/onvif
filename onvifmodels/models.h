#ifndef ONVIFMODEL_H
#define ONVIFMODEL_H

#include <QString>

class DeviceCapabilities {
public:
    QString mediaXAddr;
    QString ptzXAddr;
};

class MediaProfile {
public:
    QString name;
    QString token;
};

#endif // ONVIFMODEL_H
