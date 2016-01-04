/********************************************************************************
** Form generated from reading ui file 'tmstest.ui'
**
** Created: Sat Nov 21 15:01:46 2015
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TMSTEST_H
#define UI_TMSTEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tmstestClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *tmstestClass)
    {
        if (tmstestClass->objectName().isEmpty())
            tmstestClass->setObjectName(QString::fromUtf8("tmstestClass"));
        tmstestClass->resize(600, 400);
        centralWidget = new QWidget(tmstestClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 20, 75, 23));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(190, 10, 75, 23));
        tmstestClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(tmstestClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 19));
        tmstestClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(tmstestClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        tmstestClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(tmstestClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        tmstestClass->setStatusBar(statusBar);

        retranslateUi(tmstestClass);

        QMetaObject::connectSlotsByName(tmstestClass);
    } // setupUi

    void retranslateUi(QMainWindow *tmstestClass)
    {
        tmstestClass->setWindowTitle(QApplication::translate("tmstestClass", "tmstest", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("tmstestClass", "PushButton", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("tmstestClass", "PushButton", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(tmstestClass);
    } // retranslateUi

};

namespace Ui {
    class tmstestClass: public Ui_tmstestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TMSTEST_H
