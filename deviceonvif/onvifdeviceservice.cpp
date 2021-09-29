#include "onvifdeviceservice.h"

#include <soap/soaprequest.h>
#include <QDomDocument>

OnvifDeviceService::OnvifDeviceService()
{
    this->deviceNameSpace.push_back("xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\"");
    this->deviceNameSpace.push_back("xmlns:tt=\"http://www.onvif.org/ver10/schema\"");
}

DeviceCapabilities OnvifDeviceService::GetCapabilities(QString deviceXAddress, QString username, QString password)
{
    SoapRequest *soapRequest = new SoapRequest();
    soapRequest->xAddress = deviceXAddress;
    soapRequest->username = username;
    soapRequest->password = password;
    soapRequest->XMLNs = this->deviceNameSpace;
    QString body("<tds:GetCapabilities><tds:Category>All</tds:Category></tds:GetCapabilities>");
    soapRequest->body = body;
    QString response;
    bool ok = soapRequest->sendRequest(response);
    qInfo() << "[OnvifDeviceService] GetPTZXAddress Response " << response;
    delete soapRequest;

    DeviceCapabilities result;
    if (ok)
    {
        QDomDocument doc;
        doc.setContent(response);
        QDomElement rootElement = doc.documentElement();
        for(QDomNode node = rootElement.firstChild(); !node.isNull(); node = node.nextSibling())
        {
            QDomElement elementBody = node.toElement();
            for(QDomNode node1 = elementBody.firstChild(); !node1.isNull(); node1 = node1.nextSibling())
            {
                QDomElement elementGetCapabilitiesResponse = node1.toElement();
                for(QDomNode node2 = elementGetCapabilitiesResponse.firstChild(); !node2.isNull(); node2 = node2.nextSibling())
                {
                    QDomElement elementCapabilities = node2.toElement();
                    for(QDomNode node3 = elementCapabilities.firstChild(); !node3.isNull(); node3 = node3.nextSibling())
                    {
                        if (node3.nodeName().toLower().endsWith("ptz"))
                        {
                            QDomElement elementPtz = node3.toElement();
                            for(QDomNode node4 = elementPtz.firstChild(); !node4.isNull(); node4 = node4.nextSibling())
                            {
                                if (node4.nodeName().toLower().endsWith("xaddr"))
                                {
                                    result.ptzXAddr = node4.toElement().text();
                                }
                            }
                        }
                        else if (node3.nodeName().toLower().endsWith("media"))
                        {
                            QDomElement elementPtz = node3.toElement();
                            for(QDomNode node4 = elementPtz.firstChild(); !node4.isNull(); node4 = node4.nextSibling())
                            {
                                if (node4.nodeName().toLower().endsWith("xaddr"))
                                {
                                    result.mediaXAddr = node4.toElement().text();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}
