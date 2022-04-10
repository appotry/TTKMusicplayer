#include "musicprogresswidget.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundconfigmanager.h"
#include "musicextractwrapper.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"

MusicProgressWidget::MusicProgressWidget(QWidget *parent)
    : QProgressDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_TranslucentBackground);

    setCancelButton(nullptr);
    setLabel(nullptr);
    setFixedSize(368, 123);
    setMinimumDuration(2);

    initialize();
}

MusicProgressWidget::~MusicProgressWidget()
{
    delete m_background;
    delete m_textLabel;
    delete m_progressBar;
}

void MusicProgressWidget::initialize()
{
    m_background = new QLabel(this);
    m_background->setGeometry(4, 4, 360, 115);
    QWidget *backgroundMask = new QWidget(this);
    backgroundMask->setGeometry(4, 29, 360, 90);
    backgroundMask->setStyleSheet(MusicUIObject::MQSSBackgroundStyle10);

    QLabel *topTitleName = new QLabel(this);
    topTitleName->setText(tr("Progress Bar"));
    topTitleName->setGeometry(14, 4, 221, 25);
    topTitleName->setStyleSheet(MusicUIObject::MQSSColorStyle01 + MusicUIObject::MQSSFontStyle01);

    QToolButton *topTitleCloseButton = new QToolButton(this);
    topTitleCloseButton->setGeometry(344, 6, 20, 20);
    topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    topTitleCloseButton->setEnabled(false);

    m_progressBar = new QProgressBar(this);
    setBar(m_progressBar);
    m_progressBar->setStyleSheet(MusicUIObject::MQSSProgressBar01);

    m_textLabel = new QLabel(this);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setGeometry(24, 34, 320, 25);
}

void MusicProgressWidget::setTitle(const QString &name)
{
    m_textLabel->setText(name);
}

void MusicProgressWidget::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    m_progressBar->setGeometry(24, 69, 320, 25);
}

void MusicProgressWidget::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    QPainter painter(this);
    MusicUtils::Widget::setBorderShadow(this, &painter);
}

void MusicProgressWidget::show()
{
    MusicBackgroundImage image;
    if(MusicExtractWrapper::outputSkin(&image, G_BACKGROUND_PTR->backgroundUrl()))
    {
        m_background->setPixmap(image.m_pix.scaled(size()));
    }
    else
    {
        m_background->setPixmap(QPixmap(G_BACKGROUND_PTR->backgroundUrl()).scaled(size()));
    }
    QProgressDialog::show();
}
