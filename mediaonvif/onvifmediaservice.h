#ifndef ONVIFMEDIASERVICE_H
#define ONVIFMEDIASERVICE_H

#include <QList>
#include <onvifmodels/models.h>


class OnvifMediaService
{
private:
    QList<QString> mediaNameSpace;
public:
    OnvifMediaService();

    QList<MediaProfile> GetProfiles(QString mediaXAddress, QString username, QString password);
};

#endif // ONVIFMEDIASERVICE_H
