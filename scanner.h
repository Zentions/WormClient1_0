#ifndef SCANNER_H
#define SCANNER_H
#include <QVector>
#include <QThread>
class Scanner:public QThread
{
private:
    QVector<QString>* ipVec;

public:
    Scanner();
};

#endif // SCANNER_H
