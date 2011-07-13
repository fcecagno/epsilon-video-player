#include <QDialog>
#include <QDir>
#include <QString>
#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), loader(NULL), ui(new Ui::MainWindow){
    ui->setupUi(this);

	// connect signals to slots
	// menu
	connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(load()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(quit()));
        // \todo remove for the final version
        load();
}

MainWindow::~MainWindow()
{
	if(loader != NULL)
		delete loader;
    delete ui;
}

void MainWindow::load()
{
        // \todo uncomment this and comment the next line for the final version
//	QString filename = QFileDialog::getOpenFileName(this->parentWidget(), tr("Open File"), QDir::currentPath());
        QString filename = "../Data/Recitation13.wmv";
	if(!filename.isEmpty()) {
		loader = new MediaLoader(filename);
		loader->start();

		ui->videoWidget->setVideoLoader(loader->getVideoHandler());
	}
}
