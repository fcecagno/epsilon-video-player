#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    videoWidget = new VideoGL(parent);
    videoDrawer = new VideoDrawer(videoWidget, "/home/felipe/Codes/epsilon-video-player/The Killers - Human.avi");
    videoDrawer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
