#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv.hpp>
#include <QTimer>
#include <QFileDialog>

#include "histogramwindow.h"

using namespace cv;

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
    void on_startCameraButton_clicked();
    void cameraTimerTimeout();
    void on_logoButton_clicked();

private:
    Ui::MainWindow *ui;
    HistogramWindow hist;
    Mat image, logo;
    bool isCameraRunning, logoRead;
    VideoCapture capture;
    QTimer cameraTimer;
};

#endif // MAINWINDOW_H
