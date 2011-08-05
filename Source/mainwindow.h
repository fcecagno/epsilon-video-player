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
	void play();
	void stop();
	void backward();
	void backwardOne();
	void forwardOne();
	void forward();

private:
    Ui::MainWindow *ui;
	Homography *homography;
    MediaLoader *loader;
};

#endif // __MAINWINDOW_H
