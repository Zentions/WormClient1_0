#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QImage>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMainWindow>
#include "mapthread.h"
#include "cmdthread.h"
#include "values.h"
#include "controlpanel.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    ControlPanel* controlPanel;
    Ui::MainWindow *ui;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void keyPressed(QKeyEvent *);
    void keyReleased(QKeyEvent *);

//    void mouseMove(QMouseEvent *);
//    void mouseDoubleClick(QMouseEvent *);
//    void mousePress(QMouseEvent *);
//    void mouseRelease(QMouseEvent *);
public slots:
    void tryconnect();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
//    void mouseMoveEvent(QMouseEvent *);
//    void mouseDoubleClickEvent(QMouseEvent *);
//    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
private slots:
    void on_conButton_clicked();
    void on_disConButton_clicked();
};

#endif // MAINWINDOW_H

