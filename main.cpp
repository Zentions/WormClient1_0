#include "mainwindow.h"
#include <QApplication>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QProcess process(0);
    //process.startDetached("GethJavaScriptConsole.bat");
    MainWindow w;
    w.show();
    return a.exec();
}
