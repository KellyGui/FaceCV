#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QCloseEvent>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<opencv2/face.hpp>
#include<opencv2/face/facerec.hpp>
#include<iostream>

using namespace std;
using namespace cv;
using namespace face;

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
    void on_loadButton_clicked();
    void on_testButton_clicked();
    void on_regButton_clicked();
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    Ptr<LBPHFaceRecognizer> model;
    QString fileName,saveXml,saveName,name[10];
};

#endif // MAINWINDOW_H
