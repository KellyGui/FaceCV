#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//QLabel显示mat图像
void MainWindow::displayResult(Mat mat)
{
    Mat rgb;
    QImage img;
    if(mat.channels() == 3)    // RGB image
    {
        cvtColor(mat,rgb,CV_BGR2RGB);
        img = QImage((const uchar*)(rgb.data),  //(const unsigned char*)
                     rgb.cols,rgb.rows,
                     rgb.cols*rgb.channels(),   //new add
                     QImage::Format_RGB888);
    }else                     // gray image
    {
        img = QImage((const uchar*)(mat.data),
                     mat.cols,mat.rows,
                     mat.cols*mat.channels(),    //new add
                     QImage::Format_Indexed8);
    }

    ui->newLabel->clear();
    ui->newLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::detectAndDraw( Mat& img, CascadeClassifier& cascade,double scale, bool tryflip )
{
    int i = 0;
    double t = 0;
    //建立用于存放人脸的向量容器
    vector<Rect> faces, faces2;
    //定义一些颜色，用来标示不同的人脸
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    //建立缩小的图片，加快检测速度
    //nt cvRound (double value) 对一个double型的数进行四舍五入，并返回一个整型数！
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
    //转成灰度图像，Harr特征基于灰度图
    cvtColor( img, gray, CV_BGR2GRAY );
    //改变图像大小，使用双线性差值
    cv::resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    //变换后的图像进行直方图均值化处理
    equalizeHist( smallImg, smallImg );

    //程序开始和结束插入此函数获取时间，经过计算求得算法执行时间
    t = (double)cvGetTickCount();
    //检测人脸
    //detectMultiScale函数中smallImg表示的是要检测的输入图像为smallImg，faces表示检测到的人脸目标序列，1.1表示
    //每次图像尺寸减小的比例为1.1，2表示每一个目标至少要被检测到3次才算是真的目标(因为周围的像素和不同的窗口大
    //小都可以检测到人脸),CV_HAAR_SCALE_IMAGE表示不是缩放分类器来检测，而是缩放图像，Size(30, 30)为目标的
    //最小最大尺寸
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30));
    //如果使能，翻转图像继续检测
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CV_HAAR_FIND_BIGGEST_OBJECT
                                 //|CV_HAAR_DO_ROUGH_SEARCH
                                 |CV_HAAR_SCALE_IMAGE
                                 ,
                                 Size(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++ )
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }
    t = (double)cvGetTickCount() - t;
    ui->textBrowser->append(tr("detection time = %1 ms\n").arg(t/((double)cvGetTickFrequency()*1000.)));
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {

        Point center;
        Scalar color = colors[i%8];
        int radius;

        //标示人脸时在缩小之前的图像上标示，所以这里根据缩放比例换算回去
        center.x = cvRound((r->x + r->width*0.5)*scale);
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        circle( img, center, radius, color, 3, 8, 0 );

    }

    displayResult(img);
}

void MainWindow::on_selectButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,tr("选择图片"),tr("."));
    if(fileName.isEmpty())
        return;
    ui->textBrowser->append(tr("打开图片:%1\n").arg(fileName));
    ui->oldLabel->setPixmap(QPixmap(fileName));
}

void MainWindow::on_detButton_clicked()
{
    if(ui->showLabel->text().isEmpty())
    {
        ui->textBrowser->append(tr("请选择一个级联!\n"));
        return;
    }
    Mat image;
    CascadeClassifier cascade;

    bool check;
    if(ui->checkBox->isChecked())
        check = 1;
    else
        check = 0;


    //训练好的文件名称，放置在可执行文件同目录下
    QString cascadename = tr("%1").arg(ui->showLabel->text());
    if(!cascade.load(cascadename.toStdString()))
        ui->textBrowser->append("WARNING: Could not load classifier cascade for nested objects\n");

    image = imread(fileName.toStdString(), CV_LOAD_IMAGE_COLOR);
    if(image.empty())
        ui->textBrowser->append("Couldn't read picture");

    detectAndDraw( image, cascade, 2, check);
}

void MainWindow::on_pushButton_clicked()
{
    QString tempfile = QFileDialog::getOpenFileName(this,tr("选择级联"),tr("."),tr("xmlfile(*.xml)"));
    if(tempfile.isEmpty())
        return;
    ui->showLabel->setText(tempfile.split("/").last());
}
