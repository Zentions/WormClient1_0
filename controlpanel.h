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
#include <QDateTime>
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
    void endConnect();
    QString getIP();
signals:
    void mouseMoveTo(int, int);

public slots:
    void frameChanged(char *buf, int len);
    void changeFrameSize(int, int);
    void gotServerScreenSize(int, int);
    void notOnlineError();
    void keyPressSlot(QKeyEvent *);
    void keyReleaseSlot(QKeyEvent *);
    void totalTimeSlot(int);
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
