#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QXmlStreamReader"
#include "QDebug"
#include "QMouseEvent"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Button_save_scale->setEnabled(false);
    ui->Button_mark->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_load_map_clicked()
{
    QString mapImageFile = QFileDialog::getOpenFileName(this, "Choose Map",
                                                        "/home/maary/文档/code/robot/ourcar/navigation_stage/stage_config/maps",
                                                        tr("PGM(*.pgm)"));
    qDebug() << mapImageFile << Qt::endl;
    QPixmap image_map(mapImageFile);

    map_width = image_map.width();
    map_height = image_map.height();

    label_width = ui->Label_map->width();
    label_height = ui->Label_map->height();
    ui->Label_map->setPixmap(image_map.scaled(label_width, label_height, Qt::KeepAspectRatio));
}


void MainWindow::on_Text_scale_textChanged()
{
    ui->Button_save_scale->setEnabled(true);
}


void MainWindow::on_Button_save_scale_clicked()
{
    QString scale = ui->Text_scale->toPlainText();
    if (scale != NULL){
        //qDebug() << scale << Qt::endl;
        ui->Button_save_scale->setEnabled(false);
        ui->Button_mark->setEnabled(true);
        //TODO: 增加判断，目前判断不合理
        scale_num = scale.toDouble();
        qDebug() << QString::number(scale_num) << Qt::endl;
    }else{
        qDebug() << "no value" << Qt::endl;
    }
}


void MainWindow::on_Button_mark_clicked()
{
    qDebug() << ui->Label_map->geometry() << Qt::endl;
    ui->Label_map->installEventFilter(this);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    if(watched != ui->Label_map){
        return false;
    }
    if(event->type() != QEvent::MouseButtonPress){
        return false;
    }
    const QMouseEvent* const mEvent = static_cast<const QMouseEvent*>(event);
    const QPoint point = mEvent->pos();
    int display_width = ui->Label_map->pixmap(Qt::ReturnByValue).width();
    int display_height = ui->Label_map->pixmap(Qt::ReturnByValue).height();
    if (point.x() <= display_width && point.y() <= display_height){
        double x = point.x()/double(ui->Label_map->pixmap(Qt::ReturnByValue).width())*map_width*scale_num;
        double y = (label_height - point.y())/double(ui->Label_map->pixmap(Qt::ReturnByValue).height())*map_height*scale_num;
        ui->Text_points->append(QString::number(x) + "," + QString::number(y));
    }
    return false;
}
