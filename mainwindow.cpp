#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controlPanel = new ControlPanel(ui->centralWidget);
    setObjectName(QStringLiteral("controlPanel"));
    controlPanel->setGeometry(QRect(260, 40, 1000, 600));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tryconnect()
{
    addr = ui->lineEdit->text();
    rq_width = 1000;
    rq_height = 600;


    //controlPanel = new ControlPanel(QRect(0, 0, screen_width / 3 * 2, screen_height / 3 * 2), this);
    controlPanel->startConnect();
    //this->setCentralWidget(controlPanel);
    connect(this, SIGNAL(keyPressed(QKeyEvent*)), controlPanel, SLOT(keyPressSlot(QKeyEvent*)));
    connect(this, SIGNAL(keyReleased(QKeyEvent*)), controlPanel, SLOT(keyReleaseSlot(QKeyEvent*)));
//    connect(this, SIGNAL(mouseMove(QMouseEvent*)), controlPanel, SLOT(mouseMoveSlot(QMouseEvent*)));
//    connect(this, SIGNAL(mouseDoubleClick(QMouseEvent*)), controlPanel, SLOT(mouseDoubleClickSlot(QMouseEvent*)));
//    connect(this, SIGNAL(mousePress(QMouseEvent*)), controlPanel, SLOT(mousePressSlot(QMouseEvent*)));
//    connect(this, SIGNAL(mouseRelease(QMouseEvent*)), controlPanel, SLOT(mouseReleaseSlot(QMouseEvent*)));
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    emit keyPressed(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    emit keyReleased(e);
}
//void MainWindow::mouseMoveEvent(QMouseEvent *e)
//{

//    emit mouseMove(e);
//}
//void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
//{
//    emit mouseDoubleClick(e);
//}
//void MainWindow::mousePressEvent(QMouseEvent *e)
//{
//    qDebug()<<"mousePress";
//    emit mousePress(e);
//}
//void MainWindow::mouseReleaseEvent(QMouseEvent *e)
//{
//    emit mouseRelease(e);
//}


void MainWindow::on_pushButton_clicked()
{
    tryconnect();
}
