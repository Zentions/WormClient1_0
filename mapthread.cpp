#include "mapthread.h"

MapThread::MapThread(QObject *parent): QThread(parent)
{
    m_buf = new char[1024*1024];
    memset(m_buf, 0, 1024);
}
MapThread::~MapThread()
{
    m_udpSocket->close();
    delete m_buf;
}
void MapThread::run()
{
    m_udpSocket = new QUdpSocket;
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(ReadData()),Qt::DirectConnection);
    qDebug()<< m_udpSocket->bind(QHostAddress::Any, 5648);
    exec();
}

void MapThread::ReadData()
{
    char *recvBuf = new char[1052];
    memset(recvBuf, 0, 1052);
    //qDebug("start");
    while(m_udpSocket->hasPendingDatagrams()) {
        memset(recvBuf, 0, 1052);
        int size = m_udpSocket->pendingDatagramSize();

        //qDebug()<<"size"<<size;
        m_udpSocket->readDatagram(recvBuf, size);
        ImageFrameHead *mes = (ImageFrameHead *)recvBuf;
        //qDebug()<<mes->uDataFrameCurr<<mes->uDataFrameTotal;
        if (mes->funCode == 24) {
            memcpy(m_buf+mes->uDataInFrameOffset, (recvBuf+ sizeof(ImageFrameHead)), mes->uTransFrameSize);
            if (mes->uDataFrameCurr == mes->uDataFrameTotal) {
                qDebug("write");
                if(control)emit sigRecvOk(m_buf, mes->uDataFrameSize);
            }
        }

    }
    delete recvBuf;

}
