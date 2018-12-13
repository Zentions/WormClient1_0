#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controlPanel = new ControlPanel(ui->centralWidget);
    connect(this, SIGNAL(keyPressed(QKeyEvent*)), controlPanel, SLOT(keyPressSlot(QKeyEvent*)));
    connect(this, SIGNAL(keyReleased(QKeyEvent*)), controlPanel, SLOT(keyReleaseSlot(QKeyEvent*)));
    qDebug()<< "clientIP:" + controlPanel->getIP();
    clientIP = controlPanel->getIP();
    setObjectName(QStringLiteral("controlPanel"));
    ui->disConButton->setEnabled(false);
    controlPanel->setGeometry(QRect(260, 40, 1000, 600));
    scan = NULL;
    ui->lineEdit->setEnabled(false);
    parentTree = new QTreeWidgetItem(ui->treeWidget);
    parentTree->setText(0,"IP address");    //树形控件显示的文本信息
    connect(ui->treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tryconnect()
{
    addr = ui->lineEdit->text();
    serverIP = addr;
    rq_width = 1000;
    rq_height = 600;
    //controlPanel = new ControlPanel(QRect(0, 0, screen_width / 3 * 2, screen_height / 3 * 2), this);
    controlPanel->startConnect();
    //this->setCentralWidget(controlPanel);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    emit keyPressed(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    emit keyReleased(e);
}

void MainWindow::on_conButton_clicked()
{
    if(control)
    {
        qDebug()<<"controling";
        return;
    }
    ui->conButton->setEnabled(false);
    ui->disConButton->setEnabled(true);

    tryconnect();

}

void MainWindow::on_disConButton_clicked()
{
    controlPanel->endConnect();
    ui->conButton->setEnabled(true);
    ui->disConButton->setEnabled(false);
}
void MainWindow::receiveScanRec(QVector<QString>* ipVec)
{
    for(int i=0;i<ipVec->size();i++)
    {
        //parentTree->
        QTreeWidgetItem* subItem = new QTreeWidgetItem(parentTree);
        subItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        subItem->setText(0,ipVec->at(i));
        subItem->setCheckState(0,Qt::Unchecked);
    }

}
void MainWindow::on_change_clicked()
{
    int count = parentTree->childCount();
    for(int i=0;i<count;i++)
    {
        parentTree->removeChild(parentTree->child(0));
    }
    scan = new Scanner;
    connect(scan, SIGNAL(finishScan(QVector<QString>*)), this, SLOT(receiveScanRec(QVector<QString>*)));
    scan->start();
}
void MainWindow::treeItemChanged(QTreeWidgetItem* item,int value)
{

    if(Qt::Checked == item->checkState(0))
    {
       clearChildState(item,value);
       QString ip = item->text(0);
       ui->lineEdit->setText(ip);
     }
    else if(Qt::Unchecked == item->checkState(0))
    {
        int count = item->childCount();
        if(count > 0){}
        else
        {
            ui->lineEdit->clear();
        }
    }
}
void MainWindow::clearChildState(QTreeWidgetItem* item,int value)
{
    QTreeWidgetItem *parent = item->parent();
    int count = parent->childCount();
    for(int i=0;i<count;i++)
    {
        if(parent->child(i)!=item)
        parent->child(i)->setCheckState(0,Qt::Unchecked);
    }
}
