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
    // 初始化 UI

    buttonLoadMap = ui->buttonLoadMap;
    buttonSaveScale = ui->buttonSaveScale;
    buttonMarkPoints = ui->buttonMark;
    buttonSaveCoords = ui->buttonSaveCoords;

    textMapScale = ui->textScale;
    textPointsCoord = ui->textCoords;

    labelMap = ui->labelMap;

    buttonSaveScale->setEnabled(false);
    buttonMarkPoints->setEnabled(false);
    buttonSaveCoords->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonLoadMap_clicked()
{
    // 加载地图需要清空对应数据
    textPointsCoord->clear();
    buttonSaveCoords->setEnabled(false);
    if(isMapLoaded){
        // 尚未加载地图则以下状态不需要修改
        isMapLoaded = false;
        isScaleSet = false;
        isMarkerClicked = false;
        buttonMarkPoints->setEnabled(false);
        textMapScale->clear();
    }
    buttonSaveScale->setEnabled(false);

    currentFile = QFileDialog::getOpenFileName(this, "Choose Map",
                                                        "/home/maary/文档/code/robot/ourcar/navigation_stage/stage_config/maps",
                                                        tr("PGM(*.pgm)"));
    qDebug() << currentFile << Qt::endl;
    loadToLabel(currentFile);

    isMapLoaded = true;
    if(isScaleSet) buttonMarkPoints->setEnabled(true);
    // TODO: 增加专门函数处理按键 ENABLED 状态
}


void MainWindow::on_textScale_textChanged()
{
    buttonSaveScale->setEnabled(true);
    // 输入 scale 之后才可以保存
}


void MainWindow::on_buttonSaveScale_clicked()
{
    // 如果输入的 Scale 是数字则保存并且修改 isScaleSet 状态
    QString scale = textMapScale->toPlainText();
    std::string scale_string = scale.toStdString();
    if (is_numeric(scale_string)){
        buttonSaveScale->setEnabled(false);
        scale_num = scale.toDouble();

        isScaleSet = true;
        if(isMapLoaded) buttonMarkPoints->setEnabled(true);
        qDebug() << QString::number(scale_num) << Qt::endl;
    }else{
        qDebug() << "error value" << Qt::endl;
    }
}


void MainWindow::on_buttonMark_clicked()
{
    qDebug() << labelMap->geometry() << Qt::endl;
    // 类似 toggleButton，在 Mark Points 和 Done 两种状态之间切换
    if(!isMarkerClicked){
        labelMap->installEventFilter(this);
        buttonMarkPoints->setText("Done");
        isMarkerClicked = true;
    }else{
        // 点击 Done 则不可再标点，只能保存现有点坐标
        labelMap->removeEventFilter(this);
        buttonMarkPoints->setText("Mark Points");
        buttonSaveCoords->setEnabled(true);
        buttonMarkPoints->setEnabled(false);
        isMarkerClicked = false;
    }
}


void MainWindow::on_textCoords_textChanged()
{
    buttonSaveCoords->setEnabled(true);
}


void MainWindow::on_buttonSaveCoords_clicked()
{
    // 将坐标保存为文件
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString text = textPointsCoord->toPlainText();
    out << text;
    file.close();

    // 点击 Save 保存后清空现有数据，但可以重新标点进行保存
    buttonSaveCoords->setEnabled(false);
    textPointsCoord->clear();
    buttonMarkPoints->setEnabled(true);
    loadToLabel(currentFile);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    // 记录鼠标在 Label 上点击位置
    if(watched != labelMap){
        return false;
    }
    if(event->type() != QEvent::MouseButtonPress){
        return false;
    }
    const QMouseEvent* const mEvent = static_cast<const QMouseEvent*>(event);
    const QPoint point = mEvent->pos();

    //获取当前显示的地图图片
    auto pix = labelMap->pixmap(Qt::ReturnByValue);
    // Label 中显示的图片和 Label 大小可能不同
    int display_width = pix.width();
    int display_height = pix.height();
    if (point.x() <= display_width && point.y() <= display_height){
        // 地图上坐标 X = 图上点坐标 X点 / 图片宽度 * 地图分辨率 * 比率
        double x = point.x()/double(display_width)*map_width*scale_num;
        // 记录鼠标坐标时原点为左上角点
        // 地图坐标原点为左下角
        double y = (label_height - point.y())/double(display_height)
                *map_height*scale_num;
        textPointsCoord->append(QString::number(x) + "," + QString::number(y));

        // 在 Label 显示的图片上进行已标点的绘制和显示
        // 每添加一个点则设置一张新图片
        // TODO： 使用继承重写 Qlabel 的方法
        QPainter painter(&pix);
        QPen paintPen(Qt::red);
        paintPen.setWidth(10);
        painter.setPen(paintPen);
        painter.drawPoint(point);

        labelMap->setPixmap(pix);

    }
    return false;
}


bool MainWindow::is_numeric (std::string const & str)
{
    auto result = double();
    auto i = std::istringstream(str);

    i >> result;

    return !i.fail() && i.eof();
}



void MainWindow::loadToLabel(QString currentFile){
    // 将 currentFile 对应的文件加载到 QLbael 中
    QPixmap image_map(currentFile);

    map_width = image_map.width();
    map_height = image_map.height();

    label_width = labelMap->width();
    label_height = labelMap->height();
    labelMap->setPixmap(image_map.scaled(label_width, label_height, Qt::KeepAspectRatio));
}

