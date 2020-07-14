#include "CommandBar.h"
#include "QCustomPlot.h"

#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QStringList>
#include <QTextEdit>
#include <QToolBar>

CommandBar::CommandBar(QMainWindow* parent)
    : QMainWindow(parent)
{
    //设置标题和初始窗口大小；
    setWindowTitle(tr("数据"));
    setMinimumSize(1500, 800);

    //设置中心widget为QWidget子类
    QWidget* CWidget;
    CWidget = new QWidget(this);
    setCentralWidget(CWidget);

    //创建表格，初始化，并放在CWidget下；
    QTableWidget* Table;
    Table = new QTableWidget(CWidget);
    Table->setMaximumWidth(1000);

    //创建画图工具，初始化，放在CWidget下；
    QCustomPlot* Curve = new QCustomPlot(CWidget);
    Curve->setMinimumWidth(800);

    //设置CWidget垂直布局
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(Table);
    layout->addWidget(Curve);
    CWidget->setLayout(layout);

    //Table->setGeometry(200, 100, 400, 600);

    //设置表格表头，行列数目，
    QStringList headerNameList;
    headerNameList.append("Name");
    headerNameList.append("Value");
    headerNameList.append("Unit");
    Table->setColumnCount(3);
    Table->setRowCount(5000);
    Table->setHorizontalHeaderLabels(headerNameList);
    Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //设置画图工具

    //Curve->setGeometry(700, 100, 400, 600);

    QAction* openAction = new QAction(QIcon("D:/grf/openfile"), tr("&open..."), this);
    //使用qt默认快捷键，可在不同系统中使用
    openAction->setShortcuts(QKeySequence::Open);
    //设置状态栏信息
    openAction->setStatusTip(tr("Open an existing file"));
    //信号槽连接
    connect(openAction, &QAction::triggered, this, &CommandBar::openfile);

    //menuBar如无菜单栏则创建菜单栏并返回，addmenu在菜单栏添加按钮file
    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);

    //没有类似菜单栏menubar，是因为工具栏可以设置多个
    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);
    //类似菜单栏menubar，创建状态栏；
    statusBar();
}

CommandBar::~CommandBar()
{
}

void CommandBar::openfile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("选择日志文件"),"",tr("TXT(*.txt)"));
}

