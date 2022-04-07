#include "qdlnaclient.h"
#include "qdlnaxml.h"

#include <QMap>
#include <unistd.h>
#include <QStringList>

#define AVTRANSPORT "avtransport"

const QString XML_HEAD = "<?xml version=\"1.0\"?>\n<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n<SOAP-ENV:Body>\n";
const QString XML_FOOT = "</SOAP-ENV:Body>\n</SOAP-ENV:Envelope>\n";
const QStringList FRIENS_NAMES = {"friendlyname", "friendlyName", "FriendlyName", "FriendlyName"};
const QStringList AVT_NAMES = {"avtransport", "AVTransport"};

/*! @brief The class of the dlna client private.
 * @author Greedysky <greedysky@163.com>
 */
class QDlnaClientPrivate : public TTKPrivate<QDlnaClient>
{
public:
    QDlnaClientPrivate();
    ~QDlnaClientPrivate();

    void initialize(const QString &data);
    bool connectServer();

    bool m_isConnected;

    QDlnaXml *m_xml;
    QString m_serverIP, m_serverPort;
    QString m_smp, m_controlURL;
    QString m_friendlyName;
    QMap<QString, QDlnaService> m_services;

};

QDlnaClientPrivate::QDlnaClientPrivate()
{
    m_isConnected = false;
    m_xml = new QDlnaXml;
}

QDlnaClientPrivate::~QDlnaClientPrivate()
{
    delete m_xml;
}

void QDlnaClientPrivate::initialize(const QString &data)
{
    const QStringList& list = data.split("\r\n");
    for(QString str : qAsConst(list))
    {
        QStringList data_list;
        if(str.contains("LOCATION:"))
        {
            data_list = str.remove("LOCATION: http://").split(":");
        }
        else if(str.contains("Location:"))
        {
            data_list = str.remove("Location: http://").split(":");
        }
        else if(str.contains("location:"))
        {
            data_list = str.remove("location: http://").split(":");
        }

        if(data_list.count() >= 2)
        {
            m_serverIP = data_list[0];
            const QStringList& info_list = data_list[1].split(TTK_SEPARATOR);
            if(info_list.count() >= 2)
            {
                m_serverPort = info_list[0];
                m_smp = info_list[1];
            }
        }
    }
}

bool QDlnaClientPrivate::connectServer()
{
    const QString &request = QHelperDlna::MakeRequest("GET", m_smp, 0, QString(), m_serverIP, m_serverPort);
    const QString &response = QHelperDlna::makeSocketGetReply(m_serverIP, m_serverPort, request);
    TTK_LOGGER_INFO(m_serverIP << m_serverPort << m_smp << response);
    const int code = QHelperDlna::GetResponseCode(response);
    if(code != 200)
    {
        return false;
    }

    const QString &body = QHelperDlna::removeHttpHeader(response);
    if(!m_xml->fromString(m_xml->tagNameToLower(body)))
    {
        return false;
    }
    //
    for(const QString &name : qAsConst(FRIENS_NAMES))
    {
        m_friendlyName = m_xml->readTagNameValue(name);
        if(!m_friendlyName.isEmpty())
        {
            break;
        }
    }
    if(m_friendlyName.isEmpty())
    {
        return false;
    }
    //
    for(const QString &name : qAsConst(AVT_NAMES))
    {
        QDlnaService server = m_xml->readServiceTag(name, "server");
        if(!server.isEmpty())
        {
            m_services.insert(AVTRANSPORT, server);
            break;
        }
    }
    //
    for(const QString &name : qAsConst(AVT_NAMES))
    {
        QDlnaService server = m_xml->readServiceTag(name, "service");
        if(!server.isEmpty())
        {
            m_services.insert(AVTRANSPORT, server);
            break;
        }
    }
    //
    for(auto it = m_services.begin(); it != m_services.end(); ++it)
    {
        if(it.key().contains(AVTRANSPORT))
        {
            m_controlURL = it.value().m_controlURL;
            m_isConnected = true;
            return true;
        }
    }
    return false;
}



QDlnaClient::QDlnaClient(const QString &data)
{
    TTK_INIT_PRIVATE(QDlnaClient);
    TTK_D(QDlnaClient);
    d->initialize(data);
}

QString QDlnaClient::server()
{
    TTK_D(QDlnaClient);
    return d->m_serverIP;
}

QString QDlnaClient::serverName()
{
    TTK_D(QDlnaClient);
    return d->m_friendlyName;
}

bool QDlnaClient::connect()
{
    TTK_D(QDlnaClient);
    return d->connectServer();
}

bool QDlnaClient::isConnected() const
{
    TTK_D(QDlnaClient);
    return d->m_isConnected;
}

QString QDlnaClient::tryToPlayFile(const QString &url)
{
    uploadFileToPlay(url);
    const QString &playState = startPlay(0);
    return playState;
}

QString QDlnaClient::uploadFileToPlay(const QString &url)
{
    TTK_D(QDlnaClient);
    //Later we will send a message to the DLNA server to start the file playing
    QString play_url = url;
    QString body = XML_HEAD;
    body += "<u:SetAVTransportURI xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">\n";
    body += "<InstanceID>0</InstanceID>\n";
    body += "<CurrentURI>" + play_url.replace(" ", "%20") + "</CurrentURI>\n";
    body += "</u:SetAVTransportURI>\n";
    body += XML_FOOT + "\n";
    const QString &request = QHelperDlna::MakeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#SetAVTransportURI", d->m_serverIP,d->m_serverPort) + body;
    TTK_LOGGER_INFO(request);
    return QHelperDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request);
}

QString QDlnaClient::startPlay(int instance)
{
    TTK_D(QDlnaClient);
    //Start playing the new upload film or music track
    QString body = XML_HEAD;
    body += "<u:Play xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>"+ QString::number(instance) + "</InstanceID><Speed>1</Speed></u:Play>\n";
    body += XML_FOOT + "\n";
    const QString &request = QHelperDlna::MakeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Play", d->m_serverIP, d->m_serverPort) + body;
    return QHelperDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request);
}

QString QDlnaClient::stopPlay(int instance)
{
    TTK_D(QDlnaClient);
    //Called to stop playing a movie or a music track
    QString body = XML_HEAD;
    body += "<u:Stop xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:Stop>\n";
    body += XML_FOOT + "\n";
    const QString &request = QHelperDlna::MakeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Stop", d->m_serverIP, d->m_serverPort) + body;
    return QHelperDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request);
}

QString QDlnaClient::pause(int instance)
{
    TTK_D(QDlnaClient);
    //Called to pause playing a movie or a music track
    QString body = XML_HEAD;
    body += "<u:Pause xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:Pause>\n";
    body += XML_FOOT + "\n";
    const QString &request = QHelperDlna::MakeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Pause", d->m_serverIP, d->m_serverPort) + body;
    return QHelperDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request);
}

QString QDlnaClient::position()
{
    TTK_D(QDlnaClient);
    //Returns the current position for the track that is playing on the DLNA server
    const QString &body = XML_HEAD + "<m:GetPositionInfo xmlns:m=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui4\">0</InstanceID></m:GetPositionInfo>" + XML_FOOT + "\n";
    const QString &request = QHelperDlna::MakeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#GetPositionInfo", d->m_serverIP, d->m_serverPort) + body;
    return QHelperDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request);
}

int QDlnaClient::totalSeconds(const QString &value)
{
    //Convert the time left for the track to play back to seconds
    const QStringList& data_list = value.split(TTK_DOT);
    if(data_list.count() >= 2)
    {
        const QStringList& info_list = data_list[1].split(":");
        if(info_list.count() >= 2)
        {
            const int mins = info_list[0].toInt();
            const int secs = info_list[1].toInt();
            return mins * 60 + secs;
        }
    }
    return 0;
}
