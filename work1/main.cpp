#include "CommandBar.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CommandBar w;
    w.show();
    return a.exec();
}
