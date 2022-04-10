#include "musicscreensaverwidget.h"
#include "musicapplicationmodule.h"
#include "musicsettingmanager.h"
#include "musictoolsetsuiobject.h"
#include "musicdownloadqueuerequest.h"
#include "musicimageutils.h"
#include "musicstringutils.h"
#include "musicwidgetutils.h"
#include "ttkdesktopwrapper.h"

#include "qsync/qsyncutils.h"

#define OS_ITEM_SIZE        QSize(165, 110)
#define OS_COUNT            10

#define OS_SCREENSAVER_URL  "ScreenSaver"
#define OS_WALLPAPER_NAME   "wallpaper.png"
#define OS_WALLBAR_NAME     "wallbar.png"
#define OS_WALLNAIL_NAME    "thumbnail.png"
#define OS_SCREEN_DIR       "Screen"

MusicScreenSaverHoverItem::MusicScreenSaverHoverItem(QLabel *parent)
    : QLabel(parent)
{
    setFixedSize(OS_ITEM_SIZE + QSize(8, 8));
    setAttribute(Qt::WA_TranslucentBackground);

    hide();
    m_index = -1;
    m_parentClass = parent;
    m_enableButton = new QPushButton(this);
    m_enableButton->setCursor(Qt::PointingHandCursor);
    m_enableButton->setStyleSheet(MusicUIObject::MQSSScreenItemDisable);
    m_enableButton->setGeometry((width() - 38) / 2, (height() - 38) / 2, 38, 38);
#ifdef Q_OS_UNIX
    m_enableButton->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_enableButton, SIGNAL(clicked()), SLOT(caseButtonOnAndOff()));
}

void MusicScreenSaverHoverItem::setFilePath(const QString &path)
{
    m_path = path;
    QPixmap pix(m_path);
    m_parentClass->setPixmap(pix);
    setPixmap(pix);
}

void MusicScreenSaverHoverItem::setStatus(int index, bool status)
{
    m_index = index;
    if(!status)
    {
        caseButtonOnAndOff();
    }
}

void MusicScreenSaverHoverItem::showItem(const QPoint &point)
{
    move(point - rect().center());
    show();
}

void MusicScreenSaverHoverItem::caseButtonOnAndOff()
{
    if(m_enableButton->styleSheet().contains(MusicUIObject::MQSSScreenItemDisable))
    {
        QPixmap pix(MusicUtils::Image::grayScalePixmap(QPixmap(m_path), 70));
        m_parentClass->setPixmap(pix);
        setPixmap(pix);
        m_enableButton->setStyleSheet(MusicUIObject::MQSSScreenItemEnable);
        Q_EMIT itemClicked(m_index, false);
    }
    else
    {
        QPixmap pix(m_path);
        m_parentClass->setPixmap(pix);
        setPixmap(pix);
        m_enableButton->setStyleSheet(MusicUIObject::MQSSScreenItemDisable);
        Q_EMIT itemClicked(m_index, true);
    }
}

void MusicScreenSaverHoverItem::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    hide();
}

void MusicScreenSaverHoverItem::focusOutEvent(QFocusEvent *event)
{
    QLabel::focusOutEvent(event);
    hide();
}

void MusicScreenSaverHoverItem::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    MusicUtils::Widget::setBorderShadow(this, &painter);
#if TTK_QT_VERSION_CHECK(5,15,0)
    const QPixmap &pix = pixmap(Qt::ReturnByValue);
#else
    const QPixmap &pix = *pixmap();
#endif
    painter.drawPixmap(QRect(QPoint(4, 4), OS_ITEM_SIZE), pix.scaled(OS_ITEM_SIZE));
}



MusicScreenSaverListItem::MusicScreenSaverListItem(QObject *object, QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(155, 100);

    m_hoverItem = new MusicScreenSaverHoverItem(this);
    m_hoverItem->setParent(parent);
    connect(m_hoverItem, SIGNAL(itemClicked(int,bool)), object, SLOT(itemHasClicked(int,bool)));
}

MusicScreenSaverListItem::~MusicScreenSaverListItem()
{
    delete m_hoverItem;
}

void MusicScreenSaverListItem::setFilePath(const QString &path)
{
    m_hoverItem->setFilePath(path);
}

void MusicScreenSaverListItem::setStatus(int index, bool status)
{
    m_hoverItem->setStatus(index, status);
}

#if TTK_QT_VERSION_CHECK(6,0,0)
void MusicScreenSaverListItem::enterEvent(QEnterEvent *event)
#else
void MusicScreenSaverListItem::enterEvent(QEvent *event)
#endif
{
    QLabel::enterEvent(event);
    m_hoverItem->showItem(mapToParent(rect().center()));
}



MusicScreenSaverListWidget::MusicScreenSaverListWidget(QWidget *parent)
    : QWidget(parent)
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_gridLayout->setContentsMargins(7, 7, 7, 7);
    setLayout(m_gridLayout);
}

MusicScreenSaverListWidget::~MusicScreenSaverListWidget()
{
    qDeleteAll(m_items);
}

void MusicScreenSaverListWidget::createItem(QObject *object, const QString &path, int index, bool status)
{
    MusicScreenSaverListItem *item = new MusicScreenSaverListItem(object, this);
    item->setFilePath(path);
    item->setStatus(index, status);

    m_gridLayout->addWidget(item, m_items.count() / 4, m_items.count() % 4, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

void MusicScreenSaverListWidget::resizeWindow()
{
    for(MusicScreenSaverListItem *item : qAsConst(m_items))
    {
        m_gridLayout->removeWidget(item);
    }

    const int side = width() / 160;
    for(int i = 0; i < m_items.count(); ++i)
    {
        m_gridLayout->addWidget(m_items[i],i / side, i % side, Qt::AlignLeft | Qt::AlignTop);
    }
}

void MusicScreenSaverListWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWindow();
}



MusicScreenSaverWidget::MusicScreenSaverWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle12 + MusicUIObject::MQSSColorStyle09);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);
    //
    QWidget *topWidget = new QWidget(this);
    topWidget->setFixedHeight(50);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *pLabel = new QLabel(tr("Screen Saver"), topWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(20);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(MusicUIObject::MQSSColorStyle11);

    QLabel *iLabel = new QLabel(tr("Popular singers spend your leisure time with you"), topWidget);
    QFont iLabelFont = iLabel->font();
    iLabelFont.setPixelSize(15);
    iLabel->setFont(iLabelFont);

    QLabel *wLabel = new QLabel(tr("Wait"), topWidget);
    QLabel *mLabel = new QLabel(tr("Min"), topWidget);

    m_currentState = false;
    m_inputEdit = new QLineEdit(topWidget);
    m_inputEdit->setFixedWidth(50);
    m_inputEdit->setEnabled(false);
    m_inputEdit->setAlignment(Qt::AlignCenter);
    m_inputEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle01);

    m_caseButton = new QPushButton(topWidget);
    m_caseButton->setFixedSize(44, 20);
    m_caseButton->setCursor(Qt::PointingHandCursor);
    m_caseButton->setStyleSheet(MusicUIObject::MQSSScreenSaverOff);
#ifdef Q_OS_UNIX
    m_caseButton->setFocusPolicy(Qt::NoFocus);
#endif

    topWidgetLayout->addWidget(pLabel);
    topWidgetLayout->addWidget(iLabel);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(wLabel);
    topWidgetLayout->addWidget(m_inputEdit);
    topWidgetLayout->addWidget(mLabel);
    topWidgetLayout->addWidget(m_caseButton);
    topWidget->setLayout(topWidgetLayout);
    mainLayout->addWidget(topWidget);
    //
    QFrame *frame = new QFrame(this);
    frame->setFixedHeight(1);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Plain);
    frame->setStyleSheet(MusicUIObject::MQSSColorStyle04);
    mainLayout->addWidget(frame);
    //
    QWidget *functionWidget = new QWidget(this);
    functionWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(10, 10, 10, 10);
    functionWidget->setLayout(functionWidgetLayout);
    mainLayout->addWidget(functionWidget);

    m_backgroundList = new MusicScreenSaverListWidget(this);
    functionWidgetLayout->addWidget(m_backgroundList);

    connect(m_inputEdit, SIGNAL(textChanged(QString)), SLOT(inputDataChanged()));
    connect(m_caseButton, SIGNAL(clicked()), SLOT(caseButtonOnAndOff()));

    initialize();
    applySettingParameter();
}

void MusicScreenSaverWidget::applySettingParameter()
{
    const bool state = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    const int value = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();

    m_inputEdit->setText(QString::number(value));
    if(state != m_currentState)
    {
        caseButtonOnAndOff();
    }
}

QVector<bool> MusicScreenSaverWidget::parseSettingParameter()
{
    QVector<bool> statusVector;
    statusVector.fill(true, OS_COUNT);

    const QString &value = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverIndex).toString();
    const QStringList items(value.split(";"));
    for(const QString &item : qAsConst(items))
    {
        const QStringList itemStatus(item.split(","));
        if(itemStatus.count() == 2)
        {
            const int index = itemStatus[0].toInt();
            const bool status = itemStatus[1].toInt();
            statusVector[index] = status;
        }
    }
    return statusVector;
}

void MusicScreenSaverWidget::resizeWindow()
{
    m_backgroundList->resizeWindow();
}

void MusicScreenSaverWidget::inputDataChanged()
{
    const bool state = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    if(state)
    {
        G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime, m_inputEdit->text().toInt());
        MusicApplicationModule::instance()->applySettingParameter();
    }
}

void MusicScreenSaverWidget::caseButtonOnAndOff()
{
    if(m_currentState)
    {
        m_currentState = false;
        m_caseButton->setStyleSheet(MusicUIObject::MQSSScreenSaverOff);
    }
    else
    {
        m_currentState = true;
        m_caseButton->setStyleSheet(MusicUIObject::MQSSScreenSaverOn);
        G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime, m_inputEdit->text().toInt());
    }

    m_inputEdit->setEnabled(m_currentState);
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable, m_currentState);
    MusicApplicationModule::instance()->applySettingParameter();
}

void MusicScreenSaverWidget::downLoadFinished(const QString &bytes)
{
    QVector<bool> statusVector(parseSettingParameter());
    if(bytes.contains(OS_WALLNAIL_NAME))
    {
        const int index = MusicUtils::String::stringSplitToken(bytes, OS_SCREEN_DIR, TTK_SEPARATOR).toInt();
        if(index < 0 || index >= statusVector.count())
        {
            return;
        }
        m_backgroundList->createItem(this, bytes, index, statusVector[index]);
    }
}

void MusicScreenSaverWidget::itemHasClicked(int index, bool status)
{
    QVector<bool> statusVector(parseSettingParameter());
    statusVector[index] = status;

    QStringList items;
    for(int i = 0; i < statusVector.count(); ++i)
    {
        items << QString("%1,%2").arg(i).arg(statusVector[i]);
    }
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverIndex, items.join(";"));
    MusicApplicationModule::instance()->applySettingParameter();
}

void MusicScreenSaverWidget::initialize()
{
    m_downloadQueue = new MusicDownloadQueueRequest(MusicObject::DownloadBigBackground, this);
    connect(m_downloadQueue, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));

    MusicDownloadQueueDataList datas;
    for(int i = 0; i < OS_COUNT; ++i)
    {
        const QString &url = QSyncUtils::makeDataBucketUrl() + QString("%1/%2/").arg(OS_SCREENSAVER_URL).arg(i);
        const QString &path = QString("%1%2/%3/").arg(CACHE_DIR_FULL, OS_SCREEN_DIR).arg(i);
        QDir().mkpath(path);

        MusicDownloadQueueData wallData;
        wallData.m_url = url + OS_WALLPAPER_NAME;
        wallData.m_savePath = path + OS_WALLPAPER_NAME;
        datas << wallData;

        MusicDownloadQueueData barData;
        barData.m_url = url + OS_WALLBAR_NAME;
        barData.m_savePath = path + OS_WALLBAR_NAME;
        datas << barData;

        MusicDownloadQueueData nailData;
        nailData.m_url = url + OS_WALLNAIL_NAME;
        nailData.m_savePath = path + OS_WALLNAIL_NAME;
        datas << nailData;
    }

    m_downloadQueue->addImageQueue(datas);
    m_downloadQueue->startToDownload();
}


MusicScreenSaverBackgroundWidget::MusicScreenSaverBackgroundWidget(QWidget *parent)
    : MusicTransitionAnimationLabel(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    m_state = false;
    m_isRunning = false;

    m_runningTimer = new QTimer(this);
    connect(m_runningTimer, SIGNAL(timeout()), SLOT(runningTimeout()));

    m_backgroundTimer = new QTimer(this);
    m_backgroundTimer->setInterval(15 * MT_S2MS);
    connect(m_backgroundTimer, SIGNAL(timeout()), SLOT(backgroundTimeout()));

    hide();
    qApp->installEventFilter(this);
}

MusicScreenSaverBackgroundWidget::~MusicScreenSaverBackgroundWidget()
{
    if(m_runningTimer->isActive())
    {
        m_runningTimer->stop();
    }
    delete m_runningTimer;

    if(m_backgroundTimer->isActive())
    {
        m_backgroundTimer->stop();
    }
    delete m_backgroundTimer;
}

void MusicScreenSaverBackgroundWidget::applySettingParameter()
{
    if(m_runningTimer->isActive())
    {
        m_runningTimer->stop();
        m_backgroundTimer->stop();
    }

    m_state = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toBool();
    const int value = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();
    m_state = (m_state && (value > 0));
    if(m_state)
    {
        m_runningTimer->setInterval(value * MT_M2MS);
        m_runningTimer->start();
    }
}

void MusicScreenSaverBackgroundWidget::runningTimeout()
{
    if(!m_isRunning)
    {
        m_isRunning = true;
        setGeometry(TTKDesktopWrapper::screenGeometry());
        setParent(nullptr);
        showFullScreen();

        backgroundTimeout();
        m_backgroundTimer->start();
    }
}

void MusicScreenSaverBackgroundWidget::backgroundTimeout()
{
    QVector<bool> statusVector(MusicScreenSaverWidget::parseSettingParameter());
    QVector<int> intVector;
    for(int i = 0; i < OS_COUNT; ++i)
    {
        if(statusVector[i])
        {
            intVector << i;
        }
    }

    if(!intVector.isEmpty())
    {
        const int index = intVector[MusicTime::random(intVector.count())];
        const QString &prefix = QString("%1%2/%3/").arg(CACHE_DIR_FULL, OS_SCREEN_DIR).arg(index);
        QPixmap background(prefix + OS_WALLPAPER_NAME);
        const QPixmap bar(prefix + OS_WALLBAR_NAME);
        MusicUtils::Image::fusionPixmap(background, bar, QPoint(100, 900));
        setPixmap(QPixmap(background.scaled(size())));
    }
}

bool MusicScreenSaverBackgroundWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()== QEvent::MouseButtonPress || event->type()== QEvent::MouseButtonRelease ||
       event->type()== QEvent::MouseButtonDblClick || event->type()== QEvent::MouseMove ||
       event->type()== QEvent::KeyPress || event->type()== QEvent::KeyRelease)
    {
        if(m_state)
        {
            if(m_isRunning)
            {
                m_isRunning = false;
                hide();
            }
            m_runningTimer->start();
        }
        else
        {
            m_runningTimer->stop();
            m_backgroundTimer->stop();
        }
    }
    return MusicTransitionAnimationLabel::eventFilter(watched, event);
}
