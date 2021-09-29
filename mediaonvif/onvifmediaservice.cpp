#include "onvifmediaservice.h"

#include <soap/soaprequest.h>

#include <QDomDocument>

OnvifMediaService::OnvifMediaService()
{
    this->mediaNameSpace.push_back("xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\"");
    this->mediaNameSpace.push_back("xmlns:tt=\"http://www.onvif.org/ver10/schema\"");
}

QList<MediaProfile> OnvifMediaService::GetProfiles(QString mediaXAddress, QString username, QString password)
{
    SoapRequest *soapRequest = new SoapRequest();
    soapRequest->xAddress = mediaXAddress;
    soapRequest->username = username;
    soapRequest->password = password;
    soapRequest->XMLNs = this->mediaNameSpace;
    QString body("<trt:GetProfiles/>");
    soapRequest->body = body;
    QString response;
    bool ok = soapRequest->sendRequest(response);
    qInfo() << "[OnvifMediaService] GetProfiles Response " << response;
    delete soapRequest;

    QList<MediaProfile> result;
    if (ok) {
        QDomDocument doc;
        doc.setContent(response);
        QDomElement rootElement = doc.documentElement();
        for(QDomNode node = rootElement.firstChild(); !node.isNull(); node = node.nextSibling())
        {
            QDomElement elementBody = node.toElement();
            for(QDomNode node1 = elementBody.firstChild(); !node1.isNull(); node1 = node1.nextSibling())
            {
                QDomElement elementGetProfilesResponse = node1.toElement();
                for(QDomNode node2 = elementGetProfilesResponse.firstChild(); !node2.isNull(); node2 = node2.nextSibling())
                {
                    MediaProfile profile;
                    QDomElement elementProfiles= node2.toElement();
                    for (int i = 0; i < node2.attributes().size(); i++)
                    {
                        if (node2.attributes().item(i).nodeName() == "token") {
                            profile.token = node2.attributes().item(i).nodeValue();
                        }
                    }
                    for(QDomNode node3 = elementProfiles.firstChild(); !node3.isNull(); node3 = node3.nextSibling())
                    {
                        if (node3.nodeName().toLower().endsWith("name")) {
                            profile.name = node3.toElement().text();
                        }
                    }
                    result.push_back(profile);
                }
            }
        }
    }
    return result;
}
