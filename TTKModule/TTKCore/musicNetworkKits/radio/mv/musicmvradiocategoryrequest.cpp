#include "musicmvradiocategoryrequest.h"

MusicMVRadioCategoryRequest::MusicMVRadioCategoryRequest(QObject *parent)
    : MusicAbstractMVRadioRequest(parent)
{

}

void MusicMVRadioCategoryRequest::downLoadFinished()
{
    MusicAbstractMVRadioRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        bytes = QString(bytes).split("var mvfmdata = ").back().split("$img = ").front().toUtf8();
        bytes.chop(3);

        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(bytes, &ok);
        if(ok)
        {
            const QVariantList &datas = data.toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                if(value["classId"].toString() == m_queryValue)
                {
                    const QVariantList &fms = value["fm_list"].toList();
                    for(const QVariant &var : qAsConst(fms))
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();
                        TTK_NETWORK_QUERY_CHECK();

                        MusicResultsItem item;
                        item.m_name = value["fmName"].toString();
                        item.m_id = value["fmId"].toString();
                        item.m_coverUrl = value["imgUrlMv"].toString();
                        Q_EMIT createCategoryItem(item);
                    }
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
