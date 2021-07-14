#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QSplashScreen>
#include <QPixmap>
#include <mainwindow.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/brown.css");
    if( file.open(QFile::ReadOnly)){
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
        file.close();
    }
    else{
        QMessageBox::warning(NULL, "warning", "Open failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

    QPixmap pixmap(":/image/start.png");
    QSplashScreen screen(pixmap);
    screen.show();

    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 1500)
    {
         a.processEvents();
    }

    MainWindow w;
    w.show();
    screen.finish(&w);
    return a.exec();
}
