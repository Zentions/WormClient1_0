#include "cmdthread.h"
#include <QDateTime>
CmdThread::CmdThread(QString add, int p, QObject* parent) : QObject(parent)
{
    serverIP = add;
    address = add;
    port = p;
    control=false;
    cmd_buf_fill = 0;
    errorNum = 0;
}
void CmdThread::setIPandPort(QString add, int p)
{
    this->address=add;
    this->port=p;
    control=false;
    cmd_buf_fill = 0;
}
void CmdThread::run()
{
    qRegisterMetaType<QAbstractSocket::SocketError>("SocketError");
    cmdSocket = new QTcpSocket;
    connect(cmdSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError(QAbstractSocket::SocketError)));
    connect(cmdSocket, SIGNAL(connected()), this, SLOT(connectSucceed()));
    connect(cmdSocket, SIGNAL(readyRead()), this, SLOT(newData()));

    connectToServer();
    cmdSocket->waitForConnected();
    if(control) cmdScreenSize();
}

void CmdThread::newData()
{
    while(true)
    {
        int r = cmdSocket->read((char*)(cmd_buf + cmd_buf_fill), 48 - cmd_buf_fill);
        if(r <= 0)
            return;
        cmd_buf_fill += r;
        if(cmd_buf_fill == 48)
        {
            newCommand();
            cmd_buf_fill = 0;
        }
    }
}

void CmdThread::newCommand()
{

    int c = cmd_buf[0];
    if(c == CMD_GET_SCREEN_SIZE_RES)
    {
        int w = cmd_buf[1];
        w = w << 8;
        w += cmd_buf[2];
        int h = cmd_buf[3];
        h = h << 8;
        h += cmd_buf[4];
        emit setServerScreenSize(w, h);
        qDebug()<<"get server screen size:"<<w<<" "<<h;
        for(int i=5;i<47;i++){
            QChar temp = (QChar)cmd_buf[i];
            serverAcconut += temp;
        }
    }
}

void CmdThread::connectToServer()
{
    qDebug()<<"try to connect";
    cmdSocket->connectToHost(address, port);
}

void CmdThread::connectError(QAbstractSocket::SocketError)
{
    qDebug()<<"connect error, reconnect";
    //QThread::msleep(200);
    cmdSocket->abort();
    cmdSocket->close();
    control = false;
    errorNum++;
    if(errorNum<2) connectToServer();
    else emit notOnline();
}
void CmdThread::noRun()
{
    cmdSocket->abort();
    cmdSocket->close();
    control = false;
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    end = time.toTime_t();
    emit totalTime(end-start);
}
void CmdThread::connectSucceed()
{
    qDebug()<<"connect succeed";
    control = true;
    //建立图形通道
    mapThread = new MapThread();
    connect(mapThread, SIGNAL(sigRecvOk(char *, int)), this, SLOT(getMapSignal(char *, int)));
    mapThread->start();
    errorNum = 0;
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    start = time.toTime_t();

}
void CmdThread::getMapSignal(char *chars, int len)
{
    emit receiveMapSig(chars,len);
}
void CmdThread::cmdMouseMoveTo(int x, int y)
{
    if(control == false)
        return;
    uchar uc[8];
    uc[0] = CMD_MOUSE_MOVE_TO;
    uc[1] = x / 0x100;
    uc[2] = x % 0x100;
    uc[3] = y / 0x100;
    uc[4] = y % 0x100;
    //qDebug()<<"mouse move to"<<x<<" "<<y;
    writeAndBlock(cmdSocket, uc, 8);
}

void CmdThread::cmdMouseDoubleClick(int x, int y)
{
    uchar uc[8];
    uc[0] = CMD_MOUSE_DOUBLE_CLICK;
    uc[1] = x / 0x100;
    uc[2] = x % 0x100;
    uc[3] = y / 0x100;
    uc[4] = y % 0x100;
    writeAndBlock(cmdSocket, uc, 8);
}

void CmdThread::cmdMouseLeftDown(int x, int y)
{
    uchar uc[8];
    uc[0] = CMD_MOUSE_LEFT_DOWN;
    uc[1] = x / 0x100;
    uc[2] = x % 0x100;
    uc[3] = y / 0x100;
    uc[4] = y % 0x100;
    writeAndBlock(cmdSocket, uc, 8);
}

void CmdThread::cmdMouseLeftUp(int x, int y)
{
    uchar uc[8];
    uc[0] = CMD_MOUSE_LEFT_UP;
    uc[1] = x / 0x100;
    uc[2] = x % 0x100;
    uc[3] = y / 0x100;
    uc[4] = y % 0x100;
    writeAndBlock(cmdSocket, uc, 8);
}

void CmdThread::cmdMouseRightDown(int x, int y)
{
    uchar uc[8];
    uc[0] = CMD_MOUSE_RIGHT_DOWN;
    uc[1] = x / 0x100;
    uc[2] = x % 0x100;
    uc[3] = y / 0x100;
    uc[4] = y % 0x100;
    writeAndBlock(cmdSocket, uc, 8);
}

void CmdThread::cmdMouseRightUp(int x, int y)
{
    uchar uc[8];
    uc[0] = CMD_MOUSE_RIGHT_UP;
    uc[1] = x / 0x100;
    uc[2] = x % 0x100;
    uc[3] = y / 0x100;
    uc[4] = y % 0x100;
    writeAndBlock(cmdSocket, uc, 8);
}

void CmdThread::cmdMouseWheel(int delta, int x, int y)
{
    uchar uc[8];
    uc[0] = CMD_MOUSE_WHEEL;
    uc[1] = delta / 0x100;
    uc[2] = delta % 0x100;
    uc[3] = x / 0x100;
    uc[4] = x % 0x100;
    uc[5] = y / 0x100;
    uc[6] = y % 0x100;
    writeAndBlock(cmdSocket, uc, 8);
}

void CmdThread::cmdScreenSize()
{
    uchar uc[8];
    uc[0] = CMD_GET_SCREEN_SIZE;
    writeAndBlock(cmdSocket, uc, 8);
}

void CmdThread::cmdKeyPress(uchar key)
{
    uchar uc[8];
    uc[0] = CMD_KEY_PRESS;
    uc[1] = key;
    writeAndBlock(cmdSocket, uc, 8);
}

void CmdThread::cmdKeyRelease(uchar key)
{
    uchar uc[8];
    uc[0] = CMD_KEY_RELEASE;
    uc[1] = key;
    writeAndBlock(cmdSocket, uc, 8);
}
