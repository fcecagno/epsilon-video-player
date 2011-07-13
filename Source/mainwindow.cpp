#include <QDialog>
#include <QDir>
#include <QString>
#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), loader(NULL), homography(NULL)
{
    ui->setupUi(this);

	// connect signals to slots
	// menu
	connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(load()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(quit()));
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
