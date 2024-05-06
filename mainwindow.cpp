#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    imageLabel = new QLabel(this);
    closeButton = new QPushButton("x", this);

    player->setVideoOutput(videoWidget);

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia) {
            image = new QImage("file:///F:/QT_Task/logo.png");
            imageLabel->setPixmap(QPixmap::fromImage(*image));
            imageLabel->resize(image->size());
            imageLabel->show();
            closeButton->move(imageLabel->width() - closeButton->width(), 0);
            closeButton->show();
        }
    });

    connect(closeButton, &QPushButton::clicked, this, &MainWindow::imageClosed);
    videoWidget->installEventFilter(this);

    setCentralWidget(videoWidget);
    player->setMedia(QUrl::fromLocalFile("file:///F:/QT_Task/video.mp4"));
    player->play();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == videoWidget && event->type() == QEvent::MouseButtonDblClick) {
        videoDoubleClicked();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::videoDoubleClicked()
{
    if (player->state() == QMediaPlayer::PlayingState)
        player->pause();
    else if (player->state() == QMediaPlayer::PausedState)
        player->play();
}

void MainWindow::imageClosed()
{
    delete image;
    imageLabel->clear();
    imageLabel->hide();
    closeButton->hide();
}

