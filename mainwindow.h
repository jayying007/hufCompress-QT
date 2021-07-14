#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "compress.h"
#include "unpack.h"
#include<QPainter>
#include<QBitmap>
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
    void progress(int n);

    void on_choosePath1_clicked();

    void on_Savepath1_clicked();

    void on_Compress_clicked();

    void on_choosePath2_clicked();

    void on_SavePath2_clicked();

    void on_Unpack_clicked();

    void on_Show_currentChanged(int index);

    void ErrorFile();

private:
    Ui::MainWindow *ui;
    Compress *press;
    Unpack *pack;
    QPixmap pix;
};

#endif // MAINWINDOW_H
