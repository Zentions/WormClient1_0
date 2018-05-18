#ifndef SCANNER_H
#define SCANNER_H


#include <QVector>
#include <QThread>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QHostInfo>
#include <QTcpSocket>
#include <QMutex>
class Scanner: public QThread
{
    Q_OBJECT

public:
   explicit Scanner(QObject *parent = 0);
   ~Scanner();
   void startScan();
protected:
    void run();
signals:
    void finishScan(QVector<QString>*);
private:
    QVector<QString>* needToProcess;
    QVector<QString>* ipVec;
    int ip[4];
    QTcpSocket *socket;
};

#endif // SCANNER_H
