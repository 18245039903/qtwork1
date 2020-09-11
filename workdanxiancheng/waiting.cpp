#include "waiting.h"

#include <QPixmap>

Waiting::Waiting(QWidget *parent)
    :QDialog(parent)
    ,movie(nullptr)
{
    setWindowOpacity(0.8);
    setMinimumSize(700, 700);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    background = new QFrame(this);
    dialog = new QLabel(background);
    background->setMinimumSize(700, 700);
    background->setStyleSheet("background-color: transparent;");
    movie = new QImage(":/new/prefix1/timg.jpeg");
    //movie->setScaledSize(QSize(500, 500));
    dialog->setPixmap(QPixmap::fromImage(*movie));
    dialog->setScaledContents(true);
    dialog->show();
    //movie->start();
}

Waiting::~Waiting()
{
    //movie->stop();
    delete background;
    delete movie;
    //delete dialog;
}
