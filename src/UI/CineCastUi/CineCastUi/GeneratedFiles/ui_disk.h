/********************************************************************************
** Form generated from reading ui file 'disk.ui'
**
** Created: Tue Aug 16 10:38:28 2016
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DISK_H
#define UI_DISK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DiskClass
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QProgressBar *progressBar;
    QPushButton *pushButton_format;
    QHBoxLayout *horizontalLayout;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *DiskClass)
    {
        if (DiskClass->objectName().isEmpty())
            DiskClass->setObjectName(QString::fromUtf8("DiskClass"));
        DiskClass->resize(595, 448);
        verticalLayout_2 = new QVBoxLayout(DiskClass);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(DiskClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QFont font;
        font.setFamily(QString::fromUtf8("Book Antiqua"));
        font.setPointSize(20);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        groupBox->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        horizontalLayout_4->addWidget(progressBar);

        pushButton_format = new QPushButton(groupBox);
        pushButton_format->setObjectName(QString::fromUtf8("pushButton_format"));
        pushButton_format->setMaximumSize(QSize(120, 35));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Book Antiqua"));
        font1.setPointSize(18);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        font1.setStrikeOut(false);
        pushButton_format->setFont(font1);

        horizontalLayout_4->addWidget(pushButton_format);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textBrowser = new QTextBrowser(groupBox);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        horizontalLayout->addWidget(textBrowser);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(groupBox);


        retranslateUi(DiskClass);

        QMetaObject::connectSlotsByName(DiskClass);
    } // setupUi

    void retranslateUi(QWidget *DiskClass)
    {
        DiskClass->setWindowTitle(QApplication::translate("DiskClass", "Disk", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("DiskClass", "Format Removeable HDD", 0, QApplication::UnicodeUTF8));
        pushButton_format->setText(QApplication::translate("DiskClass", "Format", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DiskClass);
    } // retranslateUi

};

namespace Ui {
    class DiskClass: public Ui_DiskClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISK_H
