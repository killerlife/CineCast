/********************************************************************************
** Form generated from reading ui file 'content.ui'
**
** Created: Tue Aug 16 10:38:28 2016
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONTENT_H
#define UI_CONTENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContentClass
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_HDD_USB;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_HDD;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_5;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_5;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_6;
    QTreeWidget *treeWidget_HDD_info;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_ExporttoUSB;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_DeleteDir_HDD;
    QSpacerItem *horizontalSpacer_7;
    QFrame *line;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_USB;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QTreeWidget *treeWidget_USB_info;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *pushButton_Refush_Disk;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushButton_USB_UnMount;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *pushButton_DeleteDir_USB;
    QSpacerItem *horizontalSpacer_11;
    QProgressBar *progressBar_CopyDir;
    QLabel *label_Progress;

    void setupUi(QWidget *ContentClass)
    {
        if (ContentClass->objectName().isEmpty())
            ContentClass->setObjectName(QString::fromUtf8("ContentClass"));
        ContentClass->resize(834, 388);
        verticalLayout_3 = new QVBoxLayout(ContentClass);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setMargin(11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_HDD_USB = new QGroupBox(ContentClass);
        groupBox_HDD_USB->setObjectName(QString::fromUtf8("groupBox_HDD_USB"));
        QFont font;
        font.setFamily(QString::fromUtf8("Book Antiqua"));
        font.setPointSize(18);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        groupBox_HDD_USB->setFont(font);
        verticalLayout_4 = new QVBoxLayout(groupBox_HDD_USB);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setMargin(11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_HDD = new QLabel(groupBox_HDD_USB);
        label_HDD->setObjectName(QString::fromUtf8("label_HDD"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Book Antiqua"));
        font1.setPointSize(18);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setWeight(75);
        font1.setStrikeOut(false);
        label_HDD->setFont(font1);
        label_HDD->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/HDD2.png")));

        horizontalLayout_5->addWidget(label_HDD);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        groupBox = new QGroupBox(groupBox_HDD_USB);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_7 = new QHBoxLayout(groupBox);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setMargin(11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_5->addWidget(label);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_5->addWidget(label_2);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_5->addWidget(label_5);


        horizontalLayout_7->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_6->addWidget(label_3);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_6->addWidget(label_4);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_6->addWidget(label_6);


        horizontalLayout_7->addLayout(verticalLayout_6);


        horizontalLayout_5->addWidget(groupBox);


        verticalLayout->addLayout(horizontalLayout_5);

        treeWidget_HDD_info = new QTreeWidget(groupBox_HDD_USB);
        treeWidget_HDD_info->setObjectName(QString::fromUtf8("treeWidget_HDD_info"));
        treeWidget_HDD_info->setFont(font);
        treeWidget_HDD_info->setRootIsDecorated(false);

        verticalLayout->addWidget(treeWidget_HDD_info);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        pushButton_ExporttoUSB = new QPushButton(groupBox_HDD_USB);
        pushButton_ExporttoUSB->setObjectName(QString::fromUtf8("pushButton_ExporttoUSB"));
        pushButton_ExporttoUSB->setMinimumSize(QSize(120, 0));
        pushButton_ExporttoUSB->setMaximumSize(QSize(120, 35));

        horizontalLayout->addWidget(pushButton_ExporttoUSB);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_DeleteDir_HDD = new QPushButton(groupBox_HDD_USB);
        pushButton_DeleteDir_HDD->setObjectName(QString::fromUtf8("pushButton_DeleteDir_HDD"));
        pushButton_DeleteDir_HDD->setMinimumSize(QSize(120, 0));
        pushButton_DeleteDir_HDD->setMaximumSize(QSize(120, 35));

        horizontalLayout->addWidget(pushButton_DeleteDir_HDD);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_6->addLayout(verticalLayout);

        line = new QFrame(groupBox_HDD_USB);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_6->addWidget(line);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_USB = new QLabel(groupBox_HDD_USB);
        label_USB->setObjectName(QString::fromUtf8("label_USB"));
        QFont font2;
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setWeight(75);
        label_USB->setFont(font2);
        label_USB->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/usbdisk.png")));

        horizontalLayout_2->addWidget(label_USB);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        groupBox_2 = new QGroupBox(groupBox_HDD_USB);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_8 = new QHBoxLayout(groupBox_2);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setMargin(11);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_7->addWidget(label_7);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        verticalLayout_7->addWidget(label_8);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout_7->addWidget(label_9);


        horizontalLayout_8->addLayout(verticalLayout_7);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout_8->addWidget(label_10);

        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        verticalLayout_8->addWidget(label_11);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        verticalLayout_8->addWidget(label_12);


        horizontalLayout_8->addLayout(verticalLayout_8);


        horizontalLayout_2->addWidget(groupBox_2);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        treeWidget_USB_info = new QTreeWidget(groupBox_HDD_USB);
        treeWidget_USB_info->setObjectName(QString::fromUtf8("treeWidget_USB_info"));
        treeWidget_USB_info->setFont(font);
        treeWidget_USB_info->setRootIsDecorated(false);

        verticalLayout_2->addWidget(treeWidget_USB_info);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_10 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        pushButton_Refush_Disk = new QPushButton(groupBox_HDD_USB);
        pushButton_Refush_Disk->setObjectName(QString::fromUtf8("pushButton_Refush_Disk"));
        pushButton_Refush_Disk->setMinimumSize(QSize(120, 0));
        pushButton_Refush_Disk->setMaximumSize(QSize(120, 35));

        horizontalLayout_4->addWidget(pushButton_Refush_Disk);

        horizontalSpacer_8 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        pushButton_USB_UnMount = new QPushButton(groupBox_HDD_USB);
        pushButton_USB_UnMount->setObjectName(QString::fromUtf8("pushButton_USB_UnMount"));
        pushButton_USB_UnMount->setMinimumSize(QSize(120, 0));
        pushButton_USB_UnMount->setMaximumSize(QSize(120, 35));

        horizontalLayout_4->addWidget(pushButton_USB_UnMount);

        horizontalSpacer_9 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);

        pushButton_DeleteDir_USB = new QPushButton(groupBox_HDD_USB);
        pushButton_DeleteDir_USB->setObjectName(QString::fromUtf8("pushButton_DeleteDir_USB"));
        pushButton_DeleteDir_USB->setMinimumSize(QSize(120, 0));
        pushButton_DeleteDir_USB->setMaximumSize(QSize(120, 35));

        horizontalLayout_4->addWidget(pushButton_DeleteDir_USB);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_11);


        verticalLayout_2->addLayout(horizontalLayout_4);


        horizontalLayout_6->addLayout(verticalLayout_2);


        verticalLayout_4->addLayout(horizontalLayout_6);

        progressBar_CopyDir = new QProgressBar(groupBox_HDD_USB);
        progressBar_CopyDir->setObjectName(QString::fromUtf8("progressBar_CopyDir"));
        progressBar_CopyDir->setValue(0);

        verticalLayout_4->addWidget(progressBar_CopyDir);

        label_Progress = new QLabel(groupBox_HDD_USB);
        label_Progress->setObjectName(QString::fromUtf8("label_Progress"));

        verticalLayout_4->addWidget(label_Progress);


        verticalLayout_3->addWidget(groupBox_HDD_USB);


        retranslateUi(ContentClass);

        QMetaObject::connectSlotsByName(ContentClass);
    } // setupUi

    void retranslateUi(QWidget *ContentClass)
    {
        ContentClass->setWindowTitle(QApplication::translate("ContentClass", "Content", 0, QApplication::UnicodeUTF8));
        groupBox_HDD_USB->setTitle(QString());
        label_HDD->setText(QString());
        groupBox->setTitle(QApplication::translate("ContentClass", "Disk Space(MB)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ContentClass", "Total:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ContentClass", "Avalible:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ContentClass", "Used:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ContentClass", "0", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ContentClass", "0", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ContentClass", "0", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget_HDD_info->headerItem();
        ___qtreewidgetitem->setText(7, QApplication::translate("ContentClass", "TotalSegment", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(6, QApplication::translate("ContentClass", "RecvSegment", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(5, QApplication::translate("ContentClass", "FilmLength", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(4, QApplication::translate("ContentClass", "2D/3D", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(3, QApplication::translate("ContentClass", "RecvTime", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(2, QApplication::translate("ContentClass", "FilmName", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("ContentClass", "Progress", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("ContentClass", "Status", 0, QApplication::UnicodeUTF8));
        pushButton_ExporttoUSB->setText(QApplication::translate("ContentClass", "Export", 0, QApplication::UnicodeUTF8));
        pushButton_DeleteDir_HDD->setText(QApplication::translate("ContentClass", "Delete", 0, QApplication::UnicodeUTF8));
        label_USB->setText(QString());
        groupBox_2->setTitle(QApplication::translate("ContentClass", "Disk Space(MB)", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ContentClass", "Total:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("ContentClass", "Avalible:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("ContentClass", "Used:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("ContentClass", "0", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("ContentClass", "0", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("ContentClass", "0", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget_USB_info->headerItem();
        ___qtreewidgetitem1->setText(1, QApplication::translate("ContentClass", "FilmLength", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("ContentClass", "FilmName", 0, QApplication::UnicodeUTF8));
        pushButton_Refush_Disk->setText(QApplication::translate("ContentClass", "Refush Disk", 0, QApplication::UnicodeUTF8));
        pushButton_USB_UnMount->setText(QApplication::translate("ContentClass", "Unmount", 0, QApplication::UnicodeUTF8));
        pushButton_DeleteDir_USB->setText(QApplication::translate("ContentClass", "Delete", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ContentClass);
    } // retranslateUi

};

namespace Ui {
    class ContentClass: public Ui_ContentClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTENT_H
