#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "values.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void frameChanged(QImage*);
    void gotServerScreenSize(int, int);

public slots:
    void tryconnect();

signals:
    void mouseMoveTo(int, int);

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
private:
    Ui::MainWindow *ui;
    QImage* image;
    MapThread* mapThread;
    CmdThread* cmdThread;
    int server_screen_width;
    int server_screen_height;
    int frame_width;
    int frame_height;

    uchar translateKey(int key);
};

#endif // MAINWINDOW_H

