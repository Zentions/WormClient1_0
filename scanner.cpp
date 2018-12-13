#include "scanner.h"
QMutex text;
Scanner::Scanner(QObject *parent): QThread(parent)
{
    needToProcess = new QVector<QString>;
    QString localHostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(localHostName);
    QList<QHostAddress> listAddress = hostInfo.addresses();
    QHostAddress tempAdd;
    foreach (tempAdd, listAddress) {
        if(tempAdd.protocol()==QAbstractSocket::IPv4Protocol &&
                (tempAdd.toString().contains("192.168.")||tempAdd.toString().contains("172.")||tempAdd.toString().contains("10.")))
        {
            needToProcess->append(tempAdd.toString());
//            qDebug()<<"d"<<tempAdd.toString();

        }
    }

}
void Scanner::run()
{
   startScan();
}
Scanner::~Scanner()
{

}
void Scanner::startScan()
{
    ipVec = new QVector<QString>;
    socket = new QTcpSocket;
    for(int j=0;j<needToProcess->size();j++)
    {

         QStringList list = needToProcess->at(j).split(".");
         ip[0] = list[0].toInt();
         ip[1] = list[1].toInt();
         ip[2] = list[2].toInt();
         ip[3] = 0;
         for(int i=1;i<255;i++)
         {

             socket->abort();
             QString tmpIp = QString::number(ip[0])+"."+QString::number(ip[1])+"."+QString::number(ip[2])+"."+QString::number(i);
             socket->connectToHost(tmpIp,5649);
             if(socket->waitForConnected(5))
             {
                 ipVec->append(tmpIp);
                 socket->close();
             }
         }


    }
     emit finishScan(ipVec);

}
