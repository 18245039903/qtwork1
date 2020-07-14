#include <QMainWindow>
#include <QTableWidget>

#ifndef COMMANDBAR_H
#define COMMANDBAR_H

#endif // COMMANDBAR_H

class CommandBar : public QMainWindow
{
    Q_OBJECT

public:
    CommandBar(QMainWindow* parent = nullptr);
    ~CommandBar();

public:
    void openfile();

protected:
    QTableWidget *m_pTableWidget;
};

/*class CWidget : public QWidget
{

};

class Table : public QTableWidget
{
    Q_OBJECT

public:
    Table(QTableWidget* parent = nullptr);
    ~Table();

signals:
    void xuanlie();

protected slots:
    void quxian();
};*/
