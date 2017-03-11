#include "mainwindow.h"
#include "ui_mainwindow.h"

// for mouse press
#include "childqlabel.h"

// global variables used by childqlabel.cpp as well
#include "globalvariables.h"
int IMAGE_COLS_ = -1;
int IMAGE_ROWS_ = -1;

#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QMenu>
#include <QPixmap>
#include <QWidget>

#include <cassert>
#include "seamcarver.h"
#include "childqlabel.h"

double vCompressPercent = 0.25;
double hCompressPercent = 0.25;

int removableSeamsV;
int removableSeamsH;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix(":/img/images/dock.png");
    ui->imgLabel1->setPixmap(pix);
    ui->imgLabel2->setPixmap(pix);

//    QWidget* widget = new QWidget;
//    ui->tabWidget->addTab(widget, "Seam");
//    QLabel* seamLabel;

    imageLabel = new childQlabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filt = "Image files ( *.png *jpeg *.jpg );; All files ( *.* )";
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image",
                                                    "/home/ioli/Pictures/", filt);
    if (fileName != ""){
        this->fileName = fileName.toStdString();
        QPixmap pix(fileName);
        ui->imgLabel1->setPixmap(pix);
        ui->imgLabel2->setPixmap(pix);
    }
    // Updating the global variables
    Mat_<Vec3b> temp = imread(this->fileName);
    IMAGE_COLS_ = temp.cols;
    IMAGE_ROWS_ = temp.rows;
    temp.release();
}

void MainWindow::on_actionSave_triggered()
{

}

void MainWindow::on_actionSave_as_triggered()
{

}

void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionInfo_triggered()
{
    QString about;
    about = "open image from the file menu \n";
    about += "process the image from run menu \n";
    about += "view outputs from respective tabs";

    QMessageBox::about(this, "How to?", about);
}

void MainWindow::on_actionProcess_triggered()
{
    if (this->fileName == ""){
        QMessageBox::warning(this, "No image to process",
                             "No! image opened to process");
        return;
    }

    qDebug() << QString::fromStdString(this->fileName);
    opencvStuff(this->fileName);
    ui->imgLabel1->setPixmap(QPixmap::fromImage(this->final));

    QMessageBox::information(this, "Done", "Done!!");

    // tabs
    QWidget* widget = new QWidget;
    ui->tabWidget->addTab(widget, "Seam");
    QLabel* seamLabel;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    vCompressPercent = 0.01 * ui->horizontalSlider->value();
    qDebug() << vCompressPercent;
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    hCompressPercent = 0.01 * ui->verticalSlider->value();
    qDebug() << hCompressPercent;
}

void MainWindow::opencvStuff(string fileName){
    Mat_<Vec3b> image = imread(fileName);

    if (!image.data) {
        qDebug() << "Invalid input";
        image.release();
        return;
    } else {
        qDebug() << "Processing!! please wait";
    }


//        imshow("Original Image", image);
    SeamCarver s(image);
//	imshow("Gradient", s.energy);
//	Mat tmp = s.energy/195075.0*255.0;
//	s.energy.convertTo(tmp,CV_8U,-1);
//	imwrite("bench_gradient.jpg", tmp);
//	vector<uint> sm = s.findVerticalSeam();
//	s.showVerticalSeam(sm);

    removableSeamsH = hCompressPercent * s.getImage().rows;
    removableSeamsV = vCompressPercent * s.getImage().cols;

    qDebug() << s.getImage().cols << " total cols";
    qDebug() << s.getImage().rows << " total rows";

    qDebug() << removableSeamsH << " rows to be removed";
    qDebug() << removableSeamsV << " cols to be removed";

    s.showEnergy();

    // finding and removing seams
    for (int i = 0; i < removableSeamsH; ++i) {
        vector<uint> seam = s.findHorizontalSeam();
        s.markHorizontalSeam(seam);
        s.removeHorizontalSeam(seam);
    }
    // resize the verticalMark from original to the
    // that of the size after horizontal removal
    //like this -> image = image(Rect(0, 0, image.cols, image.rows));
//    s.getVerticalMark() = s.getVerticalMark()(Rect(0, 0, 100, 100));

    // after the container is of appropriate size copy the data
//    s.getImage().copyTo(s.getVerticalMark());
//    imshow("vertical mark", s.getVerticalMark());
    for (int i = 0; i < removableSeamsV; ++i) {
        vector<uint> seam = s.findVerticalSeam();
        //s.markVerticalSeam(seam);
        s.removeVerticalSeam(seam);
    }

    // converting image to qt friendly format
      Mat_<Vec3b> qtImage;
      cvtColor(s.getImage(), qtImage, CV_BGR2RGB);
      this->final =  QImage(qtImage.data, qtImage.cols, qtImage.rows,
                        qtImage.step, QImage::Format_RGB888).copy();

//    temp = s.getImage();
//    cvtColor(temp, temp, CV_BGR2RGB);
//    this->final = QPixmap::fromImage(QImage(image.data,
//                     image.cols, image.rows, image.step, QImage::Format_RGB888));

//      imshow("Carved Image", s.getImage());
      qDebug() << "Complete!!";

    //  imshow("Carved Image", s.getImage());
    //	cout << "Seam Length: " << seam.size() << endl;
    //  s.showImage();
//   s.showEnergy();
//        while (waitKey(20) != 27);

    //	imwrite("bench_carved.jpg", s.getImage());

    //	for (int i = 0; i < 5; ++i) {
    //		for (int j = 0; j < 5; ++j) {
    //			cout << s.energy.at<uint32_t>(i,j) << " ";
    //		}
    //		cout << endl;
    //	}
        image.release();
}
