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
    //replot1 = new QPushButton("重绘");
    line_IP = new QLineEdit("192.168.10.75");
    layout = new QHBoxLayout();
    QVBoxLayout *layout1 = new QVBoxLayout();

    pushButton_Connect->setFixedSize(80, 25);
    //replot1->setFixedSize(80, 25);
    port->setFixedSize(100, 25);
    line_IP->setFixedSize(100, 25);
    layout->addWidget(t);
    layout1->addWidget(line_IP, 0, Qt::AlignCenter);
    layout1->addWidget(port, 0, Qt::AlignCenter);
    layout1->addWidget(pushButton_Connect, 0, Qt::AlignCenter);
    layout1->addWidget(load, 0, Qt::AlignCenter);
    //layout1->addWidget(replot1, 0, Qt::AlignCenter);
    //replot1->setDisabled(true);
    load->setDisabled(true);
    layout->addLayout(layout1);
    layout->addWidget(c);
    layout->setStretch(0, 1);
    layout->setStretch(2, 1);
    setLayout(layout);

    connect(t->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(drawCurve(int)));
    connect(t->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(drawCurve1(int)));
    //connect(replot1, SIGNAL(clicked()), this, SLOT(replot()));
    connect(load, SIGNAL(clicked()), this, SLOT(Load()));
    connect(socket, &QTcpSocket::readyRead, this, &Widget::socket_Read_Data1);
    connect(socket, &QTcpSocket::disconnected, this, &Widget::socket_Disconnected);
    connect(pushButton_Connect, SIGNAL(clicked()), this, SLOT(pushButton_Connect1()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(set_Item()));
    connect(&pT1, &QTimer::timeout, c, &Curve::replot2);
    connect(&pT1, &QTimer::timeout, this, &Widget::set_Item);

    initprivate();
}

Widget::~Widget()
{
    t->clear();
    delete port;
    delete pushButton_Connect;
    //delete replot1;
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

    QString filePath = QFileDialog::getExistingDirectory(this,tr("选择文件目录"),tr("/"));
    qDebug() << filePath;
    if(filePath.isEmpty())
    {
        return;
    }
    else
    {
        fileName << filePath + "/joint1.txt"
                 << filePath + "/joint2.txt"
                 << filePath + "/joint3.txt"
                 << filePath + "/joint4.txt"
                 << filePath + "/joint5.txt"
                 << filePath + "/joint6.txt"
                 << filePath + "/joint7.txt";
    }
    wait->show();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical(this, tr("保存"),
                                tr("文件已存在内容将被清除！"),
                                QMessageBox::Ok | QMessageBox::No);
    if(reply == QMessageBox::No)
    {
        return;
    }
    for (int r = 0;r < 7;r++)
    {
        QFile f(fileName[r]);
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Open failed";
            return;
        }
        for(int q = 0; q < 22; q++)
        {
            f.write(t->header1.at(q).toUtf8());
            f.write(",");
        }
        f.write("time");
        f.write("\n");
        for (int t = 0;t < dbfeedback[0][0].count();t ++)
        {
            for(int q = 0; q < 22; q++)
            {
                f.write(QString::number(dbfeedback[r][q][t]).toUtf8());
                f.write(",");
                QCoreApplication::processEvents();
            }
            f.write(input[r][t].toString().toUtf8());
            f.write(".");
            f.write(msec[t].toUtf8());
            f.write("\n");
        }
        f.close();
    }
    wait->close();
    return;
}

void Widget::pushButton_Connect1()
{
    if(pushButton_Connect->text() == tr("连接"))
    {
        if(c->graphCount())
        {
            replot();
            if(reconnectflag == 1)
            {
                reconnectflag = 0;
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
                //replot1->setDisabled(true);
                load->setDisabled(true);
                c->setSelectionRectMode(QCP::srmNone);
            }
            else
            {
                return;
            }
        }
        else
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
            //replot1->setDisabled(true);
            load->setDisabled(true);
            c->setSelectionRectMode(QCP::srmNone);
        }
    }
    else
    {
        //replot1->setDisabled(false);
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
    l = l / 784;
    static QTime stamp(0,0,0,0);
    if(dbfeedback[0][0].isEmpty())
    {
        QTime stamp1(0,0,0,0);
        stamp = stamp1;
    }
    while(l-- > 0)
    {
        qDebug() << stamp;
        qint64 i = socket->read((char*)feedback, sizeof(*feedback));
        //qDebug() << i;
        if(i > 0)
        {
            for(int q = 0; q < 7; q++)
            {
//                input[q].append(QString::number(feedback->error_register[q]));
                dbfeedback[q][0].append(feedback->error_register[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->modes_of_operation_display[q]));
                dbfeedback[q][1].append(feedback->modes_of_operation_display[q]);
//                input[q].append(",");
//                input[q].append(QString::number(long(feedback->motor_q_axis_current_demand[q])));
                dbfeedback[q][2].append(long(feedback->motor_q_axis_current_demand[q]));
//                input[q].append(",");
//                input[q].append(QString::number(long(feedback->friction_estimate[q])));
                dbfeedback[q][3].append(long(feedback->friction_estimate[q]));
//                input[q].append(",");
//                input[q].append(QString::number(long(feedback->acceleration_based_on_link_encoder[q])));
                dbfeedback[q][4].append(long(feedback->acceleration_based_on_link_encoder[q]));
//                input[q].append(",");
//                input[q].append(QString::number(long(feedback->acceleration_based_on_motor_encoder[q])));
                dbfeedback[q][5].append(long(feedback->acceleration_based_on_motor_encoder[q]));
//                input[q].append(",");
//                input[q].append(QString::number(feedback->tx_count[q]));
                dbfeedback[q][6].append(feedback->tx_count[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->statusword[q]));
                dbfeedback[q][7].append(feedback->statusword[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->position_demand_value[q]));
                dbfeedback[q][8].append(feedback->position_demand_value[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->position_value_based_on_motor_encoder[q]));
                dbfeedback[q][9].append(feedback->position_value_based_on_motor_encoder[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->position_value_based_on_link_encoder[q]));
                dbfeedback[q][10].append(feedback->position_value_based_on_link_encoder[q]);
//                input[q].append(",");
//                input[q].append(QString::number(long(feedback->speed_demand_of_motor_side[q])));
                dbfeedback[q][11].append(long(feedback->speed_demand_of_motor_side[q]));
//                input[q].append(",");
//                input[q].append(QString::number(long(feedback->speed_based_on_motor_encoder[q])));
                dbfeedback[q][12].append(long(feedback->speed_based_on_motor_encoder[q]));
//                input[q].append(",");
//                input[q].append(QString::number(long(feedback->speed_based_on_link_encoder[q])));
                dbfeedback[q][13].append(long(feedback->speed_based_on_link_encoder[q]));
//                input[q].append(",");
//                input[q].append(QString::number(feedback->torque_actual_value_a[q]));
                dbfeedback[q][14].append(feedback->torque_actual_value_a[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->torque_actual_value_b[q]));
                dbfeedback[q][15].append(feedback->torque_actual_value_b[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->torque_actual_value_a_derivate[q]));
                dbfeedback[q][16].append(feedback->torque_actual_value_a_derivate[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->torque_actual_value_b_derivate[q]));
                dbfeedback[q][17].append(feedback->torque_actual_value_b_derivate[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->following_error_actual_value[q]));
                dbfeedback[q][18].append(feedback->following_error_actual_value[q]);
//                input[q].append(",");
//                input[q].append(QString::number(long(feedback->speed_control_integral_value[q])));
                dbfeedback[q][19].append(long(feedback->speed_control_integral_value[q]));
//                input[q].append(",");
//                input[q].append(QString::number(feedback->digital_inputs[q]));
                dbfeedback[q][20].append(feedback->digital_inputs[q]);
//                input[q].append(",");
//                input[q].append(QString::number(feedback->current_position_actual_value[q]));
                dbfeedback[q][21].append(feedback->current_position_actual_value[q]);
                input[q] << stamp;
            }
            plot();
        }
        stamp = stamp.addMSecs(1);
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
                c->graph(r * 22 + q)->removeFromLegend();
                dataContainer[r * 22 + q] = c->graph(r * 22 + q)->data();
                c->graph(r * 22 + q)->setVisible(0);
//                QColor color;
//                color.setHsv(r * 60,255,255);
//                c->graph(r * 22 + q)->setPen(color);
            }
        }
    }
    if(mData[0][0].isEmpty())
    {
        time = 0;
    }
    time += 0.001;
    msec.append(QString::number(int(time * 1000) % 1000));
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
            if(time > 2)
            {
                c->graph(r * 22 + q)->data()->removeBefore(time - 2);
            }
        }
    }
    c->rescaleAxes(true);
    if(time < 2)
    {
        c->xAxis->setRange(0, 2);
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
    c->replot(QCustomPlot::rpQueuedReplot);
    return;
}

void Widget::replot()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical(this, tr("reconnect"),
                                    tr("重新连接后，之前的数据将被清除！"),
                                    QMessageBox::Ok | QMessageBox::No);
    if(reply == QMessageBox::Ok)
    {
        reconnectflag = 1;
        wait->show();
        for(int i = 0;i < set2.count();i ++)
        {
            QList<int> list2 = set2.toList();
            QTableWidgetItem *item = new QTableWidgetItem(t->header1.at(list2[i]));
            t->setVerticalHeaderItem(list2[i], item);
        }
        for(int i = 0;i < set1.count();i ++)
        {
            QList<int> list1 = set1.toList();
            QTableWidgetItem *item = new QTableWidgetItem(t->header2.at(list1[i]));
//            QColor color;
//            color.setHsv(list1[i] * 60,255,255);
//            item->setTextColor(color);
            t->setHorizontalHeaderItem(list1[i], item);
        }
        set1.clear();
        set2.clear();
        flag1 = 0;
        flag2 = 0;
        c->replot1();
        t->replot1();
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
        //replot1->setDisabled(true);
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
        //t->show();
    }
    return;
}

void Widget::drawCurve(int a)
{
    if(c->graphCount() == 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Warning"),
                                        tr("请先连接！"),
                                     QMessageBox::Ok);
        return;
    }
    flag2 = 1;
    if(set2.contains(a))
    {
        set2 -= a;
        QTableWidgetItem *item = new QTableWidgetItem(t->header1.at(a));
        t->setVerticalHeaderItem(a, item);
        if(set2.isEmpty())
        {
            flag2 = 0;
        }
        if(flag1 == 1)
        {
            for(int i = 0;i < set1.count();i ++)
            {
                QList<int> list1 = set1.toList();
                c->graph(list1[i] * 22 + a)->setVisible(false);
                c->graph(list1[i] * 22 + a)->removeFromLegend();
            }
            c->rescaleAxes(true);
            c->replot(QCustomPlot::rpQueuedReplot);
        }
    }
    else
    {
        set2 += a;
        if(set2.count() > 8)
        {
            set2 -= a;
            warning();
            return;
        }
        QFont Font = QFont("SimSun", 12, 87, true);
        QTableWidgetItem *item = new QTableWidgetItem(t->header1.at(a));
        item->setFont(Font);
        t->setVerticalHeaderItem(a, item);
        if(flag1 == 1)
        {
            int horizontal = set1.count();
            int vertical = set2.count();
            int counts = horizontal * vertical;
            if(counts > 8)
            {
                set2 -= a;
                if(set2.count() > 8)
                {
                    warning();
                    return;
                }
                QTableWidgetItem *item = new QTableWidgetItem(t->header1.at(a));
                t->setVerticalHeaderItem(a, item);
                warning();
                return;
            }
            for(int i = 0;i < set1.count();i ++)
            {
                QList<int> list1 = set1.toList();
                QList<int> list2 = set2.toList();
                c->graph(list1[i] * 22 + a)->setVisible(true);
                QColor color(colorname[(i+1) * list2.count()]);
                //color.setHsv(i * 59,255,255);
                c->graph(list1[i] * 22 + a)->setName(t->header1.at(a));
                c->graph(list1[i] * 22 + a)->setPen(color);
                c->graph(list1[i] * 22 + a)->addToLegend();
            }
            c->rescaleAxes(true);
            c->replot(QCustomPlot::rpQueuedReplot);
        }
    }
}

void Widget::drawCurve1(int a)
{
    if(c->graphCount() == 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Warning"),
                                        tr("请先连接！"),
                                     QMessageBox::Ok);
        return;
    }
    flag1 = 1;
    if(set1.contains(a))
    {
        set1 -= a;
        QTableWidgetItem *item = new QTableWidgetItem(t->header2.at(a));
//        QColor color;
//        color.setHsv(a * 60,255,255);
//        item->setTextColor(color);
        t->setHorizontalHeaderItem(a, item);
        if(set1.isEmpty())
        {
            flag1 = 0;
        }
        if(flag2 == 1)
        {
            for(int i = 0;i < set2.count();i ++)
            {
                QList<int> list2 = set2.toList();
                c->graph(a * 22 + list2[i])->setVisible(false);
                c->graph(a * 22 + list2[i])->removeFromLegend();
            }
            c->rescaleAxes(true);
            c->replot(QCustomPlot::rpQueuedReplot);
        }
    }
    else
    {
        set1 += a;
        QFont Font = QFont("SimSun", 12, 87, true);
        QTableWidgetItem *item = new QTableWidgetItem(t->header2.at(a));
        item->setFont(Font);
//        QColor color;
//        color.setHsv(a * 60,255,255);
//        item->setTextColor(color);
        t->setHorizontalHeaderItem(a, item);
        if(flag2 == 1)
        {
            int horizontal = set1.count();
            int vertical = set2.count();
            int counts = horizontal * vertical;
            if(counts > 8)
            {
                set1 -= a;
                if(set1.isEmpty())
                {
                    flag1 = 0;
                }
                QTableWidgetItem *item = new QTableWidgetItem(t->header2.at(a));
//                QColor color;
//                color.setHsv(a * 60,255,255);
//                item->setTextColor(color);
                t->setHorizontalHeaderItem(a, item);
                warning();
                return;
            }
            else
            {
                for(int i = 0;i < set2.count();i ++)
                {
                    QList<int> list2 = set2.toList();
                    QList<int> list1 = set1.toList();
                    c->graph(a * 22 + list2[i])->setVisible(true);
                    QColor color(colorname[(i+1) * list1.count()]);
                    c->graph(a * 22 + list2[i])->setName(t->header1.at(list2[i]));
                    c->graph(a * 22 + list2[i])->setPen(color);
                    c->graph(a * 22 + list2[i])->addToLegend();
                }
                c->rescaleAxes(true);
                c->replot(QCustomPlot::rpQueuedReplot);
            }
        }
    }
}

void Widget::warning()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, tr("显示曲线"),
                                    tr("最多显示8条曲线"),
                                 QMessageBox::Ok);
    if(reply == QMessageBox::Ok)
        return;
}

void Widget::initprivate()
{

    colorname << ""
              << "black"
              << "red"
              << "blue"
              << "yellow"
              << "green"
              << "cyan"
              << "gray"
              << "darkYellow"
              << "darkGray";
}
