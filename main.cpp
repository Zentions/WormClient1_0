#include "mainwindow.h"
#include <QApplication>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
