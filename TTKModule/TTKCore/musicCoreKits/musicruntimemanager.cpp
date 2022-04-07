#include "musicruntimemanager.h"
#include "musicconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicnetworkthread.h"
#include "musicqmmputils.h"
#include "musicfileutils.h"
#include "musiccoreutils.h"
#include "musiccodecutils.h"
#include "ttkversion.h"

#include <QFont>
#include <QApplication>

namespace MusicObject
{
static quint64 directorySize(const QString &dirName)
{
    quint64 size = 0;
    if(QFileInfo(dirName).isDir())
    {
        QDir dir(dirName);
        const QFileInfoList &fileList = dir.entryInfoList(QDir::Files | QDir::Dirs |  QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for(const QFileInfo &fin : qAsConst(fileList))
        {
            if(fin.isDir())
            {
                size += directorySize(fin.absoluteFilePath());
            }
            else
            {
                size += fin.size();
            }
        }
    }
    return size;
}

static void checkCacheSize()
{
    const bool disabled = G_SETTING_PTR->value(MusicSettingManager::DownloadCacheEnable).toInt();
    if(!disabled)
    {
        return;
    }

    const quint64 cacheSize = G_SETTING_PTR->value(MusicSettingManager::DownloadCacheSize).toInt() * MH_MB2B;
    const QString &path = G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString();
    quint64 size = directorySize(path);
    if(size > cacheSize)
    {
        const QFileInfoList &fileList = QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(const QFileInfo &fin : qAsConst(fileList))
        {
            size -= fin.size();
            QFile::remove(fin.absoluteFilePath());
            if(size <= cacheSize)
            {
                break;
            }
        }
    }
}

static QString languageName(int index)
{
    QString lan(LANGUAGE_DIR_FULL);
    switch(index)
    {
        case 0: return lan.append("cn.ln");
        case 1: return lan.append("tc.ln");
        case 2: return lan.append("en.ln");
        default: return QString();
    }
}
}


void MusicRunTimeManager::run() const
{
    TTK_LOGGER_INFO("MusicApplication Run");
#if !TTK_QT_VERSION_CHECK(5,0,0)
    MusicUtils::Codec::setLocalCodec();
#endif
    MusicUtils::QMMP::updateQmmpConfigFile();

#ifdef Q_OS_UNIX
    QFont font;
    font.setPixelSize(13);
    qApp->setFont(font);
#endif

    //detect the current network state
    G_NETWORK_PTR->start();

    MusicConfigManager xml;
    xml.fromFile();
    xml.readBuffer();

    MusicObject::checkCacheSize();
    G_NETWORK_PTR->setBlockNetWork(G_SETTING_PTR->value(MusicSettingManager::CloseNetWorkMode).toInt());
}

QString MusicRunTimeManager::translator() const
{
    const int index = G_SETTING_PTR->value(MusicSettingManager::CurrentLanIndex).toInt();
    return MusicObject::languageName(index);
}

bool MusicRunTimeManager::configVersionCheck() const
{
    return G_SETTING_PTR->value(MusicSettingManager::ConfigVersion).toString() == TTK_CONFIG_VERSION_STR;
}
