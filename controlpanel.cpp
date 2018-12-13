#include "controlpanel.h"
#include "mainwindow.h"
#include "values.h"
#include <QMessageBox>
#include <QProcess>
#include <QList>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QAbstractSocket>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    server_screen_width  = -1;
    server_screen_height = -1;
    image = new QImage(".\\imgs\\2.jpg");
    frame_width  = 1000;
    frame_height = 600;

}
void ControlPanel::startConnect()
{
    cmdThread = new CmdThread(addr, 5649,this);
    connect(cmdThread, SIGNAL(notOnline()), this, SLOT(notOnlineError()));
   // else cmdThread->setIPandPort(addr,5649);
    connect(cmdThread, SIGNAL(totalTime(int)), this, SLOT(totalTimeSlot(int)));
    connect(cmdThread, SIGNAL(receiveMapSig(char*,int)), this, SLOT(frameChanged(char*,int)));
    connect(cmdThread, SIGNAL(setServerScreenSize(int,int)), this, SLOT(gotServerScreenSize(int,int)));
    cmdThread->run();
}
void ControlPanel::notOnlineError()
{
    QMessageBox::information(this,"error","目标主机连接出错");
}
ControlPanel::~ControlPanel()
{

}
void ControlPanel::endConnect()
{
    cmdThread->noRun();
}

void ControlPanel::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= frame_width || y >= frame_height)
        return;
    double off_x = (double)x / frame_width;
    double off_y = (double)y / frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    int d = e->delta();
    cmdThread->cmdMouseWheel(d, x, y);
}

void ControlPanel::paintEvent(QPaintEvent *e)
{
    if(image != 0)
    {
        QPainter p(this);
        p.drawImage(QRect(0, 0, frame_width, frame_height), *image);
    }
    QWidget::paintEvent(e);
}


QString ControlPanel::getIP()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.count();
    for (int i = 0; i < nListSize; i++)
    {
        strIpAddress = ipAddressesList.at(i).toString();
        if(strIpAddress.contains("192.168.2.")){
            break;
        }
//           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
//               ipAddressesList.at(i).toIPv4Address()) {
//               strIpAddress = ipAddressesList.at(i).toString();
//           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
     {
         strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     }
     return strIpAddress;
}

void lookUpRecord(int index)
{
    QProcess p(0);
    QString arg = "node web3Script.js -g ";
    arg = arg + QString::number(index);
    p.setWorkingDirectory("E:\\ethereum\\web3");
    p.start("cmd", QStringList()<<"/c"<< arg);
    p.waitForStarted();
    p.waitForFinished();
    QString strTemp=QString::fromLocal8Bit(p.readAllStandardOutput());
    qDebug() << strTemp;
}


void newRecord(QString clientIP, QString serverIP, int time, int money)
{
    QProcess p(0);
    QString arg = "node web3Script.js -n ";
    arg = arg + clientIP + " " + serverIP + " "
            + QString::number(time) +  " " + QString::number(money) ;
    p.setWorkingDirectory("E:\\ethereum\\web3");
    p.start("cmd", QStringList()<<"/c"<< arg);
    p.waitForStarted();
    p.waitForFinished();
    QString strTemp=QString::fromLocal8Bit(p.readAllStandardOutput());
    qDebug() << strTemp;
}

void sendTransaction(QString address, int money)
{
    QProcess p(0);
    QString arg = "node web3Script.js -t ";
    arg = arg + address + " " + QString::number(money) ;
    p.setWorkingDirectory("E:\\ethereum\\web3");
    p.start("cmd", QStringList()<<"/c"<< arg);
    p.waitForStarted();
    p.waitForFinished();
    QString strTemp=QString::fromLocal8Bit(p.readAllStandardOutput());
    qDebug() << strTemp;
}


void ControlPanel::totalTimeSlot(int total)
{
    int money = total / 10;
    int hour,minute,sec;
    sec = total%60;
    total /= 60;
    minute = total%60;
    total /= 60;
    hour = total;
    QMessageBox::information(this,"温馨提示","您使用了"
                             +QString::number(hour)+"时"
                             +QString::number(minute)+"分"
                             +QString::number(sec)+"秒,共计花费了"
                             +QString::number(money)+" ETH。\n"
                                                     "交易已发送，您可以在日志文件中查看详细情况");
    qDebug() << "storedServerAcconut" << serverAcconut;

    sendTransaction(serverAcconut, money);
    newRecord(clientIP, serverIP, money, money);
}
uchar ControlPanel::translateKey(int key)
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

void ControlPanel::keyPressEvent(QKeyEvent *e)
{
    if(!control)
        return;

    int k = translateKey(e->key());
    if(k == 0)
        return;
    qDebug()<<"press first";
    cmdThread->cmdKeyPress(k);
    e->ignore();
}

void ControlPanel::keyReleaseEvent(QKeyEvent *e)
{
    if(!control)
        return;

    int k = translateKey(e->key());
    if(k == 0)
        return;
    cmdThread->cmdKeyRelease(k);
    e->ignore();
}

void ControlPanel::keyPressSlot(QKeyEvent *e)
{
    keyPressEvent(e);
}

void ControlPanel::keyReleaseSlot(QKeyEvent *e)
{
    keyReleaseEvent(e);
}
long long last=0;
void ControlPanel::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    long long current = QDateTime::currentDateTime().toMSecsSinceEpoch();
    bool more = (current-last)>40;
   // qDebug()<<more<<" "<<last<<" "<<current;
    last = current;
    if(!control ||!more)
        return;

    if(server_screen_width < 0 || frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= frame_width || y >= frame_height)
        return;
    double off_x = (double)x / frame_width;
    double off_y = (double)y / frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    cmdThread->cmdMouseMoveTo(x, y);
}
void ControlPanel::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= frame_width || y >= frame_height)
        return;
    double off_x = (double)x / frame_width;
    double off_y = (double)y / frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    cmdThread->cmdMouseDoubleClick(x, y);
}
void ControlPanel::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
//    qDebug()<<"mousePress"<<x;
    if(x >= frame_width || y >= frame_height)
        return;
    double off_x = (double)x / frame_width;
    double off_y = (double)y / frame_height;
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
void ControlPanel::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= frame_width || y >= frame_height)
        return;
    double off_x = (double)x / frame_width;
    double off_y = (double)y / frame_height;
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
void ControlPanel::frameChanged(char *buf, int len)
{
    image->loadFromData((uchar*)buf, len, "JPG");;
    update();
}

void ControlPanel::changeFrameSize(int w, int h)
{
    frame_width  = w;
    frame_height = h;
}

void ControlPanel::gotServerScreenSize(int w, int h)
{
    server_screen_width  = w;
    server_screen_height = h;
}
