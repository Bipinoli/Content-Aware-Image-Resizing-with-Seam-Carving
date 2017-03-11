#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include <QMainWindow>
#include "childqlabel.h"


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
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionClose_triggered();

    void on_actionInfo_triggered();

    void on_actionProcess_triggered();

    void on_horizontalSlider_valueChanged(int value);

    void on_verticalSlider_valueChanged(int value);

private:
    void opencvStuff(std::string);
    Ui::MainWindow *ui;
    std::string fileName;
    QImage final;
    childQlabel *imageLabel;
//    QPixmap energy;
//    QPixmap horizontalSeams;
//    QPixmap verticalSeams;
};

#endif // MAINWINDOW_H
