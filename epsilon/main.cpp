#include <QtGui/QApplication>
#include "mainwindow.h"
#include "VideoLoader.h"

int main(int argc, char *argv[])
{
    VideoLoader v;
    v.load("/home/felipe/Codes/epsilon-video-player/The Killers - Human.avi");
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
