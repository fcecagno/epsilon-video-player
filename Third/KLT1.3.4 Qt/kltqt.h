#ifndef KLTQT_H
#define KLTQT_H

#include <QtGui/QMainWindow>
#include "ui_kltqt.h"

class KLTQt : public QMainWindow
{
	Q_OBJECT

public:
	KLTQt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~KLTQt();

private:
	Ui::KLTQtClass ui;
};

#endif // KLTQT_H
