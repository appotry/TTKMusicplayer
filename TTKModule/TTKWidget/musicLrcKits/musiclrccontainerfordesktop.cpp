#include "musiclrccontainerfordesktop.h"
#include "musiclrcmanagerfordesktop.h"
#include "musicfunctionuiobject.h"
#include "musicdesktoplrcuiobject.h"
#include "musicsettingmanager.h"
#include "musiclrcdefines.h"

#include "musicapplication.h"
#include "musicbottomareawidget.h"

MusicLrcContainerForDesktop::MusicLrcContainerForDesktop(QWidget *parent)
    : MusicLrcContainer(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    m_containerType = LRC_DESKTOP_TPYE;
    m_reverse = false;
    m_singleLineType = false;
    m_windowLocked = false;
    m_verticalWindow = false;
    m_currentLrcFontSize = 0;
    m_widgetWidth = 0;
}

MusicLrcContainerForDesktop::~MusicLrcContainerForDesktop()
{
    clearAllMusicLRCManager();
    delete m_toolBarLayout;
    delete m_toolPlayButton;
    delete m_toolBarWidget;
}

void MusicLrcContainerForDesktop::startDrawLrc()
{
    m_musicLrcContainer[!m_singleLineType ? !m_reverse : 0]->startDrawLrc();
}

void MusicLrcContainerForDesktop::stopDrawLrc()
{
    for(MusicLrcManager *manager : qAsConst(m_musicLrcContainer))
    {
        manager->stopDrawLrc();
    }
}

void MusicLrcContainerForDesktop::applySettingParameter()
{
    MusicLrcContainer::applySettingParameter();
    for(MusicLrcManager *manager : qAsConst(m_musicLrcContainer))
    {
        m_currentLrcFontSize = G_SETTING_PTR->value(MusicSettingManager::DLrcSize).toInt();
        manager->setLrcFontSize(m_currentLrcFontSize);
    }
    m_windowLocked = G_SETTING_PTR->value(MusicSettingManager::DLrcLockedMode).toInt() == 1;
    m_singleLineType = !(G_SETTING_PTR->value(MusicSettingManager::DLrcSingleLineMode).toInt() == 1);
    setSingleLineTypeChanged();

    const QPoint &point = G_SETTING_PTR->value(MusicSettingManager::DLrcGeometry).toPoint();
    if(!point.isNull())
    {
        move(point);
    }
}

void MusicLrcContainerForDesktop::initCurrentLrc() const
{
    if(m_currentTime == 0)
    {
        m_musicLrcContainer[0]->setText(tr("Welcome to use TTKMusicPlayer!"));
    }
}

void MusicLrcContainerForDesktop::setCurrentPlayStatus(bool status) const
{
    m_toolPlayButton->setStyleSheet(status ? MusicUIObject::MQSSDeskTopPause : MusicUIObject::MQSSDeskTopPlay);
}

void MusicLrcContainerForDesktop::updateCurrentLrc(const QString &first, const QString &second, qint64 time)
{
    if(!m_singleLineType)
    {
        m_reverse = !m_reverse;
        m_musicLrcContainer[m_reverse]->reset();
        m_musicLrcContainer[m_reverse]->setText(second);
        m_musicLrcContainer[!m_reverse]->setText(first);
        m_musicLrcContainer[!m_reverse]->startDrawLrcMask(time);
    }
    else
    {
        m_musicLrcContainer[0]->setText(first);
        m_musicLrcContainer[0]->startDrawLrcMask(time);
    }

    resizeLrcSizeArea();
}

void MusicLrcContainerForDesktop::makeStatusCopy(MusicLrcContainerForDesktop *other)
{
    m_currentSongName = other->m_currentSongName;
    m_toolPlayButton->setStyleSheet(other->m_toolPlayButton->styleSheet());
}

void MusicLrcContainerForDesktop::setWindowLockedChanged()
{
    m_windowLocked = !m_windowLocked;
    if(m_windowLocked)
    {
       m_toolBarWidget->hide();
       setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    }
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcLockedMode,  m_windowLocked ? 1 : 0);
    MusicBottomAreaWidget::instance()->lockDesktopLrc(m_windowLocked);
}

void MusicLrcContainerForDesktop::setLrcBiggerChanged()
{
    if(m_currentLrcFontSize > MusicLrcDefines().findDesktopLastSize() - 1)
    {
        return;
    }
    resizeLrcSizeArea(true);
}

void MusicLrcContainerForDesktop::setLrcSmallerChanged()
{
    if(m_currentLrcFontSize < MusicLrcDefines().findDesktopFirstSize() - 1)
    {
        return;
    }
    resizeLrcSizeArea(false);
}

void MusicLrcContainerForDesktop::toolStyleChanged()
{
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    createColorMenu(menu);
    menu.exec(QCursor::pos());
}

void MusicLrcContainerForDesktop::setSingleLineTypeChanged()
{
    m_singleLineType = !m_singleLineType;
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSingleLineMode, m_singleLineType);

    if(m_singleLineType)
    {
        if(!m_reverse)
        {
            m_musicLrcContainer[0]->setText(m_musicLrcContainer[1]->text());
            startDrawLrc();
        }
        m_musicLrcContainer[1]->hide();
    }
    else
    {
        m_musicLrcContainer[1]->show();
    }
    m_musicLrcContainer[1]->setText(QString());
}

void MusicLrcContainerForDesktop::createColorMenu(QMenu &menu)
{
    QActionGroup *group = new QActionGroup(this);
    group->addAction(menu.addAction(tr("White")))->setData(0 + LRC_COLOR_OFFSET);
    group->addAction(menu.addAction(tr("Blue")))->setData(1 + LRC_COLOR_OFFSET);
    group->addAction(menu.addAction(tr("Red")))->setData(2 + LRC_COLOR_OFFSET);
    group->addAction(menu.addAction(tr("Black")))->setData(3 + LRC_COLOR_OFFSET);
    group->addAction(menu.addAction(tr("Yellow")))->setData(4 + LRC_COLOR_OFFSET);
    group->addAction(menu.addAction(tr("Purple")))->setData(5 + LRC_COLOR_OFFSET);
    group->addAction(menu.addAction(tr("Green")))->setData(6 + LRC_COLOR_OFFSET);
    connect(group, SIGNAL(triggered(QAction*)), SLOT(changeCurrentLrcColor(QAction*)));
    menu.addSeparator();
    menu.addAction(tr("Custom"), this, SLOT(currentLrcCustom()));

    const int index = G_SETTING_PTR->value("DLrcColor").toInt() - LRC_COLOR_OFFSET;
    if(index > -1 && index < group->actions().count())
    {
        group->actions()[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }
}

void MusicLrcContainerForDesktop::setSelfPosition() const
{
    for(MusicLrcManager *manager : qAsConst(m_musicLrcContainer))
    {
        manager->setSelfPosition(m_geometry.x(), m_geometry.y());
    }
}

void MusicLrcContainerForDesktop::creatToolBarWidget()
{
    m_toolBarLayout->setContentsMargins(0, 0, 0, 0);
    m_toolBarLayout->addStretch(1);

    m_toolBarWidget->setObjectName("toolBarWidget");
    m_toolBarWidget->setStyleSheet(QString("#toolBarWidget{%1}").arg(MusicUIObject::MQSSBackgroundStyle08));
    m_toolBarWidget->setLayout(m_toolBarLayout);

    QPushButton *showMainWindow = new QPushButton(m_toolBarWidget);
    showMainWindow->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(showMainWindow, 0, Qt::AlignCenter);
    connect(showMainWindow, SIGNAL(clicked()), MusicApplication::instance(), SLOT(showNormal()));
#ifdef Q_OS_UNIX
    showMainWindow->setFocusPolicy(Qt::NoFocus);
#endif

    QToolButton *toolWindowTypeButton = new QToolButton(m_toolBarWidget);
    toolWindowTypeButton->setFixedSize(m_verticalWindow ? TOOLBAR_HEIGHT : TOOLBAR_WIDTH, m_verticalWindow ? TOOLBAR_WIDTH : TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolWindowTypeButton, 0, Qt::AlignCenter);
    connect(toolWindowTypeButton, SIGNAL(clicked()), parent(), SLOT(setWindowLrcTypeChanged()));

    QToolButton *toolPreSongButton = new QToolButton(m_toolBarWidget);
    toolPreSongButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolPreSongButton, 0, Qt::AlignCenter);
    connect(toolPreSongButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicPlayPrevious()));

    m_toolPlayButton = new QToolButton(m_toolBarWidget);
    m_toolPlayButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(m_toolPlayButton, 0, Qt::AlignCenter);
    connect(m_toolPlayButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicStatePlay()));

    QToolButton *toolNextSongButton = new QToolButton(m_toolBarWidget);
    toolNextSongButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolNextSongButton, 0, Qt::AlignCenter);
    connect(toolNextSongButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicPlayNext()));

    QToolButton *toolSettingButton = new QToolButton(m_toolBarWidget);
    toolSettingButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolSettingButton, 0, Qt::AlignCenter);
    connect(toolSettingButton, SIGNAL(clicked()), SLOT(currentLrcCustom()));

    QToolButton *toolLrcSmallerButton = new QToolButton(m_toolBarWidget);
    toolLrcSmallerButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolLrcSmallerButton, 0, Qt::AlignCenter);
    connect(toolLrcSmallerButton, SIGNAL(clicked()), SLOT(setLrcSmallerChanged()));

    QToolButton *toolLrcBigerButton = new QToolButton(m_toolBarWidget);
    toolLrcBigerButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolLrcBigerButton, 0, Qt::AlignCenter);
    connect(toolLrcBigerButton, SIGNAL(clicked()), SLOT(setLrcBiggerChanged()));

    QToolButton *toolStyleButton = new QToolButton(m_toolBarWidget);
    toolStyleButton->setFixedSize(20, 20);
    m_toolBarLayout->addWidget(toolStyleButton, 0, Qt::AlignCenter);
    connect(toolStyleButton, SIGNAL(clicked()), SLOT(toolStyleChanged()));

    QToolButton *toolMakeLrcTextButton = new QToolButton(m_toolBarWidget);
    toolMakeLrcTextButton->setFixedSize(m_verticalWindow ? TOOLBAR_HEIGHT : TOOLBAR_TEXT_LENGTH, m_verticalWindow ? TOOLBAR_TEXT_LENGTH : TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolMakeLrcTextButton, 0, Qt::AlignCenter);
    connect(toolMakeLrcTextButton, SIGNAL(clicked()), SLOT(showLrcMakedWidget()));

    QToolButton *toolSearchLrcTextButton = new QToolButton(m_toolBarWidget);
    toolSearchLrcTextButton->setFixedSize(m_verticalWindow ? TOOLBAR_HEIGHT : TOOLBAR_TEXT_LENGTH, m_verticalWindow ? TOOLBAR_TEXT_LENGTH : TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolSearchLrcTextButton, 0, Qt::AlignCenter);
    connect(toolSearchLrcTextButton, SIGNAL(clicked()), SLOT(searchMusicLrcs()));

    QToolButton *toolUpdateLrcTextButton = new QToolButton(m_toolBarWidget);
    toolUpdateLrcTextButton->setFixedSize(m_verticalWindow ? TOOLBAR_HEIGHT : TOOLBAR_TEXT_LENGTH, m_verticalWindow ? TOOLBAR_TEXT_LENGTH : TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolUpdateLrcTextButton, 0, Qt::AlignCenter);
    connect(toolUpdateLrcTextButton, SIGNAL(clicked()), SIGNAL(currentLrcUpdated()));

    QToolButton *toolWindowLockedButton = new QToolButton(m_toolBarWidget);
    toolWindowLockedButton->setFixedSize(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT);
    m_toolBarLayout->addWidget(toolWindowLockedButton, 0, Qt::AlignCenter);
    connect(toolWindowLockedButton, SIGNAL(clicked()), SLOT(setWindowLockedChanged()));

    QToolButton *toolCloseButton = new QToolButton(m_toolBarWidget);
    toolCloseButton->setFixedSize(14, 14);
    m_toolBarLayout->addWidget(toolCloseButton, 0, Qt::AlignCenter);
    connect(toolCloseButton, SIGNAL(clicked()), SLOT(close()));
    m_toolBarLayout->addStretch(1);

    showMainWindow->setIcon(QIcon(":/image/lb_app_logo"));

    showMainWindow->setCursor(QCursor(Qt::PointingHandCursor));
    toolWindowTypeButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolStyleButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolMakeLrcTextButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolSearchLrcTextButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolUpdateLrcTextButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolWindowLockedButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolLrcBigerButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolLrcSmallerButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolPreSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolNextSongButton->setCursor(QCursor(Qt::PointingHandCursor));
    toolSettingButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_toolPlayButton->setCursor(QCursor(Qt::PointingHandCursor));

    showMainWindow->setStyleSheet(MusicUIObject::MQSSPushButtonStyle02);
    toolCloseButton->setStyleSheet(MusicUIObject::MQSSBtnTClose);
    toolStyleButton->setStyleSheet(MusicUIObject::MQSSDeskTopStyle);
    toolWindowLockedButton->setStyleSheet(MusicUIObject::MQSSDeskTopLock);
    toolLrcSmallerButton->setStyleSheet(MusicUIObject::MQSSDeskTopSizeDown);
    toolLrcBigerButton->setStyleSheet(MusicUIObject::MQSSDeskTopSizeUp);
    toolPreSongButton->setStyleSheet(MusicUIObject::MQSSDeskTopPrevious);
    toolNextSongButton->setStyleSheet(MusicUIObject::MQSSDeskTopNext);
    toolSettingButton->setStyleSheet(MusicUIObject::MQSSDeskTopSetting);
    m_toolPlayButton->setStyleSheet(MusicUIObject::MQSSDeskTopPlay);

    toolWindowTypeButton->setStyleSheet(m_verticalWindow ? MusicUIObject::MQSSDeskTopHorizontal : MusicUIObject::MQSSDeskTopVertical);
    toolMakeLrcTextButton->setStyleSheet(m_verticalWindow ? MusicUIObject::MQSSDeskTopVMakeLrc : MusicUIObject::MQSSDeskTopHMakeLrc);
    toolSearchLrcTextButton->setStyleSheet(m_verticalWindow ? MusicUIObject::MQSSDeskTopVSearchLrc : MusicUIObject::MQSSDeskTopHSearchLrc);
    toolUpdateLrcTextButton->setStyleSheet(m_verticalWindow ? MusicUIObject::MQSSDeskTopVUpdateLrc : MusicUIObject::MQSSDeskTopHUpdateLrc);

    showMainWindow->setToolTip(tr("Show MainWindow"));
    toolCloseButton->setToolTip(tr("Hide"));
    toolWindowLockedButton->setToolTip(tr("Lock Lrc"));
    toolLrcBigerButton->setToolTip(tr("Zoom Up"));
    toolLrcSmallerButton->setToolTip(tr("Zoom Down"));
    toolPreSongButton->setToolTip(tr("Previous"));
    toolNextSongButton->setToolTip(tr("Next"));
    toolSettingButton->setToolTip(tr("Settings"));
    m_toolPlayButton->setToolTip(tr("Play"));

    m_toolBarWidget->hide();
}

void MusicLrcContainerForDesktop::resizeLrcSizeArea(bool resize)
{
    m_geometry.setY(resize ? m_geometry.y() + 1 : m_geometry.y() - 1);
    setSelfPosition();

    const int size = resize ? ++m_currentLrcFontSize : --m_currentLrcFontSize;
    for(MusicLrcManager *manager : qAsConst(m_musicLrcContainer))
    {
        manager->setLrcFontSize(size);
    }

    if(!m_singleLineType)
    {
        m_musicLrcContainer[1]->setText(m_musicLrcContainer[1]->text());
    }

    resizeLrcSizeArea();
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSize, m_currentLrcFontSize);
}

void MusicLrcContainerForDesktop::mousePressEvent(QMouseEvent *event)
{
    MusicLrcContainer::mousePressEvent(event);
    if(!m_windowLocked && event->button() == Qt::LeftButton)
    {
#if TTK_QT_VERSION_CHECK(6,0,0)
        m_offset = event->globalPosition().toPoint() - frameGeometry().topLeft();
#else
        m_offset = event->globalPos() - frameGeometry().topLeft();
#endif
    }
}

void MusicLrcContainerForDesktop::mouseMoveEvent(QMouseEvent *event)
{
    MusicLrcContainer::mouseMoveEvent(event);
    if(!m_windowLocked && (event->buttons() & Qt::LeftButton))
    {
        setCursor(Qt::CrossCursor);
#if TTK_QT_VERSION_CHECK(6,0,0)
        move(event->globalPosition().toPoint() - m_offset);
#else
        move(event->globalPos() - m_offset);
#endif
        G_SETTING_PTR->setValue(MusicSettingManager::DLrcGeometry, pos());
    }
}

#if TTK_QT_VERSION_CHECK(6,0,0)
void MusicLrcContainerForDesktop::enterEvent(QEnterEvent *event)
#else
void MusicLrcContainerForDesktop::enterEvent(QEvent *event)
#endif
{
    if(m_windowLocked)
    {
        return;
    }

    MusicLrcContainer::enterEvent(event);
    m_toolBarWidget->show();
    setStyleSheet(QString("#desktopWidget{%1}").arg(MusicUIObject::MQSSBackgroundStyle08));
}

void MusicLrcContainerForDesktop::leaveEvent(QEvent *event)
{
    if(m_windowLocked)
    {
        return;
    }

    MusicLrcContainer::leaveEvent(event);
    m_toolBarWidget->hide();
    setStyleSheet(QString("#desktopWidget{%1}").arg(MusicUIObject::MQSSBackgroundStyle01));
}

void MusicLrcContainerForDesktop::closeEvent(QCloseEvent *event)
{
    MusicLrcContainer::closeEvent(event);
    MusicBottomAreaWidget::instance()->desktopLrcClosed();
}

void MusicLrcContainerForDesktop::contextMenuEvent(QContextMenuEvent *event)
{
    MusicLrcContainer::contextMenuEvent(event);

    QMenu menu;
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu.addAction(tr("Lrc Search"), this, SLOT(searchMusicLrcs()));
    menu.addAction(tr("Lrc Update"), this, SIGNAL(currentLrcUpdated()));
    menu.addAction(tr("Lrc Make"), this, SLOT(showLrcMakedWidget()));
    menu.addAction(!m_singleLineType ? tr("Single Line") : tr("Double Line"), this, SLOT(setSingleLineTypeChanged()));
    menu.addSeparator();

    QAction *lrcLinkAc = menu.addAction(tr("Link Off"), this, SLOT(linkLrcStateChanged()));
    m_linkLocalLrc ? lrcLinkAc->setText(tr("Link Off")) : lrcLinkAc->setText(tr("Link On"));
    menu.addAction(tr("Hide"), this, SLOT(close()));
    menu.addAction(QIcon(":/contextMenu/btn_lock"), m_windowLocked ? tr("Unlock Lrc"): tr("Lock Lrc"), this, SLOT(setWindowLockedChanged()));

    QMenu changColorMenu(tr("Color"), &menu);
    createColorMenu(changColorMenu);
    menu.addMenu(&changColorMenu);

    menu.addSeparator();
    menu.addAction(tr("Settings"), this, SLOT(currentLrcCustom()));

    menu.exec(QCursor::pos());
}



MusicLrcContainerHorizontalDesktop::MusicLrcContainerHorizontalDesktop(QWidget *parent)
    : MusicLrcContainerForDesktop(parent)
{
    m_verticalWindow = false;
    const QSize &windowSize = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    m_widgetWidth = windowSize.width() - 300;
    m_geometry.setX(m_widgetWidth);
    m_geometry.setY(60);

    m_toolBarWidget = new QWidget(this);
    m_toolBarWidget->setGeometry(0, 0, m_geometry.x(), TOOLBAR_MAIN_HEIGHT);
    m_toolBarLayout = new QHBoxLayout(m_toolBarWidget);
    creatToolBarWidget();

    QWidget *desktopWidget = new QWidget(this);
    desktopWidget->setObjectName("desktopWidget");
    m_musicLrcContainer << new MusicLrcManagerHorizontalDesktop(desktopWidget)
                        << new MusicLrcManagerHorizontalDesktop(desktopWidget);

    move(200,  windowSize.height() - height() - 200);
    setFixedSize(m_geometry.x(), 2 * m_geometry.y() + TOOLBAR_HEIGHT + TOOLBAR_MAIN_HEIGHT);
    desktopWidget->setGeometry(0, TOOLBAR_MAIN_HEIGHT, m_geometry.x(), 2 * m_geometry.y() + TOOLBAR_MAIN_HEIGHT);

    setSelfPosition();
    m_currentLrcFontSize = m_musicLrcContainer[0]->lrcFontSize();
}

void MusicLrcContainerHorizontalDesktop::initCurrentLrc() const
{
    MusicLrcContainerForDesktop::initCurrentLrc();
    if(m_currentTime == 0)
    {
        const int width = m_musicLrcContainer[0]->x();
        if(m_singleLineType)
        {
            m_musicLrcContainer[0]->setGeometry((m_widgetWidth - width) / 2, 20, width, m_geometry.y());
        }
        else
        {
            m_musicLrcContainer[0]->setGeometry(0, 20, width, m_geometry.y());
            m_musicLrcContainer[1]->setGeometry(0, m_geometry.y() + 20, 0, 0);
        }
    }
}

void MusicLrcContainerHorizontalDesktop::setSingleLineTypeChanged()
{
    MusicLrcContainerForDesktop::setSingleLineTypeChanged();
    if(m_singleLineType)
    {
        const int width = m_musicLrcContainer[0]->x();
        m_musicLrcContainer[0]->move((m_widgetWidth - width) / 2, 20);
    }
    else
    {
        m_musicLrcContainer[0]->move(0, 20);
    }
}

void MusicLrcContainerHorizontalDesktop::resizeLrcSizeArea()
{
    int width = m_musicLrcContainer[0]->x();
    if(m_singleLineType)
    {
        m_musicLrcContainer[0]->setGeometry((m_widgetWidth - width) / 2, 20, width, m_geometry.y());
    }
    else
    {
        m_musicLrcContainer[0]->setGeometry(0, 20, width, m_geometry.y());
        width = m_musicLrcContainer[1]->x();
        int pos = m_geometry.x() - width;
        if(pos < 0)
        {
            pos = 0;
        }
        m_musicLrcContainer[1]->setGeometry(pos, m_geometry.y() + 20, width, m_geometry.y());
    }
}



MusicLrcContainerVerticalDesktop::MusicLrcContainerVerticalDesktop(QWidget *parent)
    : MusicLrcContainerForDesktop(parent)
{
    m_verticalWindow = true;
    const QSize &windowSize = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    m_widgetWidth = windowSize.height() - 150;
    m_geometry.setX(m_widgetWidth);
    m_geometry.setY(60);

    m_toolBarWidget = new QWidget(this);
    m_toolBarWidget->setGeometry(0, 0, TOOLBAR_MAIN_HEIGHT, m_geometry.x());
    m_toolBarLayout = new QVBoxLayout(m_toolBarWidget);
    creatToolBarWidget();

    QWidget *desktopWidget = new QWidget(this);
    desktopWidget->setObjectName("desktopWidget");
    m_musicLrcContainer << new MusicLrcManagerVerticalDesktop(desktopWidget)
                        << new MusicLrcManagerVerticalDesktop(desktopWidget);

    move(200, 75);
    setFixedSize(2 * m_geometry.y() + TOOLBAR_HEIGHT + TOOLBAR_MAIN_HEIGHT, m_geometry.x());
    desktopWidget->setGeometry(TOOLBAR_MAIN_HEIGHT, 0, 2 * m_geometry.y() + TOOLBAR_MAIN_HEIGHT, m_geometry.x());

    setSelfPosition();
    m_currentLrcFontSize = m_musicLrcContainer[0]->lrcFontSize();
}

void MusicLrcContainerVerticalDesktop::initCurrentLrc() const
{
    MusicLrcContainerForDesktop::initCurrentLrc();
    if(m_currentTime == 0)
    {
        const int height = m_musicLrcContainer[0]->x();
        if(m_singleLineType)
        {
            m_musicLrcContainer[0]->setGeometry(20, (m_widgetWidth - height) / 2, m_geometry.y(), height);
        }
        else
        {
            m_musicLrcContainer[0]->setGeometry(20, 0, m_geometry.y(), height);
            m_musicLrcContainer[1]->setGeometry(m_geometry.x() + 20, 0, 0, 0);
        }
    }
}

void MusicLrcContainerVerticalDesktop::setSingleLineTypeChanged()
{
    MusicLrcContainerForDesktop::setSingleLineTypeChanged();
    if(m_singleLineType)
    {
        const int height = m_musicLrcContainer[0]->x();
        m_musicLrcContainer[0]->move(20, (m_widgetWidth - height) / 2);
    }
    else
    {
        m_musicLrcContainer[0]->move(20, 0);
    }
}

void MusicLrcContainerVerticalDesktop::resizeLrcSizeArea()
{
    int height = m_musicLrcContainer[0]->x();
    if(m_singleLineType)
    {
        m_musicLrcContainer[0]->setGeometry(20, (m_widgetWidth - height) / 2, m_geometry.y(), height);
    }
    else
    {
        m_musicLrcContainer[0]->setGeometry(20, 0, m_geometry.y(), height);

        height = m_musicLrcContainer[1]->x();
        int pos = m_geometry.x() - height;
        if(pos < 0)
        {
            pos = 0;
        }
        m_musicLrcContainer[1]->setGeometry(m_geometry.y() + 20, pos, m_geometry.y(), height);
    }
}
