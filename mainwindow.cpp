#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // init
    ui->setupUi(this);
    isCameraRunning = false;
    logoRead = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startCameraButton_clicked()
{
    // the webcam is not yet started
    if(!isCameraRunning)
    {
        // open camera stream
        capture.open(CV_CAP_ANY); // default: 0

        if(!capture.isOpened())
            return;

        isCameraRunning = true;

        // start timer for acquiring the video
        cameraTimer.start(33); // 33 ms = 30 fps
        // at the timeout() event, execute the cameraTimerTimeout() method
        connect(&cameraTimer, SIGNAL(timeout()), this, SLOT(cameraTimerTimeout()));

        // update the user interface...
        ui->startCameraButton->setText("Stop webcam");
        ui->logoButton->setEnabled(true);
        ui->histCheckbox->setEnabled(true);
    }
    else
    {
        if(!capture.isOpened())
            return;

        // stop timer
        cameraTimer.stop();
        // release camera stream
        capture.release();

        isCameraRunning = false;

        // restore the user interface to the original status...
        ui->startCameraButton->setText("Start webcam");
        ui->logoButton->setEnabled(false);
        ui->histCheckbox->setEnabled(false);
    }
}

void MainWindow::cameraTimerTimeout()
{
    if(isCameraRunning && capture.isOpened())
    {
        // store the frame to show in a Qt window
        QImage frameToShow;

        // get the current frame from the video stream
        capture >> image;

        /** SECTION: Adding a logo... **/
        if(logoRead)
        {
            // define a ROI (in the lower right corner of the frame)
            Mat imageROI;
            imageROI = image(Rect(image.cols-logo.cols,image.rows-logo.rows,logo.cols,logo.rows));

            // first method: add weighted
            //addWeighted(imageROI,1.0,logo,0.7,0.0,imageROI);

            // second method: copying images
            Mat mask;
            // copy the logo for having a mask, in grayscale
            mask = logo.clone();
            cvtColor(mask, mask, CV_BGR2GRAY);
            logo.copyTo(imageROI,mask);
        }
        /** END SECTION: Adding a logo... **/

        /** SECTION: Histogram(s) **/
        if (ui->histCheckbox->checkState() == Qt::Checked)
        {
            hist.updateHist(image);
            hist.show();
        } else
            hist.hide();
        /** END SECTION: Histogram(s) **/

        // prepare the image for the Qt format...
        // ... change color channel ordering (from BGR in our Mat to RGB in QImage)
        cvtColor(image, image, CV_BGR2RGB);
        //cvtColor(image, image, CV_BGR2GRAY); // in gray

        // define a color table for the Indexed8 Qt format
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++)
            colorTable.push_back(qRgb(i, i, i));

        // Qt image
        // image.step is needed to properly show some images (due to padding byte added in the Mat creation)
        //frameToShow = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);

        /** SECTION: Grayscale Image **/
        frameToShow = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.step, QImage::Format_Indexed8); // grayscale

        // apply color table for showing the frame in grayscale
        frameToShow.setColorTable(colorTable);
        /** END SECTION: Grayscale Image **/

        // display on label
        ui->label->setPixmap(QPixmap::fromImage(frameToShow));

        // resize the label to fit the image
        ui->label->resize(ui->label->pixmap()->size());
    }

}

void MainWindow::on_logoButton_clicked()
{
    if(!logoRead)
    {
        // choose the image to open
        QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

        // read the image
        logo = imread(filename.toAscii().data());

        // the logo has been correctly read
        if(logo.data)
        {
            logoRead = true;

            // update the user interface...
            ui->logoButton->setText("Remove logo");
        }
    }
    else
    {
        logoRead = false;

        // restore the user interface to the original status...
        ui->logoButton->setText("Add logo");
    }
}
