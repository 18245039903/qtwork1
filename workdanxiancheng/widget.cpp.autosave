#include "widget.h"
#include "curve.h"
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , c(new Curve)
    , t(new TableWidget)
{
    socket = new QTcpSocket();
    feedback = new topic_joint_feedback_t();
    wait = new Waiting(this);
    load = new QPushButton("选择数据保存位置");
    port = new QLineEdit("1234");
    pushButton_Connect = new QPushButton("连接");
    replot1 = new QPushButton("replot");
    line_IP = new QLineEdit("192.168.10.75");
    layout = new QHBoxLayout();
    QVBoxLayout *layout1 = new QVBoxLayout();

    pushButton_Connect->setFixedSize(80, 25);
    replot1->setFixedSize(80, 25);
    port->setFixedSize(100, 25);
    line_IP->setFixedSize(100, 25);
    layout->addWidget(t);
    layout1->addWidget(line_IP, 0, Qt::AlignCenter);
    layout1->addWidget(port, 0, Qt::AlignCenter);
    layout1->addWidget(pushButton_Connect, 0, Qt::AlignCenter);
    layout1->addWidget(load, 0, Qt::AlignCenter);
    layout1->addWidget(replot1, 0, Qt::AlignCenter);
    replot1->setDisabled(true);
    load->setDisabled(true);
    layout->addLayout(layout1);
    layout->addWidget(c);
    layout->setStretch(0, 1);
    layout->setStretch(2, 1);
    setLayout(layout);

    connect(t->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(drawCurve(int)));
    connect(t->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(drawCurve1(int)));
    connect(replot1, SIGNAL(clicked()), this, SLOT(replot()));
    connect(load, SIGNAL(clicked()), this, SLOT(Load()));
    connect(socket, &QTcpSocket::readyRead, this, &Widget::socket_Read_Data1);
    connect(socket, &QTcpSocket::disconnected, this, &Widget::socket_Disconnected);
    connect(pushButton_Connect, SIGNAL(clicked()), this, SLOT(pushButton_Connect1()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(set_Item()));
    connect(&pT1, &QTimer::timeout, c, &Curve::replot2);
    connect(&pT1, &QTimer::timeout, this, &Widget::set_Item);
}

Widget::~Widget()
{
    t->clear();
    delete port;
    delete pushButton_Connect;
    delete replot1;
    delete line_IP;
    delete load;
    delete c;
    delete t;
    delete wait;
    socket->close();
    socket->deleteLater();
    delete feedback;
    delete layout;
}

void Widget::Load()
{

    QStringList fileName = QFileDialog::getOpenFileNames(this,tr("选择日志文件"),"",tr("TXT(*.txt)"));
    if(fileName.count() != 7)
    {
        QMessageBox::critical(this, tr("replot"),
                                    tr("请选择7个文本文件！"),
                                    QMessageBox::Ok);
        return;
    }
    wait->show();
    for(int q = 0; q < 7; q++)
    {
        if (fileName[q].isEmpty())
                return;
        QFile f(fileName[q]);
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Open failed";
            return;
        }
        for (int t = 0;t <input[q].count();t++)
        {
            f.write(input[q][t].toUtf8());
        }
        f.close();
        input[q].clear();
    }
    wait->close();
    return;
}

void Widget::pushButton_Connect1()
{
    if(pushButton_Connect->text() == tr("连接"))
    {
        QString Ip1;
        quint16 Port;
        Ip1 = line_IP->text();
        Port = port->text().toUShort();
        socket->abort();
        socket->connectToHost(Ip1, Port);
        if(!socket->waitForConnected(30000))
        {
            qDebug() << "Connection failed!";
            return;
        }
        qDebug() << "Connection successfully!";
        pT.start(1);
        pT1.start(20);
        pushButton_Connect->setText("断开连接");
        disconnect(c, SIGNAL(mouseDoubleClick(QMouseEvent *)), this, SLOT(restore1()));
        replot1->setDisabled(true);
        load->setDisabled(true);
        c->setSelectionRectMode(QCP::srmNone);
    }
    else
    {
        replot1->setDisabled(false);
        load->setDisabled(false);
        socket->disconnectFromHost();
        pushButton_Connect->setText("连接");
        pT.stop();
        pT1.stop();
        connect(c, SIGNAL(mouseDoubleClick(QMouseEvent *)), this, SLOT(restore1()));
        //pushButton_Connect->setDisabled(true);
    }
}

void Widget::socket_Disconnected()
{
    restore1();
    qDebug() <<"Disconnected!";
}

void Widget::socket_Read_Data1()
{
    qint64 l = socket->size();
    qDebug() << l;
    l = l / 784;
    qDebug() << l;
    while(l-- > 0)
    {
        qint64 i = socket->read((char*)feedback, sizeof(*feedback));
        //qDebug() << i;
        if(i > 0)
        {
            for(int q = 0; q < 7; q++)
            {
                input[q].append(QString::number(feedback->error_register[q]));
                dbfeedback[q][0].append(feedback->error_register[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->modes_of_operation_display[q]));
                dbfeedback[q][1].append(feedback->modes_of_operation_display[q]);
                input[q].append(",");
                input[q].append(QString::number(long(feedback->motor_q_axis_current_demand[q])));
                dbfeedback[q][2].append(long(feedback->motor_q_axis_current_demand[q]));
                input[q].append(",");
                input[q].append(QString::number(long(feedback->friction_estimate[q])));
                dbfeedback[q][3].append(long(feedback->friction_estimate[q]));
                input[q].append(",");
                input[q].append(QString::number(long(feedback->acceleration_based_on_link_encoder[q])));
                dbfeedback[q][4].append(long(feedback->acceleration_based_on_link_encoder[q]));
                input[q].append(",");
                input[q].append(QString::number(long(feedback->acceleration_based_on_motor_encoder[q])));
                dbfeedback[q][5].append(long(feedback->acceleration_based_on_motor_encoder[q]));
                input[q].append(",");
                input[q].append(QString::number(feedback->tx_count[q]));
                dbfeedback[q][6].append(feedback->tx_count[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->statusword[q]));
                dbfeedback[q][7].append(feedback->statusword[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->position_demand_value[q]));
                dbfeedback[q][8].append(feedback->position_demand_value[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->position_value_based_on_motor_encoder[q]));
                dbfeedback[q][9].append(feedback->position_value_based_on_motor_encoder[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->position_value_based_on_link_encoder[q]));
                dbfeedback[q][10].append(feedback->position_value_based_on_link_encoder[q]);
                input[q].append(",");
                input[q].append(QString::number(long(feedback->speed_demand_of_motor_side[q])));
                dbfeedback[q][11].append(long(feedback->speed_demand_of_motor_side[q]));
                input[q].append(",");
                input[q].append(QString::number(long(feedback->speed_based_on_motor_encoder[q])));
                dbfeedback[q][12].append(long(feedback->speed_based_on_motor_encoder[q]));
                input[q].append(",");
                input[q].append(QString::number(long(feedback->speed_based_on_link_encoder[q])));
                dbfeedback[q][13].append(long(feedback->speed_based_on_link_encoder[q]));
                input[q].append(",");
                input[q].append(QString::number(feedback->torque_actual_value_a[q]));
                dbfeedback[q][14].append(feedback->torque_actual_value_a[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->torque_actual_value_b[q]));
                dbfeedback[q][15].append(feedback->torque_actual_value_b[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->torque_actual_value_a_derivate[q]));
                dbfeedback[q][16].append(feedback->torque_actual_value_a_derivate[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->torque_actual_value_b_derivate[q]));
                dbfeedback[q][17].append(feedback->torque_actual_value_b_derivate[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->following_error_actual_value[q]));
                dbfeedback[q][18].append(feedback->following_error_actual_value[q]);
                input[q].append(",");
                input[q].append(QString::number(long(feedback->speed_control_integral_value[q])));
                dbfeedback[q][19].append(long(feedback->speed_control_integral_value[q]));
                input[q].append(",");
                input[q].append(QString::number(feedback->digital_inputs[q]));
                dbfeedback[q][20].append(feedback->digital_inputs[q]);
                input[q].append(",");
                input[q].append(QString::number(feedback->current_position_actual_value[q]));
                dbfeedback[q][21].append(feedback->current_position_actual_value[q]);
                input[q].append("\n");
            }
            plot();
        }
    }
}

void Widget::plot()
{
    static double time = 0;
    if(c->graphCount() == 0)
    {
        for (int r = 0;r < 7;r++)
        {
            for(int q = 0; q < 22; q++)
            {
                c->addGraph();
                dataContainer[r * 22 + q] = c->graph(r * 22 + q)->data();
                c->graph(r * 22 + q)->setVisible(0);
            }
        }
    }
    if(mData[0][0].isEmpty())
    {
        time = 0;
    }
    time += 0.001;
    qDebug() << time;
    for (int r = 0;r < 7;r++)
    {
        for(int q = 0; q < 22; q++)
        {
            c->graph(r * 22 + q)->addData(time, dbfeedback[r][q].last());
            QCPGraphData newPoint;
            newPoint.key = time;
            newPoint.value = dbfeedback[r][q].last();
            mData[r][q].append(newPoint);
            if(time > 1.25)
            {
                c->graph(r * 22 + q)->data()->removeBefore(time - 1.25);
            }
        }
    }
    c->rescaleAxes(true);
    if(time < 1.25)
    {
        c->xAxis->setRange(0, 1.25);
    }
    //c->replot(QCustomPlot::rpQueuedReplot);
    return;
}

void Widget::restore1()
{
    for (int r = 0;r < 7;r++)
    {
        for(int q = 0; q < 22; q++)
        {
            dataContainer[r * 22 + q]->set(mData[r][q],true);
        }
    }
    c->setSelectionRectMode(QCP::srmZoom);
    c->rescaleAxes(true);
    c->replot();
    return;
}

void Widget::replot()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical(this, tr("replot"),
                                    tr("重绘后，之前的数据将被清除！"),
                                    QMessageBox::Ok | QMessageBox::No);
    if(reply == QMessageBox::Ok)
    {

        wait->show();
        c->replot1();
        t->replot1();

        replot1->setDisabled(true);
        load->setDisabled(true);
        pushButton_Connect->setDisabled(false);
        disconnect(c, SIGNAL(mouseDoubleClick(QMouseEvent *)), this, SLOT(restore1()));
        c->setSelectionRectMode(QCP::srmNone);
        for (int r = 0;r < 7; r++)
        {
            input[r].clear();
            for(int q = 0; q < 22; q++)
            {
                mData[r][q].clear();
                dbfeedback[r][q].clear();
                QCoreApplication::processEvents();
            }

        }
        wait->close();
        replot1->setDisabled(true);
    }
    return;
}

void Widget::set_Item()
{
    if(feedback != nullptr)
    {
        for(int q = 0; q < 7; q++)
        {
            t->setItem( 0, q, new QTableWidgetItem(QString::number(feedback->error_register[q], 10)));
            t->setItem( 1, q, new QTableWidgetItem(QString::number(feedback->modes_of_operation_display[q], 10)));
            t->setItem( 2, q, new QTableWidgetItem(QString::number(long(feedback->motor_q_axis_current_demand[q]), 10)));
            t->setItem( 3, q, new QTableWidgetItem(QString::number(long(feedback->friction_estimate[q]), 10)));
            t->setItem( 4, q, new QTableWidgetItem(QString::number(long(feedback->acceleration_based_on_link_encoder[q]), 10)));
            t->setItem( 5, q, new QTableWidgetItem(QString::number(long(feedback->acceleration_based_on_motor_encoder[q]), 10)));
            t->setItem( 6, q, new QTableWidgetItem(QString::number(feedback->tx_count[q], 10)));
            t->setItem( 7, q, new QTableWidgetItem(QString::number(feedback->statusword[q], 10)));
            t->setItem( 8, q, new QTableWidgetItem(QString::number(feedback->position_demand_value[q], 10, 6)));
            t->setItem( 9, q, new QTableWidgetItem(QString::number(feedback->position_value_based_on_motor_encoder[q], 10, 6)));
            t->setItem(10, q, new QTableWidgetItem(QString::number(feedback->position_value_based_on_link_encoder[q], 10, 6)));
            t->setItem(11, q, new QTableWidgetItem(QString::number(long(feedback->speed_demand_of_motor_side[q]), 10)));
            t->setItem(12, q, new QTableWidgetItem(QString::number(long(feedback->speed_based_on_motor_encoder[q]), 10)));
            t->setItem(13, q, new QTableWidgetItem(QString::number(long(feedback->speed_based_on_link_encoder[q]), 10)));
            t->setItem(14, q, new QTableWidgetItem(QString::number(feedback->torque_actual_value_a[q], 10, 6)));
            t->setItem(15, q, new QTableWidgetItem(QString::number(feedback->torque_actual_value_b[q], 10, 6)));
            t->setItem(16, q, new QTableWidgetItem(QString::number(feedback->torque_actual_value_a_derivate[q], 10, 6)));
            t->setItem(17, q, new QTableWidgetItem(QString::number(feedback->torque_actual_value_b_derivate[q], 10, 6)));
            t->setItem(18, q, new QTableWidgetItem(QString::number(feedback->following_error_actual_value[q], 10, 6)));
            t->setItem(19, q, new QTableWidgetItem(QString::number(long(feedback->speed_control_integral_value[q]), 10)));
            t->setItem(20, q, new QTableWidgetItem(QString::number(feedback->digital_inputs[q], 10)));
            t->setItem(21, q, new QTableWidgetItem(QString::number(feedback->current_position_actual_value[q], 10, 6)));
        }
        t->show();
    }
    return;
}

void Widget::drawCurve(int a)
{
    if(a > 21 || c->graphCount() == 0)
    {
        return;
    }
    for (int i = 0;i < c->graphCount();i++)
    {
        if(c->graph(i)->visible())
        {
            i = i / 22;
            if(c->graph(i * 22 + a)->visible())
            {
                c->graph(i * 22 + a)->setVisible(0);
                c->rescaleAxes(true);
                c->replot();
                t->item(a, col)->setBackgroundColor(QColor(255, 255, 255));
                t->show();
            }
            else
            {
                c->graph(i * 22 + a)->setVisible(1);
                c->rescaleAxes(true);
                c->replot();
                t->item(a, col)->setBackgroundColor(QColor(100, 100, 100));
                t->show();
            }
            return;
        }
    }
}

void Widget::drawCurve1(int a)
{
    if(a > 6 || c->graphCount() == 0)
    {
        return;
    }
    col = a;
    for (int r = 0;r < 7; r++)
    {
        for(int q = 0; q < 22; q++)
        {
            c->graph(r * 22 + q)->setVisible(0);
            t->item(q, r)->setBackgroundColor(QColor(255, 255, 255));
        }
    }
    for (int o = 0;o < 22;o ++)
    {
        c->graph(a * 22 + o)->setVisible(1);
        t->item(o, a)->setBackgroundColor(QColor(100, 100, 100));
        t->show();
    }
    c->rescaleAxes(true);
    c->replot();
}
