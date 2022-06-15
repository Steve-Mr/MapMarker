#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTextBrowser>
#include <QPlainTextEdit>
#include <QLabel>

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

    void on_buttonLoadMap_clicked();

    void on_textScale_textChanged();

    void on_buttonSaveScale_clicked();

    void on_buttonMark_clicked();

    void on_textCoords_textChanged();

    void on_buttonSaveCoords_clicked();

private:
    Ui::MainWindow *ui;

    QPushButton *buttonLoadMap;
    QPushButton *buttonSaveScale;
    QPushButton *buttonMarkPoints;
    QPushButton *buttonSaveCoords;

    QPlainTextEdit *textMapScale;
    QTextBrowser *textPointsCoord;

    QLabel *labelMap;

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
    void loadToLabel(QString currentFile);

};
#endif // MAINWINDOW_H
