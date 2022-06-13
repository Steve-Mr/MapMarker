#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QXmlStreamReader"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_Button_load_config_clicked()
{
    QString configFileName = QFileDialog::getOpenFileName();
    QFile configFile(configFileName);
    currentFile = configFileName;
    if(!configFile.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file: "+ configFile.errorString());
    }
    QTextStream in(&configFile);
    QString config = in.readAll();

    QXmlStreamReader reader(config);
    while(!reader.atEnd() && !reader.hasError()){
        if(reader.readNext() == QXmlStreamReader::StartElement){
            if(reader.name() == "include" || reader.name() == "node"){
                for(auto &attribute : reader.attributes()){
                    qDebug() << attribute.name() << attribute.value() << endl;
                }
            }
            if(reader.name() == "node"){
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("name") && attributes.value("name") == "map_server"){
                    qDebug() << attributes.value("args") << Qt::endl;
                }
            }
            if(reader.name() == "include"){

            }
        }
    }
    ui->Text_config->setText(config);
    configFile.close();

}

