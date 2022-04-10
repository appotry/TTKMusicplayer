#include "musicsourceupdatewidget.h"
#include "ui_musicsourceupdatewidget.h"
#include "musicsourceupdaterequest.h"
#include "musicsettingmanager.h"
#include "musicalgorithmutils.h"
#include "musiccoreutils.h"
#include "musicurlutils.h"

#include "qsync/qsyncutils.h"

#include <QBoxLayout>

MusicSourceUpdateNotifyWidget::MusicSourceUpdateNotifyWidget(QWidget *parent)
    : MusicAbstractMoveSingleWidget(true, parent)
{
    hide();

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle12);
    setMouseTracking(true);
    blockMoveOption(true);

    const QSize &windowSize = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    setGeometry(windowSize.width() - 240, windowSize.height() - 150, 220, 100);

    QVBoxLayout *vlayout = new QVBoxLayout(m_container);
    vlayout->setContentsMargins(5, 5, 5, 5);
    m_textLabel = new QLabel(this);
    m_textLabel->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    m_textLabel->setAlignment(Qt::AlignCenter);

    QWidget *contain = new QWidget(this);
    QHBoxLayout *hlayout = new QHBoxLayout(contain);
    hlayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *updateButton = new QPushButton(tr("Update"), contain);
    QPushButton *nextTimeButton = new QPushButton(tr("Close"), contain);
    updateButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    nextTimeButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    updateButton->setFixedSize(85, 27);
    nextTimeButton->setFixedSize(85, 27);
    hlayout->addWidget(updateButton);
    hlayout->addWidget(nextTimeButton);
    contain->setLayout(hlayout);

#ifdef Q_OS_UNIX
    updateButton->setFocusPolicy(Qt::NoFocus);
    nextTimeButton->setFocusPolicy(Qt::NoFocus);
#endif

    vlayout->addWidget(m_textLabel);
    vlayout->addWidget(contain);
    m_container->setLayout(vlayout);

    connect(updateButton, SIGNAL(clicked()), SLOT(updateSourceClicked()));
    connect(nextTimeButton, SIGNAL(clicked()), SLOT(close()));
}

MusicSourceUpdateNotifyWidget::~MusicSourceUpdateNotifyWidget()
{
    delete m_textLabel;
}

void MusicSourceUpdateNotifyWidget::start()
{
    MusicSourceUpdateRequest *download = new MusicSourceUpdateRequest(this);
    connect(download, SIGNAL(downLoadDataChanged(QVariant)), SLOT(downLoadFinished(QVariant)));
    download->startToDownload();
}

void MusicSourceUpdateNotifyWidget::updateSourceClicked()
{
    close();
    MusicSourceUpdateWidget().exec();
}

void MusicSourceUpdateNotifyWidget::downLoadFinished(const QVariant &bytes)
{
    const QVariantMap &value = bytes.toMap();
    const QString &versionStr = value["version"].toString();

    if(MusicUtils::Core::appVersionCheck(TTK_VERSION_STR, versionStr))
    {
        show();
        m_textLabel->setText(tr("New version found") + "\r\n" + versionStr);
    }
    else
    {
        close();
    }
}



MusicSourceUpdateWidget::MusicSourceUpdateWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSourceUpdateWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->upgradeButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    m_ui->upgradeButton->setEnabled(false);
#ifdef Q_OS_UNIX
    m_ui->upgradeButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->upgradeButton, SIGNAL(clicked()), SLOT(upgradeButtonClicked()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
}

MusicSourceUpdateWidget::~MusicSourceUpdateWidget()
{
    delete m_ui;
}

void MusicSourceUpdateWidget::start()
{
    MusicSourceUpdateRequest *download = new MusicSourceUpdateRequest(this);
    connect(download, SIGNAL(downLoadDataChanged(QVariant)), SLOT(downLoadFinished(QVariant)));
    download->startToDownload();
}

void MusicSourceUpdateWidget::upgradeButtonClicked()
{
    MusicUtils::Url::openUrl(MusicUtils::Algorithm::mdII(RELEASE_DATA_URL, false), false);
}

void MusicSourceUpdateWidget::downLoadFinished(const QVariant &bytes)
{
    const QVariantMap &value = bytes.toMap();
    const QString &version = value["version"].toString();

    QString text;
    if(MusicUtils::Core::appVersionCheck(TTK_VERSION_STR, version))
    {
        text.append("v" + version);
        text.append("\r\n");
        text.append(value["data"].toString());

        m_ui->upgradeButton->setEnabled(true);
        m_ui->titleLable->move(50, 0);
        m_ui->titleLable->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    }
    else
    {
        text.append(tr("Current version is updated!"));
        m_ui->titleLable->move(0, 0);
        m_ui->titleLable->setAlignment(Qt::AlignCenter);
    }
    m_ui->titleLable->setText(text);
}

int MusicSourceUpdateWidget::exec()
{
    start();
    return MusicAbstractMoveDialog::exec();
}
