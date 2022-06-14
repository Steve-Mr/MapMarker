#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_Button_load_map_clicked();

    void on_Text_scale_textChanged();

    void on_Button_save_scale_clicked();

    void on_Button_mark_clicked();

    void on_Text_points_textChanged();

    void on_Button_save_points_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    bool isMapLoaded = false;
    bool isScaleSet = false;
    bool isMarkerClicked = false;
    int map_width;
    int map_height;
    int label_width;
    int label_height;
    double scale_num;
    bool is_numeric(std::string const & str);

};
#endif // MAINWINDOW_H
