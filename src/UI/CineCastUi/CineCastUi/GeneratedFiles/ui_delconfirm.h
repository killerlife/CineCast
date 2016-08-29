/********************************************************************************
** Form generated from reading ui file 'delconfirm.ui'
**
** Created: Tue Aug 16 10:38:28 2016
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DELCONFIRM_H
#define UI_DELCONFIRM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DelConfirmClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QListWidget *listWidgetRaid;
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listWidgetRD;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonDelete;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *DelConfirmClass)
    {
        if (DelConfirmClass->objectName().isEmpty())
            DelConfirmClass->setObjectName(QString::fromUtf8("DelConfirmClass"));
        DelConfirmClass->resize(601, 373);
        verticalLayout = new QVBoxLayout(DelConfirmClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(DelConfirmClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(DelConfirmClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setMargin(11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        listWidgetRaid = new QListWidget(groupBox);
        listWidgetRaid->setObjectName(QString::fromUtf8("listWidgetRaid"));

        verticalLayout_3->addWidget(listWidgetRaid);


        horizontalLayout_2->addWidget(groupBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        groupBox_2 = new QGroupBox(DelConfirmClass);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listWidgetRD = new QListWidget(groupBox_2);
        listWidgetRD->setObjectName(QString::fromUtf8("listWidgetRD"));

        verticalLayout_2->addWidget(listWidgetRD);


        horizontalLayout_4->addWidget(groupBox_2);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        pushButtonDelete = new QPushButton(DelConfirmClass);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));

        horizontalLayout_3->addWidget(pushButtonDelete);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButtonCancel = new QPushButton(DelConfirmClass);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        horizontalLayout_3->addWidget(pushButtonCancel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(DelConfirmClass);

        QMetaObject::connectSlotsByName(DelConfirmClass);
    } // setupUi

    void retranslateUi(QDialog *DelConfirmClass)
    {
        DelConfirmClass->setWindowTitle(QApplication::translate("DelConfirmClass", "Delete Films", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DelConfirmClass", "Please confirm films will be delete.", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("DelConfirmClass", "RAID", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("DelConfirmClass", "Removeable Disk", 0, QApplication::UnicodeUTF8));
        pushButtonDelete->setText(QApplication::translate("DelConfirmClass", "Delete", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("DelConfirmClass", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DelConfirmClass);
    } // retranslateUi

};

namespace Ui {
    class DelConfirmClass: public Ui_DelConfirmClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELCONFIRM_H
