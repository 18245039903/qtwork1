#ifndef WIDGET_H
#define WIDGET_H

#include "curve.h"
#include "tablewidget.h"
#include "ln_joint_feedback.h"
#include "waiting.h"

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QPushButton>
#include <QSettings>
#include <QApplication>
#include <QFile>
#include <QTextStream>
//#include <QTime>

QT_BEGIN_NAMESPACE
namespace ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    int x;

public slots:
    void socket_Read_Data1();
    void plot();
    void restore1();
    void drawCurve(int a);
    void drawCurve1(int a);
    void pushButton_Connect1();
    void socket_Disconnected();
    void replot();
    void set_Item();
    void Load();

private:
    uint16_t timebegin;
    Curve *c;
    TableWidget *t;
    QLineEdit *port;
    QTcpSocket *socket;
    QPushButton *pushButton_Connect;
    QPushButton *load;
    QPushButton *replot1;
    QLineEdit *line_IP;
    topic_joint_feedback_t *feedback;
    QSharedPointer<QCPGraphDataContainer> dataContainer[154];
    QVector<QCPGraphData> mData[7][22];
    QStringList input[7];
    QFile f;
    QStringList fileName;
    QTimer pT;
    QTimer pT1;
    QVector<double> dbfeedback[7][22];
    int col;
    QHBoxLayout *layout;
    Waiting *wait;
};
#endif // WIDGET_H
