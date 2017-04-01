#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QFileDialog>
#include<QPixmap>
#include<QFile>
#include<QTextStream>

//正面,上,下,左,右5张.阉值85.00

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    saveName = "Names.txt";
    saveXml = "att_model.xml";

    model = createLBPHFaceRecognizer();

    if(QFile::exists(saveXml)&&QFile::exists(saveName))
    {
        model->load(saveXml.toStdString());
        QFile file(saveName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in(&file);
        QString lineText;
        while(!in.atEnd())
        {
            lineText = in.readLine();
            QString i = lineText.split(":").first();
            name[i.toInt()] = lineText.split(":").last();
        }
    }
//    for(int i=1;i<11;i++)
//        for(int j=1;j<10;j++)
//        {
//            QString file = "att_faces/s%1/%2.pgm";
//            images.push_back(imread(file.arg(i).arg(j).toStdString(), CV_LOAD_IMAGE_GRAYSCALE));
//            labels.push_back(i);
//        }

//    model = createLBPHFaceRecognizer();
//    //model->train(images, labels);
//    //model->save("att_model.xml");

//    model->load("att_model.xml");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    model->save(saveXml.toStdString());
    QFile file(saveName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return;
    QTextStream out(&file);
    for(int i=0;i<10;i++)
    {
        if(name[i].isEmpty())
            continue;
        out<<i<<":"<<name[i]<<"\n";
    }
    e->accept();
}

void MainWindow::on_loadButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,tr("选择图片"),tr("."));
    if(fileName.isEmpty())
        return;
    ui->showLabel->setPixmap(QPixmap(fileName));
    ui->textBrowser->append(tr("打开图片%1").arg(fileName.split("/").last()));
}

void MainWindow::on_testButton_clicked()
{
    if(fileName.isEmpty()||ui->nameEdit->text().isEmpty())
        return;
    vector<Mat> images;
    vector<int> labels;
    images.push_back(imread(fileName.toStdString(),CV_LOAD_IMAGE_GRAYSCALE));
    labels.push_back(ui->labelBox->value());
    name[ui->labelBox->value()] = ui->nameEdit->text();
    ui->textBrowser->append(tr("准备训练: 姓名:%1 标签:%2 ...").arg(ui->nameEdit->text()).arg(ui->labelBox->value()));
    model->update(images,labels);
    ui->textBrowser->append(tr("训练完成"));
}

void MainWindow::on_regButton_clicked()
{
    ui->nameLabel->clear();
    if(fileName.isEmpty())
        return;
    Mat image = imread(fileName.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
    model->setThreshold(ui->doubleSpinBox->value());
    ui->textBrowser->append(tr("准备识别Threshold:%1 ...").arg(ui->doubleSpinBox->value()));
    int result = model->predict(image);
    ui->textBrowser->append(tr("识别完成"));
    if(result < 0)
        ui->nameLabel->setText(tr("无法识别此人"));
    else
        ui->nameLabel->setText(tr("%1").arg(name[result]));
}
