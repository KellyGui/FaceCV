#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include<QPixmap>

using namespace std;
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
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);

    void on_openButton_clicked();
    void on_saveButton_clicked();

    void on_horizontalSlider_3_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    Mat src,dst,result,white;

    void displayResult(Mat mat);

    int dx;
    double fc;

    QString fileName;

    int FMax(const int X, const int Y);
    int FMin(const int X, const int Y);
    //转换数组
    double  highlights_add[256], highlights_sub[256];
    double  midtones_add[256], midtones_sub[256];
    double  shadows_add[256], shadows_sub[256];
    void BalanceColor(Mat& bitmap,int value);
};

#endif // MAINWINDOW_H
