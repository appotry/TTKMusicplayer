#include "qkugouwindow.h"
#include "qkugouuiobject.h"

#ifdef Q_OS_WIN
#  include <ActiveQt/QAxWidget>
#else
#  ifdef TTK_WEBKIT
#    if TTK_QT_VERSION_CHECK(5,0,0)
#      include <QtWebKitWidgets/QWebView>
#      include <QtWebKitWidgets/QWebFrame>
#    else
#      include <QtWebKit/QWebView>
#      include <QtWebKit/QWebFrame>
#    endif
#  elif defined TTK_WEBENGINE
#    include <QtWebEngineWidgets/QWebEngineView>
#    include <QtWebEngineWidgets/QWebEngineSettings>
#  endif
#endif

#include <QLabel>
#include <QTimer>
#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

/*! @brief The class of the kugou window widget private.
 * @author Greedysky <greedysky@163.com>
 */
class QKugouWindowPrivate : public TTKPrivate<QKugouWindow>
{
public:
    QKugouWindowPrivate();
    ~QKugouWindowPrivate();

    QWidget *m_webView, *m_topWidget;
    QButtonGroup *m_buttonGroup;
};

QKugouWindowPrivate::QKugouWindowPrivate()
{
    m_buttonGroup = nullptr;
    m_topWidget = nullptr;
    m_webView = nullptr;
}

QKugouWindowPrivate::~QKugouWindowPrivate()
{
    delete m_buttonGroup;
    delete m_topWidget;
    delete m_webView;
}



QKugouWindow::QKugouWindow(KuGouType type, QWidget *parent)
    : QWidget(parent)
{
    TTK_INIT_PRIVATE(QKugouWindow);

#ifdef Q_OS_UNIX
#  ifdef TTK_WEBKIT
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
#  elif defined TTK_WEBENGINE
    QWebEngineSettings *settings = QWebEngineSettings::defaultSettings();
    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
#  endif
#endif

    switch(type)
    {
        case KuGouSong: createKugouSongWidget(); break;
        case KuGouRadio: createKugouRadioWidget(); break;
        case KuGouList: createKugouListWidget(); break;
        case KugouMovie: createKugouMovieWidget(); break;
        case KuGouSingle: createKugouSingleWidget(); break;
        default: break;
    }
}

void QKugouWindow::setUrl(const QString &url)
{
    TTK_D(QKugouWindow);
#ifdef Q_OS_WIN
    QAxWidget *w = TTKObject_cast(QAxWidget*, d->m_webView);
    if(w)
    {
        w->dynamicCall("Navigate(const QString&)", url);
    }
#else
#  ifdef TTK_WEBKIT
    QWebView *w = TTKObject_cast(QWebView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
#  elif defined TTK_WEBENGINE
    QWebEngineView *w = TTKObject_cast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
#  else
    Q_UNUSED(url);
    Q_UNUSED(d);
#  endif
#endif
}

void QKugouWindow::goBack()
{
    TTK_D(QKugouWindow);
#ifdef Q_OS_WIN
    QTimer::singleShot(1, TTKObject_cast(QAxWidget*, d->m_webView), SLOT(GoBack()));
#else
#  ifdef TTK_WEBKIT
    QWebView *w = TTKObject_cast(QWebView*, d->m_webView);
    if(w)
    {
        w->back();
    }
#  elif defined TTK_WEBENGINE
    QWebEngineView *w = TTKObject_cast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->back();
    }
#  else
    Q_UNUSED(d);
#  endif
#endif
}

void QKugouWindow::refresh()
{
    TTK_D(QKugouWindow);
#ifdef Q_OS_WIN
    QTimer::singleShot(1, TTKObject_cast(QAxWidget*, d->m_webView), SLOT(Refresh()));
#else
#  ifdef TTK_WEBKIT
    QWebView *w = TTKObject_cast(QWebView*, d->m_webView);
    if(w)
    {
        w->reload();
    }
#  elif defined TTK_WEBENGINE
    QWebEngineView *w = TTKObject_cast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->reload();
    }
#  else
    Q_UNUSED(d);
#  endif
#endif
}

void QKugouWindow::kugouSongIndexChanged(int index)
{
    changeClickedButtonStyle(index);
    QString url = QKugouUrl::makeSongRecommendUrl();
    switch(index)
    {
        case 0: url = QKugouUrl::makeSongRecommendUrl(); break;
        case 1: url = QKugouUrl::makeSongRankUrl(); break;
        case 2: url = QKugouUrl::makeSongSingerUrl(); break;
        case 3: url = QKugouUrl::makeSongCategoryUrl(); break;
        default: break;
    }

    setUrl(url);
}

void QKugouWindow::kugouRadioIndexChanged(int index)
{
    changeClickedButtonStyle(index);
    QString url = QKugouUrl::makeRadioPublicUrl();
    switch(index)
    {
        case 0: url = QKugouUrl::makeRadioPublicUrl(); break;
        case 1: url = QKugouUrl::makeRadioHighFmUrl(); break;
        case 2: url = QKugouUrl::makeRadioFxUrl(); break;
        default: break;
    }

    setUrl(url);
}

void QKugouWindow::kugouMVIndexChanged(int index)
{
    changeClickedButtonStyle(index);
    QString url = QKugouUrl::makeMVRadioUrl();
    switch(index)
    {
        case 0: url = QKugouUrl::makeMVRadioUrl(); break;
        case 1: url = QKugouUrl::makeMVRecommendUrl(); break;
        case 2: url = QKugouUrl::makeMVFanxingUrl(); break;
        default: break;
    }

    setUrl(url);
}

void QKugouWindow::createWebViewer()
{
    TTK_D(QKugouWindow);
    if(d->m_webView)
    {
        delete d->m_webView;
    }
#ifdef Q_OS_WIN
    QAxWidget *view = new QAxWidget(this);
    view->setWindowFlags(Qt::FramelessWindowHint);
    view->setMouseTracking(false);
    view->setControl("{8856F961-340A-11D0-A96B-00C04FD705A2}"); //ie web view
    view->setObjectName("WebWidget");
    view->setFocusPolicy(Qt::StrongFocus);
    view->setProperty("DisplayAlerts", false);
    view->setProperty("DisplayScrollBars", false);
    view->setProperty("Silent", true);
    d->m_webView = view;
#else
#  ifdef TTK_WEBKIT
    QWebView *view = new QWebView(this);
    view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    d->m_webView = view;
#  elif defined TTK_WEBENGINE
    QWebEngineView *view = new QWebEngineView(this);
    d->m_webView = view;
#  endif
#endif
}

void QKugouWindow::createKugouSongWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle01 + MusicUIObject::MQSSWidgetStyle01);
    QHBoxLayout *topLayout = new QHBoxLayout(d->m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(25);

    d->m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" SongRecommend "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" SongRank "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" SongSinger "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 2);
    bt = new QPushButton(tr(" SongCategory "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 3);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(d->m_buttonGroup, SIGNAL(idClicked(int)), SLOT(kugouSongIndexChanged(int)));
#else
    connect(d->m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(kugouSongIndexChanged(int)));
#endif

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addWidget(d->m_buttonGroup->button(2));
    topLayout->addWidget(d->m_buttonGroup->button(3));
    topLayout->addStretch(1);

#ifdef Q_OS_UNIX
    d->m_buttonGroup->button(0)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(1)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(2)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(3)->setFocusPolicy(Qt::NoFocus);
#endif

    createWebViewer();
    layout->addWidget(d->m_topWidget);
    layout->addWidget(d->m_webView);
    kugouSongIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::createKugouRadioWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle01 + MusicUIObject::MQSSWidgetStyle01);
    QHBoxLayout *topLayout = new QHBoxLayout(d->m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(50);

    d->m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" RadioPublic "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" RadioHigh "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" RadioFx "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 2);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(d->m_buttonGroup, SIGNAL(idClicked(int)), SLOT(kugouRadioIndexChanged(int)));
#else
    connect(d->m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(kugouRadioIndexChanged(int)));
#endif

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addWidget(d->m_buttonGroup->button(2));
    topLayout->addStretch(1);

#ifdef Q_OS_UNIX
    d->m_buttonGroup->button(0)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(1)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(2)->setFocusPolicy(Qt::NoFocus);
#endif

    createWebViewer();
    layout->addWidget(d->m_topWidget);
    layout->addWidget(d->m_webView);
    kugouRadioIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::createKugouListWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    createWebViewer();
    layout->addWidget(d->m_webView);
#  ifdef Q_OS_WIN
    TTKObject_cast(QAxWidget*, d->m_webView)->dynamicCall("Navigate(const QString&)", QKugouUrl::makeListUrl());
#  else
#    ifdef TTK_WEBENGINE
    TTKObject_cast(QWebEngineView*, d->m_webView)->setUrl(QKugouUrl::makeListUrl());
#    else
    TTKObject_cast(QWebView*, d->m_webView)->setUrl(QKugouUrl::makeListUrl());
#    endif
#  endif
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::createKugouMovieWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle01 + MusicUIObject::MQSSWidgetStyle01);
    QHBoxLayout *topLayout = new QHBoxLayout(d->m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(50);

    d->m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" MVRadio "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" MVRecommend "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" MVFanxing "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 2);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(d->m_buttonGroup, SIGNAL(idClicked(int)), SLOT(kugouMVIndexChanged(int)));
#else
    connect(d->m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(kugouMVIndexChanged(int)));
#endif

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addWidget(d->m_buttonGroup->button(2));
    topLayout->addStretch(1);

#ifdef Q_OS_UNIX
    d->m_buttonGroup->button(0)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(1)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(2)->setFocusPolicy(Qt::NoFocus);
#endif

    createWebViewer();
    layout->addWidget(d->m_topWidget);
    layout->addWidget(d->m_webView);
    kugouMVIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::createKugouSingleWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    createWebViewer();
    layout->addWidget(d->m_webView);
    setUrl(QKugouUrl::makeKuiSheUrl());
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setStyleSheet("background:white;");
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::changeClickedButtonStyle(int index)
{
    TTK_D(QKugouWindow);
    for(int i = 0; i < d->m_buttonGroup->buttons().count(); ++i)
    {
        d->m_buttonGroup->button(i)->setStyleSheet(MusicUIObject::MQSSPushButtonStyle01);
    }
    d->m_buttonGroup->button(index)->setStyleSheet(MusicUIObject::MQSSPushButtonStyle02);
}
