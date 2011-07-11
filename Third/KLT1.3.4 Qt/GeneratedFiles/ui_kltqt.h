/********************************************************************************
** Form generated from reading UI file 'kltqt.ui'
**
** Created: Wed Jul 6 15:04:56 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KLTQT_H
#define UI_KLTQT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KLTQtClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *KLTQtClass)
    {
        if (KLTQtClass->objectName().isEmpty())
            KLTQtClass->setObjectName(QString::fromUtf8("KLTQtClass"));
        KLTQtClass->resize(600, 400);
        menuBar = new QMenuBar(KLTQtClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        KLTQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(KLTQtClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        KLTQtClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(KLTQtClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        KLTQtClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(KLTQtClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        KLTQtClass->setStatusBar(statusBar);

        retranslateUi(KLTQtClass);

        QMetaObject::connectSlotsByName(KLTQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *KLTQtClass)
    {
        KLTQtClass->setWindowTitle(QApplication::translate("KLTQtClass", "KLTQt", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class KLTQtClass: public Ui_KLTQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KLTQT_H
