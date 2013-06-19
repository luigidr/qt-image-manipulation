#include "histogramwindow.h"
#include "ui_histogramwindow.h"

HistogramWindow::HistogramWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogramWindow)
{
    ui->setupUi(this);
    // number of histogram lines
    histSize[0] = 256;
    // BGR values range
    hranges[0] = 0.0;
    hranges[1] = 255.0;
    ranges[0] = hranges;
    channel[0] = 0;
}

void HistogramWindow::updateHist(Mat src) {
    /*
        1. split in the 3 RGB channels
        2. for each channel, call getHistogram()
        3. show the result in the defined window
    */

    vector<Mat> rgb_planes;
    split(src, rgb_planes);

    MatND hist_red = getHistogram(rgb_planes[2]);
    MatND hist_green = getHistogram(rgb_planes[1]);
    MatND hist_blu = getHistogram(rgb_planes[0]);

    // max and min value of bins
    double maxVal=0;
    double minVal=0;
    minMaxLoc(hist_red, &minVal, &maxVal, 0, 0);

    // image for drawing the histogram
    int img_h = ui->redH->size().height();
    int img_w = histSize[0];
    Mat histImg_red(img_h,img_w,CV_8UC3,Scalar(50,50,50));
    Mat histImg_green(img_h,img_w,CV_8UC3,Scalar(50,50,50));
    Mat histImg_blu(img_h,img_w,CV_8UC3,Scalar(50,50,50));

    // max height (90% of image height)
    int hpt = static_cast<int>(0.9*img_h);

    // draw a single line for each bin
    for (int h=0; h<img_w; h++) {
        // normalized height (with respect to the histogram image)
        int intensity_red = static_cast<int>(hist_red.at<float>(h)*hpt/maxVal);
        int intensity_green = static_cast<int>(hist_green.at<float>(h)*hpt/maxVal);
        int intensity_blu = static_cast<int>(hist_blu.at<float>(h)*hpt/maxVal);

        // effectively draw the line
        line(histImg_red,Point(h,img_h),Point(h,img_h-intensity_red),Scalar(255,0,0));
        line(histImg_green,Point(h,img_h),Point(h,img_h-intensity_green),Scalar(0,255,0));
        line(histImg_blu,Point(h,img_h),Point(h,img_h-intensity_blu),Scalar(0,0,255));
    }

    // show
    ui->redH->setPixmap(QPixmap::fromImage(QImage(
                                               (uchar*)histImg_red.data,
                                               histImg_red.cols,
                                               histImg_red.rows,
                                               QImage::Format_RGB888)));
    ui->greenH->setPixmap(QPixmap::fromImage(QImage(
                                               (uchar*)histImg_green.data,
                                               histImg_green.cols,
                                               histImg_green.rows,
                                               QImage::Format_RGB888)));
    ui->bluH->setPixmap(QPixmap::fromImage(QImage(
                                               (uchar*)histImg_blu.data,
                                               histImg_blu.cols,
                                               histImg_blu.rows,
                                               QImage::Format_RGB888)));
}


// Compute the histogram for each RGB channel
MatND HistogramWindow::getHistogram(const Mat &image)
{
    MatND hist;

    calcHist(&image,
             1, // only one image
             channel, // used channels
             Mat(), // no mask
             hist, // final histogram
             1, // one histogram in 3 dimensions
             histSize, // number of the values present in the histogram
             ranges // range present in the channel
    );

    return hist;

}

HistogramWindow::~HistogramWindow()
{
    delete ui;
}
