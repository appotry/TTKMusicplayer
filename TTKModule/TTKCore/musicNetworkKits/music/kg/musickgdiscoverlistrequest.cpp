#include "musickgdiscoverlistrequest.h"
#include "musickgqueryinterface.h"

MusicKGDiscoverListRequest::MusicKGDiscoverListRequest(QObject *parent)
    : MusicDiscoverListRequest(parent)
{

}

void MusicKGDiscoverListRequest::startToSearch()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    deleteAll();
    m_item.clear();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_TOPLIST_URL, false).arg(6666).arg(0).arg(TOPLIST_PAGE_SIZE));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGDiscoverListRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicDiscoverListRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();

                const QVariantList &datas = value["info"].toList();
                int where = datas.count();
                where = (where > 0) ? TTK::random(where) : 0;

                int counter = 0;
                for(const QVariant &var : qAsConst(datas))
                {
                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    m_item = value["filename"].toString();
                    break;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(m_item);
    deleteAll();
}
