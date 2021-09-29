#include "soaprequest.h"
#include <QDateTime>
#include <QEventLoop>
#include <QTimer>

SoapRequest::SoapRequest() : QObject (nullptr)
{
    networkManager = new QNetworkAccessManager();
}

SoapRequest::~SoapRequest()
{
    delete networkManager;
}

bool SoapRequest::sendRequest(QString &result)
{
    QNetworkRequest request(this->xAddress);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/soap+xml");

    QString concatenated = this->username + ":" + this->password;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    qInfo() << "[SoapRequest] Request onvif " << this->createRequest();

    //for digest authenticaton request
    QObject::connect(this->networkManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authRequired(QNetworkReply *, QAuthenticator *)));
    QNetworkReply * reply = this->networkManager->post(request, this->createRequest().toUtf8());

    QTimer timerTimeout;
    timerTimeout.setSingleShot(true);
    QEventLoop loop;
    loop.connect(&timerTimeout, SIGNAL(timeout()), SLOT(quit()));
    loop.connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), SLOT(quit()));

    timerTimeout.start(3000);
    loop.exec();

    if (timerTimeout.isActive())
    {
        timerTimeout.stop();
        if(reply->error() > 0)
        {
            qInfo() << "[SoapRequest] Request error " << reply->error();
        }
        else
        {
            QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            result = QString(reply->readAll());
            if (statusCodeV.toInt() == 200)
            {
                return true;
            }
        }
    } else
    {
       disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
       reply->abort();
       qInfo() << "[SoapRequest] Request timeout";
    }
    return false;
}

void SoapRequest::authRequired(QNetworkReply *, QAuthenticator *authenticator)
{
    authenticator->setUser(this->username);
    authenticator->setPassword(this->password);
}

QString SoapRequest::createRequest()
{
    QString request("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    request.push_back("<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\"");
    for (int i = 0; i < this->XMLNs.size(); i++)
    {
        request.push_back(" " + this->XMLNs[i]);
    }
    request.push_back(">");
    if (this->username != "" || this->action != "")
    {
        request.push_back("<s:Header>");
        if (this->action != "")
        {
            request.push_back("<Action mustUnderstand=\"1\" xmlns=\"http://www.w3.org/2005/08/addressing\">");
            request.push_back(this->action);
            request.push_back("</Action>");
        }

        if (this->username != "")
        {
            request.push_back(this->createUserToken());
        }
        request.push_back("</s:Header>");
    }
    request += "<s:Body>" + this->body + "</s:Body>";
    request += "</s:Envelope>";
    return request;
}

QString SoapRequest::createUserToken()
{
    QUuid nonce = QUuid::createUuid();
    QString nonce64 = nonce.toByteArray().toBase64();
    auto timestamp = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    auto token = nonce.toString() + timestamp + this->password;

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(token.toUtf8());
    QString hashTokenBase64 = hash.result().toBase64();

    QString result("<Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">");
    result.push_back("<UsernameToken><Username>");
    result.push_back(this->username);
    result.push_back("</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">");
    result.push_back(hashTokenBase64);
    result.push_back("</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">");
    result.push_back(nonce64);
    result.push_back("</Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">");
    result.push_back(timestamp);
    result.push_back("</Created></UsernameToken></Security>");
    return result;
}
