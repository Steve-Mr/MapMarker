#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QXmlStreamReader"
#include "QDebug"
#include "QMouseEvent"
#include "QPainter"

#include "sstream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Button_save_scale->setEnabled(false);
    ui->Button_mark->setEnabled(false);
    ui->Button_save_points->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_load_map_clicked()
{
    ui->Text_points->clear();
    ui->Button_save_points->setEnabled(false);
    if(isMapLoaded){
        isMapLoaded = false;
        isScaleSet = false;
        isMarkerClicked = false;
        ui->Button_mark->setEnabled(false);
        ui->Text_scale->clear();
    }

    ui->Button_save_scale->setEnabled(false);


    currentFile = QFileDialog::getOpenFileName(this, "Choose Map",
                                                        "/home/maary/文档/code/robot/ourcar/navigation_stage/stage_config/maps",
                                                        tr("PGM(*.pgm)"));
    qDebug() << currentFile << Qt::endl;
    QPixmap image_map(currentFile);

    map_width = image_map.width();
    map_height = image_map.height();

    label_width = ui->Label_map->width();
    label_height = ui->Label_map->height();
    ui->Label_map->setPixmap(image_map.scaled(label_width, label_height, Qt::KeepAspectRatio));

    isMapLoaded = true;
    if(isScaleSet) ui->Button_mark->setEnabled(true);

}


void MainWindow::on_Text_scale_textChanged()
{
    ui->Button_save_scale->setEnabled(true);
}


void MainWindow::on_Button_save_scale_clicked()
{
    QString scale = ui->Text_scale->toPlainText();
    std::string scale_string = scale.toStdString();
    if (is_numeric(scale_string)){
        ui->Button_save_scale->setEnabled(false);
        scale_num = scale.toDouble();

        isScaleSet = true;
        if(isMapLoaded) ui->Button_mark->setEnabled(true);
        qDebug() << QString::number(scale_num) << Qt::endl;
    }else{
        qDebug() << "error value" << Qt::endl;
    }
}


void MainWindow::on_Button_mark_clicked()
{
    qDebug() << ui->Label_map->geometry() << Qt::endl;
    if(!isMarkerClicked){
        ui->Label_map->installEventFilter(this);
        ui->Button_mark->setText("Done");
        isMarkerClicked = true;
    }else{
        ui->Label_map->removeEventFilter(this);
        ui->Button_mark->setText("Mark Points");
        ui->Button_save_points->setEnabled(true);
        ui->Button_mark->setEnabled(false);
        isMarkerClicked = false;
    }
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

        auto pix = ui->Label_map->pixmap(Qt::ReturnByValue);
        QPainter painter(&pix);
        QPen paintPen(Qt::red);
        paintPen.setWidth(10);
        painter.setPen(paintPen);
        painter.drawPoint(point);

        ui->Label_map->setPixmap(pix);

    }
    return false;
}

void MainWindow::on_Text_points_textChanged()
{
    ui->Button_save_points->setEnabled(true);
}


bool MainWindow::is_numeric (std::string const & str)
{
    auto result = double();
    auto i = std::istringstream(str);

    i >> result;

    return !i.fail() && i.eof();
}

void MainWindow::on_Button_save_points_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString text = ui->Text_points->toPlainText();
    out << text;
    file.close();

    ui->Button_save_points->setEnabled(false);
    ui->Text_points->clear();
    ui->Button_mark->setEnabled(true);
    QPixmap image_map(currentFile);

    map_width = image_map.width();
    map_height = image_map.height();

    label_width = ui->Label_map->width();
    label_height = ui->Label_map->height();
    ui->Label_map->setPixmap(image_map.scaled(label_width, label_height, Qt::KeepAspectRatio));
}

