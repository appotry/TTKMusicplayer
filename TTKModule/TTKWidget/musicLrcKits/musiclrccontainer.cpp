#include "musiclrccontainer.h"
#include "musiclrcsearchwidget.h"
#include "musiclrcmakerwidget.h"

MusicLrcContainer::MusicLrcContainer(QWidget *parent)
    : QWidget(parent),
      m_linkLocalLrc(true),
      m_currentTime(0),
      m_totalTime(0),
      m_lrcAnalysis(nullptr),
      m_lrcSearchWidget(nullptr)
{

}

MusicLrcContainer::~MusicLrcContainer()
{
    delete m_lrcSearchWidget;
}

void MusicLrcContainer::applyParameter()
{
    const QString &t = (m_containerType == LRC_DESKTOP_TPYE ? LRC_DESKTOP_PREFIX : QString());

    for(MusicLrcManager *manager : qAsConst(m_lrcManagers))
    {
        manager->setFontFamily(G_SETTING_PTR->value(t + "LrcFamily").toInt());
        manager->setFontType(G_SETTING_PTR->value(t + "LrcType").toInt());
        manager->setFontTransparent(G_SETTING_PTR->value(t + "LrcColorTransparent").toInt());
        manager->setLrcFontSize(G_SETTING_PTR->value(t + "LrcSize").toInt());
    }

    if(G_SETTING_PTR->value(t + "LrcColor").toInt() != -1)
    {
        setLinearGradientColor(TTKStaticCast(MusicLrcColor::Color, G_SETTING_PTR->value(t + "LrcColor").toInt()));
    }
    else
    {
        setLinearGradientColor({TTK::readColorConfig(G_SETTING_PTR->value(t + "LrcFrontgroundColor").toString()),
                                TTK::readColorConfig(G_SETTING_PTR->value(t + "LrcBackgroundColor").toString())});
    }
}

void MusicLrcContainer::setLinearGradientColor(MusicLrcColor::Color color)
{
    setLinearGradientColor(TTK::mapIndexToColor(color));
}

void MusicLrcContainer::setLinearGradientColor(const MusicLrcColor &color)
{
    for(MusicLrcManager *manager : qAsConst(m_lrcManagers))
    {
        manager->setLinearGradientColor(color);
    }

    G_SETTING_PTR->setValue((m_containerType == LRC_DESKTOP_TPYE) ? MusicSettingManager::DLrcColor : MusicSettingManager::LrcColor, TTKStaticCast(int, color.m_index));
    Q_EMIT linearGradientColorChanged();
}

void MusicLrcContainer::setCurrentTime(qint64 time, qint64 total) noexcept
{
    m_currentTime = time;
    m_totalTime = total;
}

qint64 MusicLrcContainer::totalTime() const noexcept
{
    return m_totalTime;
}

void MusicLrcContainer::currentLrcCustom()
{
    Q_EMIT showCurrentLrcColorSetting();
    Q_EMIT showCurrentLrcSetting();
}

void MusicLrcContainer::changeCurrentLrcColor(QAction *action)
{
    switch(action->data().toInt())
    {
        case 0: setLinearGradientColor(MusicLrcColor::Color::IYellow); break;
        case 1: setLinearGradientColor(MusicLrcColor::Color::IBlue); break;
        case 2: setLinearGradientColor(MusicLrcColor::Color::IGray); break;
        case 3: setLinearGradientColor(MusicLrcColor::Color::IPink); break;
        case 4: setLinearGradientColor(MusicLrcColor::Color::IGreen); break;
        case 5: setLinearGradientColor(MusicLrcColor::Color::IRed); break;
        case 6: setLinearGradientColor(MusicLrcColor::Color::IPurple); break;
        case 7: setLinearGradientColor(MusicLrcColor::Color::IOrange); break;
        case 8: setLinearGradientColor(MusicLrcColor::Color::IIndigo); break;
        case 9: setLinearGradientColor(MusicLrcColor::Color::DWhite); break;
        case 10: setLinearGradientColor(MusicLrcColor::Color::DBlue); break;
        case 11: setLinearGradientColor(MusicLrcColor::Color::DRed); break;
        case 12: setLinearGradientColor(MusicLrcColor::Color::DBlack); break;
        case 13: setLinearGradientColor(MusicLrcColor::Color::DYellow); break;
        case 14: setLinearGradientColor(MusicLrcColor::Color::DPurple); break;
        case 15: setLinearGradientColor(MusicLrcColor::Color::DGreen); break;
        default: break;
    }
}

void MusicLrcContainer::changeCurrentLrcColor(int index)
{
    setLinearGradientColor(TTKStaticCast(MusicLrcColor::Color, index));
}

void MusicLrcContainer::searchMusicLrcs()
{
    delete m_lrcSearchWidget;
    m_lrcSearchWidget = new MusicLrcSearchWidget(this);
    m_lrcSearchWidget->setCurrentSongName(m_currentSongName);
    m_lrcSearchWidget->exec();
}

void MusicLrcContainer::showLrcMakedWidget()
{
    MusicLrcMakerWidget *w = TTKGenerateSingleWidget(MusicLrcMakerWidget, this);
    w->setCurrentSongName(m_currentSongName);
    w->durationChanged(m_totalTime);
}

void MusicLrcContainer::linkLrcStateChanged()
{
    m_linkLocalLrc = !m_linkLocalLrc;
    for(MusicLrcManager *manager : qAsConst(m_lrcManagers))
    {
        manager->setVisible(m_linkLocalLrc);
    }
}

void MusicLrcContainer::clearAllMusicLRCManager()
{
    qDeleteAll(m_lrcManagers);
    m_lrcManagers.clear();
}
