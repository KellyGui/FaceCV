#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <iterator>

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
    void on_selectButton_clicked();

    void on_detButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    void detectAndDraw( Mat& img, CascadeClassifier& cascade,double scale, bool tryflip );
    void displayResult(Mat mat);
    QString fileName;
};

#endif // MAINWINDOW_H
