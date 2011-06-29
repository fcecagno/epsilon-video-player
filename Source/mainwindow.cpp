#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    videoWidget = new VideoGL(parent);
    videoDrawer = new VideoDrawer(videoWidget, "../Data/Recitation13.mp4");
    videoDrawer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
