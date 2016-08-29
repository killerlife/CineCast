/********************************************************************************
** Form generated from reading ui file 'live.ui'
**
** Created: Tue Aug 16 10:38:28 2016
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LIVE_H
#define UI_LIVE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LiveClass
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton;
    QLabel *label_2;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_5;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_4;
    QLabel *label;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QSpacerItem *horizontalSpacer_8;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_3;
    QRadioButton *radioButton_6;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_5;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_6;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_5;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_7;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_6;
    QRadioButton *radioButton_10;
    QRadioButton *radioButton_11;
    QRadioButton *radioButton_9;
    QRadioButton *radioButton_7;
    QRadioButton *radioButton_8;
    QSpacerItem *horizontalSpacer_9;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *lineEdit;
    QLabel *label_8;
    QLineEdit *lineEdit_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *LiveClass)
    {
        if (LiveClass->objectName().isEmpty())
            LiveClass->setObjectName(QString::fromUtf8("LiveClass"));
        LiveClass->resize(1003, 419);
        horizontalLayout_2 = new QHBoxLayout(LiveClass);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setMargin(11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_2 = new QGroupBox(LiveClass);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMaximumSize(QSize(300, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("Book Antiqua"));
        font.setPointSize(16);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        groupBox_2->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_3->addWidget(pushButton);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_3->addWidget(pushButton_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_3);

        treeWidget = new QTreeWidget(groupBox_2);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        verticalLayout->addWidget(treeWidget);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        pushButton_4 = new QPushButton(groupBox_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/play.png")), QIcon::Normal, QIcon::Off);
        pushButton_4->setIcon(icon);
        pushButton_4->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(pushButton_4);

        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        pushButton_3 = new QPushButton(groupBox_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        QIcon icon1;
        icon1.addPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/stop.png")), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon1);
        pushButton_3->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(pushButton_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_2->addWidget(groupBox_2);

        groupBox = new QGroupBox(LiveClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout->addWidget(radioButton);

        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout->addWidget(radioButton_2);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_8);


        verticalLayout_2->addLayout(horizontalLayout);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setMargin(11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        widget_2 = new QWidget(groupBox_3);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_12 = new QHBoxLayout(widget_2);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setMargin(11);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(147, 0));

        horizontalLayout_12->addWidget(label_3);

        radioButton_6 = new QRadioButton(widget_2);
        radioButton_6->setObjectName(QString::fromUtf8("radioButton_6"));
        radioButton_6->setMinimumSize(QSize(105, 0));

        horizontalLayout_12->addWidget(radioButton_6);

        radioButton_4 = new QRadioButton(widget_2);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));
        radioButton_4->setMinimumSize(QSize(105, 0));

        horizontalLayout_12->addWidget(radioButton_4);

        radioButton_3 = new QRadioButton(widget_2);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setMinimumSize(QSize(105, 0));

        horizontalLayout_12->addWidget(radioButton_3);

        radioButton_5 = new QRadioButton(widget_2);
        radioButton_5->setObjectName(QString::fromUtf8("radioButton_5"));
        radioButton_5->setMinimumSize(QSize(105, 0));

        horizontalLayout_12->addWidget(radioButton_5);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_7);


        horizontalLayout_5->addWidget(widget_2);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        widget_3 = new QWidget(groupBox_3);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_9 = new QHBoxLayout(widget_3);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setMargin(11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_5 = new QLabel(widget_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(147, 0));

        horizontalLayout_9->addWidget(label_5);

        checkBox_2 = new QCheckBox(widget_3);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setMinimumSize(QSize(105, 0));

        horizontalLayout_9->addWidget(checkBox_2);

        checkBox = new QCheckBox(widget_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setMinimumSize(QSize(105, 0));

        horizontalLayout_9->addWidget(checkBox);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_6);


        horizontalLayout_6->addWidget(widget_3);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        widget = new QWidget(groupBox_3);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_11 = new QHBoxLayout(widget);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setMargin(11);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(147, 0));

        horizontalLayout_11->addWidget(label_6);

        radioButton_10 = new QRadioButton(widget);
        radioButton_10->setObjectName(QString::fromUtf8("radioButton_10"));
        radioButton_10->setMinimumSize(QSize(105, 0));

        horizontalLayout_11->addWidget(radioButton_10);

        radioButton_11 = new QRadioButton(widget);
        radioButton_11->setObjectName(QString::fromUtf8("radioButton_11"));
        radioButton_11->setMinimumSize(QSize(105, 0));

        horizontalLayout_11->addWidget(radioButton_11);

        radioButton_9 = new QRadioButton(widget);
        radioButton_9->setObjectName(QString::fromUtf8("radioButton_9"));
        radioButton_9->setMinimumSize(QSize(105, 0));

        horizontalLayout_11->addWidget(radioButton_9);

        radioButton_7 = new QRadioButton(widget);
        radioButton_7->setObjectName(QString::fromUtf8("radioButton_7"));
        radioButton_7->setMinimumSize(QSize(105, 0));

        horizontalLayout_11->addWidget(radioButton_7);

        radioButton_8 = new QRadioButton(widget);
        radioButton_8->setObjectName(QString::fromUtf8("radioButton_8"));
        radioButton_8->setMinimumSize(QSize(105, 0));

        horizontalLayout_11->addWidget(radioButton_8);


        horizontalLayout_7->addWidget(widget);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_9);


        verticalLayout_3->addLayout(horizontalLayout_7);


        verticalLayout_2->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setMargin(11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        lineEdit = new QLineEdit(groupBox_4);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_8->addWidget(lineEdit);

        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_8->addWidget(label_8);

        lineEdit_2 = new QLineEdit(groupBox_4);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout_8->addWidget(lineEdit_2);


        verticalLayout_4->addLayout(horizontalLayout_8);


        verticalLayout_2->addWidget(groupBox_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_2->addWidget(groupBox);


        retranslateUi(LiveClass);

        QMetaObject::connectSlotsByName(LiveClass);
    } // setupUi

    void retranslateUi(QWidget *LiveClass)
    {
        LiveClass->setWindowTitle(QApplication::translate("LiveClass", "Live", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("LiveClass", "Scan", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
        pushButton_2->setText(QApplication::translate("LiveClass", "Load", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("LiveClass", "Channel List", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_4->setText(QApplication::translate("LiveClass", "Live Mode Select:", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("LiveClass", "Direct Decode", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("LiveClass", "IP Broadcast", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("LiveClass", "Direct Decode", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LiveClass", "Video Decoder: ", 0, QApplication::UnicodeUTF8));
        radioButton_6->setText(QApplication::translate("LiveClass", "Auto", 0, QApplication::UnicodeUTF8));
        radioButton_4->setText(QApplication::translate("LiveClass", "MPEG2", 0, QApplication::UnicodeUTF8));
        radioButton_3->setText(QApplication::translate("LiveClass", "H.264", 0, QApplication::UnicodeUTF8));
        radioButton_5->setText(QApplication::translate("LiveClass", "H.265", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("LiveClass", "Audio Output:", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("LiveClass", "SPDIF", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("LiveClass", "Analog", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("LiveClass", "Audio Decoder:", 0, QApplication::UnicodeUTF8));
        radioButton_10->setText(QApplication::translate("LiveClass", "Auto", 0, QApplication::UnicodeUTF8));
        radioButton_11->setText(QApplication::translate("LiveClass", "MP3", 0, QApplication::UnicodeUTF8));
        radioButton_9->setText(QApplication::translate("LiveClass", "PCM", 0, QApplication::UnicodeUTF8));
        radioButton_7->setText(QApplication::translate("LiveClass", "DTS", 0, QApplication::UnicodeUTF8));
        radioButton_8->setText(QApplication::translate("LiveClass", "AC3", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("LiveClass", "IP Broadcast", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("LiveClass", "Boardcast IP:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("LiveClass", "Port:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(LiveClass);
    } // retranslateUi

};

namespace Ui {
    class LiveClass: public Ui_LiveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIVE_H
