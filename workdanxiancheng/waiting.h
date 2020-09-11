#ifndef WAITING_H
#define WAITING_H

#include <QDialog>
#include <QLabel>
#include <QMovie>


class Waiting : public QDialog
{
public:
    Waiting(QWidget *parent = nullptr);
    ~Waiting();

private:
    QImage *movie;
    QLabel *dialog;
    QFrame *background;
};

#endif // WAITING_H
