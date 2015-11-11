/********************************************************************************
** Form generated from reading UI file 'contentmanagementform.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTENTMANAGEMENTFORM_H
#define UI_CONTENTMANAGEMENTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContentManagementForm
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_HDD_USB;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLabel *label_HDD;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_13;
    QTableView *tableView_HDD;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_HddOutput;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_HddDelete;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_USB;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_14;
    QTableView *tableView_USB;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *pushButton_Mount;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushButton_Uninstall;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *pushButton_UsbDelete;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *ContentManagementForm)
    {
        if (ContentManagementForm->objectName().isEmpty())
            ContentManagementForm->setObjectName(QStringLiteral("ContentManagementForm"));
        ContentManagementForm->resize(917, 547);
        ContentManagementForm->setAutoFillBackground(true);
        verticalLayout_3 = new QVBoxLayout(ContentManagementForm);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox_HDD_USB = new QGroupBox(ContentManagementForm);
        groupBox_HDD_USB->setObjectName(QStringLiteral("groupBox_HDD_USB"));
        verticalLayout_4 = new QVBoxLayout(groupBox_HDD_USB);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);

        label_5 = new QLabel(groupBox_HDD_USB);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_7->addWidget(label_5);


        verticalLayout_4->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_6->addItem(verticalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_3 = new QLabel(groupBox_HDD_USB);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_5->addWidget(label_3);

        label_HDD = new QLabel(groupBox_HDD_USB);
        label_HDD->setObjectName(QStringLiteral("label_HDD"));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label_HDD->setFont(font);

        horizontalLayout_5->addWidget(label_HDD);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_13);

        tableView_HDD = new QTableView(groupBox_HDD_USB);
        tableView_HDD->setObjectName(QStringLiteral("tableView_HDD"));

        verticalLayout->addWidget(tableView_HDD);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        pushButton_HddOutput = new QPushButton(groupBox_HDD_USB);
        pushButton_HddOutput->setObjectName(QStringLiteral("pushButton_HddOutput"));
        pushButton_HddOutput->setMaximumSize(QSize(120, 35));

        horizontalLayout->addWidget(pushButton_HddOutput);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_HddDelete = new QPushButton(groupBox_HDD_USB);
        pushButton_HddDelete->setObjectName(QStringLiteral("pushButton_HddDelete"));
        pushButton_HddDelete->setMaximumSize(QSize(120, 35));

        horizontalLayout->addWidget(pushButton_HddDelete);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_6->addLayout(verticalLayout);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_6->addItem(verticalSpacer_4);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_6->addItem(verticalSpacer_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_4 = new QLabel(groupBox_HDD_USB);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_3->addWidget(label_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_USB = new QLabel(groupBox_HDD_USB);
        label_USB->setObjectName(QStringLiteral("label_USB"));
        label_USB->setFont(font);

        horizontalLayout_2->addWidget(label_USB);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_14);

        tableView_USB = new QTableView(groupBox_HDD_USB);
        tableView_USB->setObjectName(QStringLiteral("tableView_USB"));

        verticalLayout_2->addWidget(tableView_USB);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_10 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        pushButton_Mount = new QPushButton(groupBox_HDD_USB);
        pushButton_Mount->setObjectName(QStringLiteral("pushButton_Mount"));
        pushButton_Mount->setMaximumSize(QSize(120, 35));

        horizontalLayout_4->addWidget(pushButton_Mount);

        horizontalSpacer_8 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        pushButton_Uninstall = new QPushButton(groupBox_HDD_USB);
        pushButton_Uninstall->setObjectName(QStringLiteral("pushButton_Uninstall"));
        pushButton_Uninstall->setMaximumSize(QSize(120, 35));

        horizontalLayout_4->addWidget(pushButton_Uninstall);

        horizontalSpacer_9 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);

        pushButton_UsbDelete = new QPushButton(groupBox_HDD_USB);
        pushButton_UsbDelete->setObjectName(QStringLiteral("pushButton_UsbDelete"));
        pushButton_UsbDelete->setMaximumSize(QSize(120, 35));

        horizontalLayout_4->addWidget(pushButton_UsbDelete);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_11);


        verticalLayout_2->addLayout(horizontalLayout_4);


        horizontalLayout_6->addLayout(verticalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_6->addItem(verticalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_6);


        verticalLayout_3->addWidget(groupBox_HDD_USB);


        retranslateUi(ContentManagementForm);

        QMetaObject::connectSlotsByName(ContentManagementForm);
    } // setupUi

    void retranslateUi(QWidget *ContentManagementForm)
    {
        ContentManagementForm->setWindowTitle(QApplication::translate("ContentManagementForm", "Form", 0));
        groupBox_HDD_USB->setTitle(QString());
        label_5->setText(QString());
        label_3->setText(QString());
        label_HDD->setText(QApplication::translate("ContentManagementForm", "HDD", 0));
        pushButton_HddOutput->setText(QString());
        pushButton_HddDelete->setText(QString());
        label_4->setText(QString());
        label_USB->setText(QApplication::translate("ContentManagementForm", "USB", 0));
        pushButton_Mount->setText(QString());
        pushButton_Uninstall->setText(QString());
        pushButton_UsbDelete->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ContentManagementForm: public Ui_ContentManagementForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTENTMANAGEMENTFORM_H
