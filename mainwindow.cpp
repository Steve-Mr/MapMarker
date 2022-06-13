#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QXmlStreamReader"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Button_save_scale->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_load_map_clicked()
{
    QString mapImageFile = QFileDialog::getOpenFileName(this, "Choose Map", "/home", tr("PGM(*.pgm)"));
    qDebug() << mapImageFile << Qt::endl;
    QPixmap image_map(mapImageFile);

    ui->Label_map->setScaledContents(true);
    ui->Label_map->setPixmap(image_map);
}


void MainWindow::on_Text_scale_textChanged()
{
    ui->Button_save_scale->setEnabled(true);
}


void MainWindow::on_Button_save_scale_clicked()
{
    QString scale = ui->Text_scale->toPlainText();
    if (scale != NULL){
        qDebug() << scale << Qt::endl;
    }else{
        qDebug() << "no value" << Qt::endl;
    }
}

