#include "waiting.h"

#include <QPixmap>

Waiting::Waiting(QWidget *parent)
    :QDialog(parent)
    ,movie(nullptr)
{
    setWindowOpacity(0.5);
    setMinimumSize(200, 100);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    //background = new QFrame(this);
    dialog = new QLabel(this);
    //background->setMinimumSize(200, 200);
    //background->setStyleSheet("background-color: transparent;");
    //movie = new QImage(":/new/prefix1/timg.jpeg");
    movie = new QMovie(":/new/prefix1/loading3.gif");
    movie->setScaledSize(QSize(200, 100));
    //dialog->setPixmap(QPixmap::fromImage(*movie));
    dialog->setMovie(movie);
    dialog->setScaledContents(true);
    //dialog->show();
    movie->start();
}

Waiting::~Waiting()
{
    //movie->stop();
    //delete background;
    delete movie;
    delete dialog;
}
