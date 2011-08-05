#include <QDialog>
#include <QDir>
#include <QString>
#include <QtGui>
#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), loader(NULL), homography(NULL), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->btStop->setVisible(false);

	// connect signals to slots
	// menu
	connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(load()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(quit()));
	connect(ui->btPlay, SIGNAL(clicked()), this, SLOT(play()));
	connect(ui->btStop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui->btForwardOne, SIGNAL(clicked()), this, SLOT(forwardOne()));
	
}

MainWindow::~MainWindow()
{
	if(loader != NULL)
		delete loader;
	if(homography != NULL)
		delete homography;
    delete ui;
}

void MainWindow::load()
{
	QDir dir = QDir::current();
	dir.cdUp();
	QString path = dir.path();

	QString filename = QFileDialog::getOpenFileName(this->parentWidget(), tr("Open File"), path+"/Data");

	if(!filename.isEmpty()) {

		loader = new MediaLoader(filename);

		filename = filename.left(filename.size()-4)+".txt";
		homography = new Homography();
		homography->clear();
		homography->load(filename.toStdString());

		loader->start();
		ui->videoWidget->setHomography(homography);
		ui->videoWidget->setVideoLoader(loader->getVideoHandler());		
	}
}

void MainWindow::play()
{
	ui->videoWidget->start();
	ui->btPlay->setVisible(false);
	ui->btStop->setVisible(true);
}

void MainWindow::stop()
{
	ui->videoWidget->stop();
	ui->btPlay->setVisible(true);
	ui->btStop->setVisible(false);
}

void MainWindow::backward()
{
}

void MainWindow::backwardOne()
{
}

void MainWindow::forwardOne()
{
	ui->videoWidget->forwardOne();
}

void MainWindow::forward()
{
}