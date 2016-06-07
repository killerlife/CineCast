/********************************************************************************
** Form generated from reading ui file 'numberim.ui'
**
** Created: Tue May 31 15:14:05 2016
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_NUMBERIM_H
#define UI_NUMBERIM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_NumberIMClass
{
public:
    QPushButton *pushButton6;
    QPushButton *pushButton3;
    QPushButton *pushButton0;
    QPushButton *pushButton2;
    QPushButton *pushButtonClose;
    QPushButton *pushButton4;
    QPushButton *pushButton7;
    QPushButton *pushButton9;
    QPushButton *pushButton8;
    QPushButton *pushButton1;
    QPushButton *pushButtonOK;
    QLineEdit *lineEdit;
    QPushButton *pushButton5;
    QPushButton *pushButtonBack;
    QPushButton *pushButtonDot;

    void setupUi(QDialog *NumberIMClass)
    {
        if (NumberIMClass->objectName().isEmpty())
            NumberIMClass->setObjectName(QString::fromUtf8("NumberIMClass"));
        NumberIMClass->resize(231, 260);
        pushButton6 = new QPushButton(NumberIMClass);
        pushButton6->setObjectName(QString::fromUtf8("pushButton6"));
        pushButton6->setGeometry(QRect(130, 110, 41, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Book Antiqua"));
        font.setPointSize(14);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        pushButton6->setFont(font);
        pushButton3 = new QPushButton(NumberIMClass);
        pushButton3->setObjectName(QString::fromUtf8("pushButton3"));
        pushButton3->setGeometry(QRect(130, 160, 41, 41));
        pushButton3->setFont(font);
        pushButton0 = new QPushButton(NumberIMClass);
        pushButton0->setObjectName(QString::fromUtf8("pushButton0"));
        pushButton0->setGeometry(QRect(10, 210, 101, 41));
        pushButton0->setFont(font);
        pushButton2 = new QPushButton(NumberIMClass);
        pushButton2->setObjectName(QString::fromUtf8("pushButton2"));
        pushButton2->setGeometry(QRect(70, 160, 41, 41));
        pushButton2->setFont(font);
        pushButtonClose = new QPushButton(NumberIMClass);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));
        pushButtonClose->setGeometry(QRect(180, 60, 41, 91));
        pushButtonClose->setFont(font);
        pushButton4 = new QPushButton(NumberIMClass);
        pushButton4->setObjectName(QString::fromUtf8("pushButton4"));
        pushButton4->setGeometry(QRect(10, 110, 41, 41));
        pushButton4->setFont(font);
        pushButton7 = new QPushButton(NumberIMClass);
        pushButton7->setObjectName(QString::fromUtf8("pushButton7"));
        pushButton7->setGeometry(QRect(10, 60, 41, 41));
        pushButton7->setFont(font);
        pushButton9 = new QPushButton(NumberIMClass);
        pushButton9->setObjectName(QString::fromUtf8("pushButton9"));
        pushButton9->setGeometry(QRect(130, 60, 41, 41));
        pushButton9->setFont(font);
        pushButton8 = new QPushButton(NumberIMClass);
        pushButton8->setObjectName(QString::fromUtf8("pushButton8"));
        pushButton8->setGeometry(QRect(70, 60, 41, 41));
        pushButton8->setFont(font);
        pushButton1 = new QPushButton(NumberIMClass);
        pushButton1->setObjectName(QString::fromUtf8("pushButton1"));
        pushButton1->setGeometry(QRect(10, 160, 41, 41));
        pushButton1->setFont(font);
        pushButtonOK = new QPushButton(NumberIMClass);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(180, 160, 41, 91));
        pushButtonOK->setFont(font);
        lineEdit = new QLineEdit(NumberIMClass);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(10, 10, 161, 31));
        lineEdit->setFont(font);
        pushButton5 = new QPushButton(NumberIMClass);
        pushButton5->setObjectName(QString::fromUtf8("pushButton5"));
        pushButton5->setGeometry(QRect(70, 110, 41, 41));
        pushButton5->setFont(font);
        pushButtonBack = new QPushButton(NumberIMClass);
        pushButtonBack->setObjectName(QString::fromUtf8("pushButtonBack"));
        pushButtonBack->setGeometry(QRect(180, 10, 41, 31));
        pushButtonBack->setFont(font);
        pushButtonDot = new QPushButton(NumberIMClass);
        pushButtonDot->setObjectName(QString::fromUtf8("pushButtonDot"));
        pushButtonDot->setGeometry(QRect(130, 210, 41, 41));
        pushButtonDot->setFont(font);
        QWidget::setTabOrder(pushButtonClose, lineEdit);
        QWidget::setTabOrder(lineEdit, pushButtonBack);
        QWidget::setTabOrder(pushButtonBack, pushButtonOK);
        QWidget::setTabOrder(pushButtonOK, pushButton7);
        QWidget::setTabOrder(pushButton7, pushButton8);
        QWidget::setTabOrder(pushButton8, pushButton9);
        QWidget::setTabOrder(pushButton9, pushButton4);
        QWidget::setTabOrder(pushButton4, pushButton5);
        QWidget::setTabOrder(pushButton5, pushButton6);
        QWidget::setTabOrder(pushButton6, pushButton1);
        QWidget::setTabOrder(pushButton1, pushButton2);
        QWidget::setTabOrder(pushButton2, pushButton3);
        QWidget::setTabOrder(pushButton3, pushButton0);
        QWidget::setTabOrder(pushButton0, pushButtonDot);

        retranslateUi(NumberIMClass);

        QMetaObject::connectSlotsByName(NumberIMClass);
    } // setupUi

    void retranslateUi(QDialog *NumberIMClass)
    {
        NumberIMClass->setWindowTitle(QApplication::translate("NumberIMClass", "NumberIM", 0, QApplication::UnicodeUTF8));
        pushButton6->setText(QApplication::translate("NumberIMClass", "6", 0, QApplication::UnicodeUTF8));
        pushButton3->setText(QApplication::translate("NumberIMClass", "3", 0, QApplication::UnicodeUTF8));
        pushButton0->setText(QApplication::translate("NumberIMClass", "0", 0, QApplication::UnicodeUTF8));
        pushButton2->setText(QApplication::translate("NumberIMClass", "2", 0, QApplication::UnicodeUTF8));
        pushButtonClose->setText(QApplication::translate("NumberIMClass", "Cancel", 0, QApplication::UnicodeUTF8));
        pushButton4->setText(QApplication::translate("NumberIMClass", "4", 0, QApplication::UnicodeUTF8));
        pushButton7->setText(QApplication::translate("NumberIMClass", "7", 0, QApplication::UnicodeUTF8));
        pushButton9->setText(QApplication::translate("NumberIMClass", "9", 0, QApplication::UnicodeUTF8));
        pushButton8->setText(QApplication::translate("NumberIMClass", "8", 0, QApplication::UnicodeUTF8));
        pushButton1->setText(QApplication::translate("NumberIMClass", "1", 0, QApplication::UnicodeUTF8));
        pushButtonOK->setText(QApplication::translate("NumberIMClass", "OK", 0, QApplication::UnicodeUTF8));
        pushButton5->setText(QApplication::translate("NumberIMClass", "5", 0, QApplication::UnicodeUTF8));
        pushButtonBack->setText(QApplication::translate("NumberIMClass", "\342\206\220", 0, QApplication::UnicodeUTF8));
        pushButtonDot->setText(QApplication::translate("NumberIMClass", ".", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(NumberIMClass);
    } // retranslateUi

};

namespace Ui {
    class NumberIMClass: public Ui_NumberIMClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NUMBERIM_H
