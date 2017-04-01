#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QImage>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化转换数组
    for (int i = 0; i < 256; i++)
    {
        highlights_add[i] = shadows_sub[255 - i] = (1.075 - 1 / ((double) i / 16.0 + 1));
        midtones_add[i] = midtones_sub[i] = 0.667 * (1 - (((double) i - 127.0) / 127.0)*(((double) i - 127.0) / 127.0));
        shadows_add[i] = highlights_sub[i] = 0.667 * (1 - (((double) i - 127.0) / 127.0)*(((double) i - 127.0) / 127.0));
    }

    dx = 0; //大小
    fc = 0; //方差

    ui->openButton->setStyleSheet("QPushButton{border: 3px solid grey;background:rgb(183,203,188);border-radius: 5px;}QPushButton:hover{border-color:rgb(183,203,188);}QPushButton:pressed{border-color:gray}");
    ui->saveButton->setStyleSheet("QPushButton{border: 3px solid grey;background:rgb(183,203,188);border-radius: 5px;}QPushButton:hover{border-color:rgb(183,203,188);}QPushButton:pressed{border-color:gray}");
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

    ui->showLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->dxLabel->setText(tr("%1").arg(value));
    if(fileName.isEmpty())
        return;
    dx = value*5;
    fc = value*12.5;
    //双边滤波
    bilateralFilter(src,dst,dx,fc,fc);
    ui->horizontalSlider_2->setValue(1);
    ui->horizontalSlider_3->setValue(0);
    displayResult(dst);
    result = dst.clone();
    white = dst.clone();
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->fcLabel->setText(tr("%1").arg(value));
    if(fileName.isEmpty())
        return;
    Mat temp1,temp2,temp3,temp4;   
    ui->horizontalSlider_3->setValue(0);

    //Dest =(Src * (100 - Opacity) + (Src + 2 * GuassBlur(EPFFilter(Src) - Src + 128) - 256) * Opacity) /100 ;
    bilateralFilter(src,temp1,dx,fc,fc);
    temp2 = (temp1-src+128);
    GaussianBlur(temp2,temp3,Size((2*value-1),(2*value-1)),0,0);
    temp4 = src+2*temp3-255;
    int p = 50;//透明度
    dst = (src*(100-p) + temp4*p)/100;

    displayResult(dst);
    result = dst.clone();
    white = dst.clone();
}

void MainWindow::on_openButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,tr("打开图片"),tr("."));
    if(fileName.isEmpty())
        return;
    src = imread(fileName.toStdString());
    result = src.clone();
    white = src.clone();

    ui->showLabel->setPixmap(QPixmap(fileName));
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider_2->setValue(1);
}

void MainWindow::on_saveButton_clicked()
{
    if(fileName.isEmpty())
        return;
    QString saveName = QFileDialog::getSaveFileName(this,tr("保存文件"),tr("."),"JPEG(*.jpg);;PNG(*.png);;BMP(*.bmp)");
    if(saveName.isEmpty())
        return;
    imwrite(saveName.toStdString(),result);
}


void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->whiteLabel->setText(tr("%1").arg(value));
    if(fileName.isEmpty())
        return;
    Mat temp = white.clone();
    BalanceColor(temp,value*15);  
    displayResult(temp);
    result = temp.clone();
}


int MainWindow::FMax(const int X, const int Y)
{
    return (X < Y ? Y : X);
}

int MainWindow::FMin(const int X, const int Y)
{
    return (Y < X ? Y : X);
}

void MainWindow::BalanceColor(Mat& bitmap,int value)
{
    int red, green, blue;
    unsigned char r_lookup[256],g_lookup[256],b_lookup[256];

    for (int i = 0; i < 256; i++)
    {
        red = i;
        green = i;
        blue = i;

        red += (int)( 0.0 * shadows_sub[red] + value * midtones_add[red] + 0.0 * highlights_sub[red]);
        red = FMax(0,FMin(0xFF,red));

        green += (int)( 0.0 * shadows_sub[green] + value * midtones_add[green] + 0.0 * highlights_sub[green]);
        green = FMax(0,FMin(0xFF,green));

        blue += (int)( 0.0 * shadows_sub[blue] + value * midtones_add[blue] + 0.0 * highlights_sub[blue]);
        blue = FMax(0,FMin(0xFF,blue));

        r_lookup[i] = (unsigned char)red;
        g_lookup[i] = (unsigned char)green;
        b_lookup[i] = (unsigned char)blue;
    }

    for (int row = 0; row < bitmap.rows; row++)
    {
        for (int col = 0; col < bitmap.cols; col++)
        {
            bitmap.at<Vec3b>(row, col)[0] = b_lookup[bitmap.at<Vec3b>(row, col)[0]];
            bitmap.at<Vec3b>(row, col)[1] = g_lookup[bitmap.at<Vec3b>(row, col)[1]];
            bitmap.at<Vec3b>(row, col)[2] = r_lookup[bitmap.at<Vec3b>(row, col)[2]];
        }
    }

}
