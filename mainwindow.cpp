#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    frame_width = 1001;
    frame_height = 601;
    setMouseTracking(true);
    server_screen_width  = -1;
    server_screen_height = -1;
    image = 0;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    tryconnect();
}

void MainWindow::on_pushButton_2_clicked()
{

}
void MainWindow::tryconnect()
{
    addr = ui->lineEdit->text();
    mapThread = new MapThread(addr, 5648, rect.width(), rect.height());
    connect(mapThread, SIGNAL(frameGot(QImage*)), this, SLOT(frameChanged(QImage*)));
    mapThread->start();
    cmdThread = new CmdThread(addr, 5649);
    connect(cmdThread, SIGNAL(setServerScreenSize(int,int)), this, SLOT(gotServerScreenSize(int,int)));
    cmdThread->run();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
        return;
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    cmdThread->cmdMouseMoveTo(x, y);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
        return;
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    if(e->button() == Qt::LeftButton)
    {
        cmdThread->cmdMouseLeftDown(x, y);
    }
    else if(e->button() == Qt::RightButton)
    {
        cmdThread->cmdMouseRightDown(x, y);
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
        return;
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    if(e->button() == Qt::LeftButton)
    {
        cmdThread->cmdMouseLeftUp(x, y);
    }
    else if(e->button() == Qt::RightButton)
    {
        cmdThread->cmdMouseRightUp(x, y);
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
        return;
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    cmdThread->cmdMouseDoubleClick(x, y);
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
        return;
    int x = e->globalX();
    int y = e->globalY();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
        return;
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    int d = e->delta();
    cmdThread->cmdMouseWheel(d, x, y);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    if(image != 0)
    {
        QPainter p(this);
        p.drawImage(QRect(0, 0, frame_width, frame_height), *image);
    }
    QWidget::paintEvent(e);
}

uchar MainWindow::translateKey(int key)
{
    int k = key;
    bool legal = true;
    if(k >= Qt::Key_0 && k <= Qt::Key_9)
    {
    }
    else if(k >= Qt::Key_A && k <= Qt::Key_Z)
    {

    }
    else if(k >= Qt::Key_F1 && k <= Qt::Key_F24)
    {
        k &= 0x000000ff;
        k += 0x40;
    }
    else if(k == Qt::Key_Tab)
    {
        k = 0x09;
    }
    else if(k == Qt::Key_Backspace)
    {
        k = 0x08;
    }
    else if(k == Qt::Key_Return)
        k = 0x0d;
    else if(k <= Qt::Key_Down && k >= Qt::Key_Left)
    {
        int off = k - Qt::Key_Left;
        k = 0x25 + off;
    }
    else if(k == Qt::Key_Shift)
        k = 0x10;
    else if(k == Qt::Key_Control)
        k = 0x11;
    else if(k == Qt::Key_Alt)
        k = 0x12;
    else if(k == Qt::Key_Meta)
        k = 0x5b;
    else if(k == Qt::Key_Insert)
        k = 0x2d;
    else if(k == Qt::Key_Delete)
        k = 0x2e;
    else if(k == Qt::Key_Home)
        k = 0x24;
    else if(k == Qt::Key_End)
        k = 0x23;
    else if(k == Qt::Key_PageUp)
        k = 0x21;
    else if(k == Qt::Key_PageDown)
        k = 0x22;
    else if(k == Qt::Key_CapsLock)
        k = 0x14;
    else if(k == Qt::Key_NumLock)
        k = 0x90;
    else if(k == Qt::Key_Space)
        k = 0x20;
    else
        legal = false;
    if(legal == false)
        return 0;
    return k;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(!control)
        return;

    int k = translateKey(e->key());
    if(k == 0)
        return;
    cmdThread->cmdKeyPress(k);
    e->ignore();
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(!control)
        return;

    int k = translateKey(e->key());
    if(k == 0)
        return;
    cmdThread->cmdKeyRelease(k);
    e->ignore();
}
void MainWindow::frameChanged(QImage* i)
{
    image = i;
    update();
}


void MainWindow::gotServerScreenSize(int w, int h)
{
    server_screen_width  = w;
    server_screen_height = h;
}
