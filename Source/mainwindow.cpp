#include <QDialog>
#include <QDir>
#include <QString>
#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), loader(NULL)
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
    delete ui;
}

void MainWindow::load()
{
	QString filename = QFileDialog::getOpenFileName(this->parentWidget(), tr("Open File"), QDir::currentPath());
	if(!filename.isEmpty()) {
		loader = new MediaLoader(filename);
		loader->start();

		ui->videoWidget->setVideoLoader(loader->getVideoHandler());
	}
}
