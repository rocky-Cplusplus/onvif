#include "onvifptzservice.h"

#include <soap/soaprequest.h>

OnvifPTZService::OnvifPTZService()
{
    this->ptzNameSpace.push_back("xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\"");
    this->ptzNameSpace.push_back("xmlns:d=\"http://www.w3.org/2001/XMLSchema\"");
    this->ptzNameSpace.push_back("xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\"");
}

bool OnvifPTZService::ContinuousMove(QString ptzXAddress, QString username, QString password, QString profile, double x, double y, double z)
{
    SoapRequest *soapRequest = new SoapRequest();
    soapRequest->xAddress = ptzXAddress;
    soapRequest->username = username;
    soapRequest->password = password;
    soapRequest->action = "http://www.onvif.org/ver20/ptz/wsdl/ContinuousMove";
    soapRequest->XMLNs = this->ptzNameSpace;
    QString body("<ContinuousMove xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile);
    body.push_back("</ProfileToken>");
    body.push_back("<Velocity>");
    body.push_back("<PanTilt xmlns=\"http://www.onvif.org/ver10/schema\" x=\"" + QString::number(x) + "\" y=\""+ QString::number(y) +"\"/>");
    body.push_back("<Zoom xmlns=\"http://www.onvif.org/ver10/schema\" x=\"" + QString::number(z) + "\"/>");
    body.push_back("</Velocity></ContinuousMove>");
    soapRequest->body = body;
    QString response;
    bool result = soapRequest->sendRequest(response);
    qInfo() << "[OnvifPTZService] ContinuousMove Response " << response;
    delete soapRequest;
    return result;
}

bool OnvifPTZService::Stop(QString ptzXAddress, QString username, QString password, QString profile)
{
    SoapRequest *soapRequest = new SoapRequest();
    soapRequest->xAddress = ptzXAddress;
    soapRequest->username = username;
    soapRequest->password = password;
    soapRequest->action = "http://www.onvif.org/ver20/ptz/wsdl/Stop";
    soapRequest->XMLNs = this->ptzNameSpace;
    QString body("<Stop xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile);
    body.push_back("</ProfileToken>");
    body.push_back("<PanTilt>true</PanTilt><Zoom>true</Zoom>");
    body.push_back("</Stop>");
    soapRequest->body = body;
    QString response;
    bool result = soapRequest->sendRequest(response);
    qInfo() << "[OnvifPTZService] Stop Response " << response;
    delete soapRequest;
    return result;
}

bool OnvifPTZService::GoToHomePosition(QString ptzXAddress, QString username, QString password, QString profile)
{
    SoapRequest *soapRequest = new SoapRequest();
    soapRequest->xAddress = ptzXAddress;
    soapRequest->username = username;
    soapRequest->password = password;
    soapRequest->action = "http://www.onvif.org/ver20/ptz/wsdl/GotoHomePosition";
    soapRequest->XMLNs = this->ptzNameSpace;
    QString body("<GotoHomePosition xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile);
    body.push_back("</ProfileToken>");
    body.push_back("</GotoHomePosition>");
    soapRequest->body = body;
    QString response;
    bool result = soapRequest->sendRequest(response);
    qInfo() << "[OnvifPTZService] GoToHomePosition Response " << response;
    delete soapRequest;
    return result;
}

bool OnvifPTZService::setPreset(QString ptzXAddress, QString username, QString password, QString profile)
{
    SoapRequest *soapRequest = new SoapRequest();
    soapRequest->xAddress = ptzXAddress;
    soapRequest->username = username;
    soapRequest->password = password;
    soapRequest->action = "http://www.onvif.org/ver20/ptz/wsdl/SetPreset";
    soapRequest->XMLNs = this->ptzNameSpace;
    QString body("<SetPreset xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile);
    body.push_back("</ProfileToken>");
//    body.push_back("<PresetName>");
//    body.push_back("qwe");
//    body.push_back("</PresetName>");
//    body.push_back("<PresetToken>");
//    body.push_back("123");
//    body.push_back("</PresetToken>");
    body.push_back("</SetPreset>");
    soapRequest->body = body;
    QString response;
    bool result = soapRequest->sendRequest(response);
    qInfo() << "[OnvifPTZService] SetPreset Response " << response;
    delete soapRequest;
    return result;
}

bool OnvifPTZService::gotoPreset(QString ptzXAddress, QString username, QString password, QString profile)
{
    SoapRequest *soapRequest = new SoapRequest();
    soapRequest->xAddress = ptzXAddress;
    soapRequest->username = username;
    soapRequest->password = password;
    soapRequest->action = "http://www.onvif.org/ver20/ptz/wsdl/GotoPreset";
    soapRequest->XMLNs = this->ptzNameSpace;
    QString body("<GotoPreset xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile);
    body.push_back("</ProfileToken>");
    body.push_back("<PresetToken>");
    body.push_back("0");
    body.push_back("</PresetToken>");
    body.push_back("</GotoPreset>");
    soapRequest->body = body;
    QString response;
    bool result = soapRequest->sendRequest(response);
    qInfo() << "[OnvifPTZService] gotoPreset Response " << response;
    delete soapRequest;
    return result;
}


bool OnvifPTZService::getPreset(QString ptzXAddress, QString username, QString password, QString profile)
{
    SoapRequest *soapRequest = new SoapRequest();
    soapRequest->xAddress = ptzXAddress;
    soapRequest->username = username;
    soapRequest->password = password;
    soapRequest->action = "http://www.onvif.org/ver20/ptz/wsdl/GetPresets";
    soapRequest->XMLNs = this->ptzNameSpace;
    QString body("<GetPresets xmlns=\"http://www.onvif.org/ver20/ptz/wsdl\">");
    body.push_back("<ProfileToken>");
    body.push_back(profile);
    body.push_back("</ProfileToken>");
    body.push_back("</GetPresets>");
    soapRequest->body = body;
    QString response;
    bool result = soapRequest->sendRequest(response);
    qInfo() << "[OnvifPTZService] GetPresets Response " << response;
    delete soapRequest;
    return result;
}






















