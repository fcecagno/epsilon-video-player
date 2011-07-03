#include <QtGui/QApplication>
#include "mainwindow.h"

#ifdef UNIX
#include <X11/Xlib.h>
#endif

int main(int argc, char *argv[])
{
#ifdef UNIX
    XInitThreads();
#endif

    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    VideoGL videoWidget(0);
//    a.setActiveWindow(videoWidget);
    videoWidget.resize(500,500);
    videoWidget.show();
    VideoDrawer videoDrawer(&videoWidget, "../Data/Recitation13.wmv");
//    videoDrawer.start();

    return a.exec();
}
