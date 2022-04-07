#include "musicwebfmradioplaywidget.h"
#include "ui_musicwebfmradioplaywidget.h"
#include "musiccoremplayer.h"
#include "musicfmradiosongsrequest.h"
#include "musicfmradiodownloadtextrequest.h"
#include "musicdownloaddatarequest.h"
#include "musiclrcanalysis.h"
#include "musicimageutils.h"
#include "musicfunctionuiobject.h"
#include "musicdownloadwidget.h"

MusicWebFMRadioPlayWidget::MusicWebFMRadioPlayWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicWebFMRadioPlayWidget),
      m_songThread(nullptr)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_isPlaying = false;
    m_analysis = new MusicLrcAnalysis(this);
    m_analysis->setLineMax(9);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    m_ui->previousButton->setIcon(QIcon(":/functions/btn_previous_hover"));
    m_ui->nextButton->setIcon(QIcon(":/functions/btn_next_hover"));
    m_ui->downloadButton->setStyleSheet(MusicUIObject::MQSSBtnUnDownload);

    m_ui->playButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->previousButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->nextButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);

#ifdef Q_OS_UNIX
    m_ui->playButton->setFocusPolicy(Qt::NoFocus);
    m_ui->previousButton->setFocusPolicy(Qt::NoFocus);
    m_ui->nextButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->playButton->setIconSize(QSize(31, 31));
    m_ui->previousButton->setIconSize(QSize(31, 31));
    m_ui->nextButton->setIconSize(QSize(31, 31));

    m_ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->previousButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->nextButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->volumeSlider->setStyleSheet(MusicUIObject::MQSSSliderStyle10);
    m_ui->volumeSlider->setRange(0, 100);
    m_ui->volumeSlider->setValue(100);

    createCoreModule();
    MusicUtils::Widget::positionInCenter(this);

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(radioPlay()));
    connect(m_ui->previousButton, SIGNAL(clicked()), SLOT(radioPrevious()));
    connect(m_ui->nextButton, SIGNAL(clicked()), SLOT(radioNext()));
    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(radioResourceDownload()));
    connect(m_ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(radioVolume(int)));
}

MusicWebFMRadioPlayWidget::~MusicWebFMRadioPlayWidget()
{
    delete m_analysis;
    delete m_player;
    delete m_songThread;
    delete m_ui;
}

void MusicWebFMRadioPlayWidget::updateRadioSong(const QString &id)
{
    m_currentID = id;
    m_songThread->startToDownload(m_currentID);
}

void MusicWebFMRadioPlayWidget::mediaAutionPlayError(int code)
{
    if(DEFAULT_LOW_LEVEL == code)
    {
        radioNext();
    }
}

void MusicWebFMRadioPlayWidget::radioPlay()
{
    if(!m_player)
    {
        return;
    }

    m_isPlaying = !m_isPlaying;
    m_ui->playButton->setIcon(QIcon(m_isPlaying ? ":/functions/btn_pause_hover" : ":/functions/btn_play_hover"));
    m_player->play();
}

void MusicWebFMRadioPlayWidget::radioPrevious()
{
    if(m_currentID.isEmpty())
    {
        return;
    }

    m_songThread->startToDownload(m_currentID);

    if(!m_isPlaying)
    {
        m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    }
}

void MusicWebFMRadioPlayWidget::radioNext()
{
    if(m_currentID.isEmpty())
    {
        return;
    }

    m_songThread->startToDownload(m_currentID);

    if(!m_isPlaying)
    {
        m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    }
}

void MusicWebFMRadioPlayWidget::radioVolume(int num)
{
    if(m_player)
    {
        m_player->setVolume(num);
    }
}

void MusicWebFMRadioPlayWidget::radioResourceDownload()
{
    MusicObject::MusicSongInformation info;
    if(m_songThread)
    {
        info = m_songThread->musicSongInfo();
    }

    if(info.m_songProps.isEmpty())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(info, MusicAbstractQueryRequest::MusicQuery);
    download->show();
}

void MusicWebFMRadioPlayWidget::querySongInfoFinished()
{
    MusicObject::MusicSongInformation info;
    if(m_songThread)
    {
        info = m_songThread->musicSongInfo();
    }

    m_isPlaying = true;
    if(info.m_songProps.isEmpty())
    {
        return;
    }

    if(!m_player)
    {
        createCoreModule();
    }

    m_player->setMedia(MusicCoreMPlayer::MusicCategory, info.m_songProps.front().m_url);
    m_player->play();

    /// fix current play volume temporary
    const int v = m_ui->volumeSlider->value();
    m_ui->volumeSlider->setValue(0);
    m_ui->volumeSlider->setValue(v);

    QString name = MusicUtils::String::lrcPrefix() + info.m_singerName + " - " + info.m_songName + LRC_FILE;
    if(!QFile::exists(name))
    {
        MusicFMRadioDownLoadTextRequest* lrcDownload = new MusicFMRadioDownLoadTextRequest(info.m_lrcUrl, name, MusicObject::DownloadLrc, this);
        connect(lrcDownload, SIGNAL(downLoadDataChanged(QString)), SLOT(lrcDownloadStateChanged()));
        lrcDownload->startToDownload();
    }
    else
    {
        lrcDownloadStateChanged();
    }

    name = ART_DIR_FULL + info.m_singerName + SKN_FILE;
    if(!QFile::exists(name))
    {
        MusicDownloadDataRequest *download = new MusicDownloadDataRequest(info.m_coverUrl, name, MusicObject::DownloadSmallBackground, this);
        connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(picDownloadStateChanged()));
        download->startToDownload();
    }
    else
    {
        picDownloadStateChanged();
    }
}

void MusicWebFMRadioPlayWidget::closeEvent(QCloseEvent *event)
{
    delete m_player;
    m_player = nullptr;
    QWidget::closeEvent(event);
}

void MusicWebFMRadioPlayWidget::createCoreModule()
{
    m_player = new MusicCoreMPlayer(this);
    m_songThread = new MusicFMRadioSongsRequest(this);

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_player, SIGNAL(finished(int)), SLOT(mediaAutionPlayError(int)));
    connect(m_songThread, SIGNAL(downLoadDataChanged(QString)), SLOT(querySongInfoFinished()));
}

void MusicWebFMRadioPlayWidget::lrcDownloadStateChanged()
{
    MusicObject::MusicSongInformation info;
    if(m_songThread)
    {
        info = m_songThread->musicSongInfo();
    }

    if(info.m_songProps.isEmpty())
    {
        return;
    }

    const QString &name = (info.m_singerName + " - " + info.m_songName).trimmed();
    m_ui->titleWidget->setText(name);
    m_analysis->readFromLrcFile(MusicUtils::String::lrcPrefix() + name + LRC_FILE);
}

void MusicWebFMRadioPlayWidget::picDownloadStateChanged()
{
    MusicObject::MusicSongInformation info;
    if(m_songThread)
    {
        info = m_songThread->musicSongInfo();
    }

    if(info.m_songProps.isEmpty())
    {
        return;
    }

    QPixmap pix(ART_DIR_FULL + info.m_singerName + SKN_FILE);
    if(pix.isNull())
    {
        pix.load(":/image/lb_default_art");
    }

    pix = MusicUtils::Image::pixmapToRound(pix, QSize(150, 150), 100, 100);
    m_ui->artistLabel->setPixmap(pix);
    m_ui->artistLabel->start();
}

void MusicWebFMRadioPlayWidget::positionChanged(qint64 position)
{
    if(!m_player)
    {
        return;
    }

    m_ui->positionLabel->setText(QString("%1").arg(MusicTime::msecTime2LabelJustified(position * MT_S2MS)));

    if(m_analysis->isEmpty())
    {
        QString lrc = QString("<p style='font-weight:600;' align='center'>%1</p>").arg(tr("No lrc data file found"));
        m_ui->lrcLabel->setText(lrc);
        return;
    }

    const int index = m_analysis->currentIndex();
    const qint64 time = m_analysis->findTime(index);

    if(time < position * MT_S2MS && time != -1)
    {
        QString lrc;
        for(int i = 0; i < m_analysis->lineMax(); ++i)
        {
            if(i == m_analysis->lineMiddle())
            {
                lrc += QString("<p style='font-weight:600;' align='center'>");
            }
            else
            {
                lrc += QString("<p align='center'>");
            }

            lrc += m_analysis->text(i);
            lrc += QString("</p>");
        }
        m_ui->lrcLabel->setText(lrc);
        m_analysis->setCurrentIndex(index + 1);
    }
}

void MusicWebFMRadioPlayWidget::durationChanged(qint64 duration)
{
    if(!m_player)
    {
        return;
    }
    m_ui->durationLabel->setText(QString("/%1").arg(MusicTime::msecTime2LabelJustified(duration * MT_S2MS)));
}

void MusicWebFMRadioPlayWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
