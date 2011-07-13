#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "VideoGL.h"
#include "Homography.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void load();

private:
    Ui::MainWindow *ui;
	Homography *homography;
    MediaLoader *loader;
	
};

#endif // MAINWINDOW_H
