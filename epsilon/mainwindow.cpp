#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    videoWidget(new VideoGL(this))
{
    ui->setupUi(this);

    loader = new VideoLoader("/home/felipe/Codes/epsilon-video-player/The Killers - Human.avi");
    loader->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
