#ifndef CMDTHREAD_H
#define CMDTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QMetaType>

#include "consts.h"
#include "algo.h"
#include "values.h"
class CmdThread : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* cmdSocket;
    QString     address;
    int         port;

    uchar cmd_buf[8];
    int   cmd_buf_fill;
    int errorNum;
public:
    CmdThread(QString add, int p, QObject* parent = 0);
    void connectToServer();
    void reconnectToServer();
    void setIPandPort(QString add, int p);

signals:
    void setServerScreenSize(int, int);
    void notOnline();
public slots:
    void connectError(QAbstractSocket::SocketError);
    void connectSucceed();
    void newData();
    void newCommand();
    void cmdMouseMoveTo(int x, int y);
    void cmdMouseLeftDown(int x, int y);
    void cmdMouseLeftUp(int x, int y);
    void cmdMouseRightDown(int x, int y);
    void cmdMouseRightUp(int x, int y);
    void cmdMouseWheel(int delta, int x, int y);
    void cmdScreenSize();
    void cmdMouseDoubleClick(int x, int y);

    void cmdKeyPress(uchar key);
    void cmdKeyRelease(uchar key);

    void noRun();
    void run();

};


#endif // CMDTHREAD_H
