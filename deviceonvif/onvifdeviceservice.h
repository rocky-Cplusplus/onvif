#ifndef ONVIFDEVICESERVICE_H
#define ONVIFDEVICESERVICE_H

#include <QList>
#include <onvifmodels/models.h>


class OnvifDeviceService
{
private:
    QList<QString> deviceNameSpace;
public:
    OnvifDeviceService();
    DeviceCapabilities GetCapabilities(QString deviceXAddress, QString username, QString password);

};

#endif // ONVIFDEVICESERVICE_H
