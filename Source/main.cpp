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
    MainWindow w;
    w.show();
    return a.exec();
}
