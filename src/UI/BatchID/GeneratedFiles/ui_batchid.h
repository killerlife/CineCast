/********************************************************************************
** Form generated from reading ui file 'batchid.ui'
**
** Created: Wed Mar 16 14:10:18 2016
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_BATCHID_H
#define UI_BATCHID_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_BatchIDClass
{
public:
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButtonBrowse;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_3;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QPushButton *pushButton;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit_4;
    QPushButton *pushButtonBrowse_2;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit_5;
    QPushButton *pushButtonBrowse_3;
    QPushButton *pushButton_2;

    void setupUi(QDialog *BatchIDClass)
    {
        if (BatchIDClass->objectName().isEmpty())
            BatchIDClass->setObjectName(QString::fromUtf8("BatchIDClass"));
        BatchIDClass->resize(600, 291);
        groupBox = new QGroupBox(BatchIDClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 80, 561, 60));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        pushButtonBrowse = new QPushButton(groupBox);
        pushButtonBrowse->setObjectName(QString::fromUtf8("pushButtonBrowse"));

        horizontalLayout->addWidget(pushButtonBrowse);

        groupBox_2 = new QGroupBox(BatchIDClass);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 220, 561, 60));
        horizontalLayout_3 = new QHBoxLayout(groupBox_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setMargin(11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        spinBox = new QSpinBox(groupBox_2);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(10060001);
        spinBox->setMaximum(10069999);

        horizontalLayout_3->addWidget(spinBox);

        spinBox_2 = new QSpinBox(groupBox_2);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMinimum(10060001);
        spinBox_2->setMaximum(10069999);
        spinBox_2->setValue(10069999);

        horizontalLayout_3->addWidget(spinBox_2);

        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_3->addWidget(pushButton);

        groupBox_3 = new QGroupBox(BatchIDClass);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 150, 561, 60));
        horizontalLayout_2 = new QHBoxLayout(groupBox_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setMargin(11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineEdit_4 = new QLineEdit(groupBox_3);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        horizontalLayout_2->addWidget(lineEdit_4);

        pushButtonBrowse_2 = new QPushButton(groupBox_3);
        pushButtonBrowse_2->setObjectName(QString::fromUtf8("pushButtonBrowse_2"));

        horizontalLayout_2->addWidget(pushButtonBrowse_2);

        groupBox_4 = new QGroupBox(BatchIDClass);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setEnabled(false);
        groupBox_4->setGeometry(QRect(20, 10, 561, 60));
        horizontalLayout_4 = new QHBoxLayout(groupBox_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setMargin(11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lineEdit_5 = new QLineEdit(groupBox_4);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        horizontalLayout_4->addWidget(lineEdit_5);

        pushButtonBrowse_3 = new QPushButton(groupBox_4);
        pushButtonBrowse_3->setObjectName(QString::fromUtf8("pushButtonBrowse_3"));

        horizontalLayout_4->addWidget(pushButtonBrowse_3);

        pushButton_2 = new QPushButton(groupBox_4);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_4->addWidget(pushButton_2);


        retranslateUi(BatchIDClass);

        QMetaObject::connectSlotsByName(BatchIDClass);
    } // setupUi

    void retranslateUi(QDialog *BatchIDClass)
    {
        BatchIDClass->setWindowTitle(QApplication::translate("BatchIDClass", "BatchID", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("BatchIDClass", "BatchID File", 0, QApplication::UnicodeUTF8));
        pushButtonBrowse->setText(QApplication::translate("BatchIDClass", "Browse", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("BatchIDClass", "ID Range", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("BatchIDClass", "Batch", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("BatchIDClass", "Output Directory", 0, QApplication::UnicodeUTF8));
        pushButtonBrowse_2->setText(QApplication::translate("BatchIDClass", "Browse", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("BatchIDClass", "Generate BatchID File", 0, QApplication::UnicodeUTF8));
        pushButtonBrowse_3->setText(QApplication::translate("BatchIDClass", "Browse", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("BatchIDClass", "Generate", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(BatchIDClass);
    } // retranslateUi

};

namespace Ui {
    class BatchIDClass: public Ui_BatchIDClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BATCHID_H
