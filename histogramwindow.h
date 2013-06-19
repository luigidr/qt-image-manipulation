#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QWidget>
#include <opencv.hpp>

using namespace cv;

namespace Ui {
    class HistogramWindow;
}

class HistogramWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramWindow(QWidget *parent = 0);
    ~HistogramWindow();
    void updateHist(Mat frame);

private:
    MatND getHistogram(const Mat &image);

private:
    Ui::HistogramWindow *ui;
    int histSize[1];
    float hranges[2];
    const float* ranges[1];
    int channel[1];

};

#endif // HISTOGRAMWINDOW_H
