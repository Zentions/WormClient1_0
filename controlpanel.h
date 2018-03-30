#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QMainWindow>
#include <QRect>
#include <QTcpSocket>
#include <QHostAddress>
#include <QImage>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

#include "mapthread.h"
#include "cmdthread.h"

class ControlPanel : public QWidget
{
    Q_OBJECT

private:
    QImage* image;
    MapThread* mapThread;
    CmdThread* cmdThread;
    int frame_width;
    int frame_height;
    int server_screen_width;
    int server_screen_height;

    uchar translateKey(int key);

public:
    ControlPanel(QWidget *parent = 0);
    ~ControlPanel();
    void startConnect();
signals:
    void mouseMoveTo(int, int);

public slots:
    void frameChanged(QImage*);
    void changeFrameSize(int, int);
    void gotServerScreenSize(int, int);

    void keyPressSlot(QKeyEvent *);
    void keyReleaseSlot(QKeyEvent *);
//    void mouseMoveSlot(QMouseEvent *);
//    void mouseDoubleClickSlot(QMouseEvent *);
//    void mousePressSlot(QMouseEvent *);
//    void mouseReleaseSlot(QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // CONTROLPANEL_H
