/********************************************************************************
** Form generated from reading ui file 'status.ui'
**
** Created: Fri Nov 6 10:28:21 2015
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STATUS_H
#define UI_STATUS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatusClass
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_satellite;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_4;
    QLabel *agc;
    QLabel *label_2;
    QLabel *label_3;
    QVBoxLayout *verticalLayout_5;
    QProgressBar *progressBar_siganl_Strength;
    QProgressBar *progressBar_siganl_Quality;
    QLabel *label_4;
    QGroupBox *groupBox_Filme;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_filmName;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_Receiver;
    QHBoxLayout *horizontalLayout_10;
    QProgressBar *progressBar_Revceiver_length;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_9;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_10;
    QTextBrowser *textBrowser;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *StatusClass)
    {
        if (StatusClass->objectName().isEmpty())
            StatusClass->setObjectName(QString::fromUtf8("StatusClass"));
        StatusClass->resize(860, 492);
        QFont font;
        font.setFamily(QString::fromUtf8("Book Antiqua"));
        font.setPointSize(18);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        StatusClass->setFont(font);
        verticalLayout_3 = new QVBoxLayout(StatusClass);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setMargin(11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_satellite = new QGroupBox(StatusClass);
        groupBox_satellite->setObjectName(QString::fromUtf8("groupBox_satellite"));
        groupBox_satellite->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox_satellite);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        agc = new QLabel(groupBox_satellite);
        agc->setObjectName(QString::fromUtf8("agc"));
        agc->setMinimumSize(QSize(175, 0));
        agc->setMaximumSize(QSize(175, 16777215));

        verticalLayout_4->addWidget(agc);

        label_2 = new QLabel(groupBox_satellite);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(175, 0));
        label_2->setMaximumSize(QSize(175, 16777215));

        verticalLayout_4->addWidget(label_2);

        label_3 = new QLabel(groupBox_satellite);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(175, 0));
        label_3->setMaximumSize(QSize(175, 16777215));

        verticalLayout_4->addWidget(label_3);


        horizontalLayout_6->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        progressBar_siganl_Strength = new QProgressBar(groupBox_satellite);
        progressBar_siganl_Strength->setObjectName(QString::fromUtf8("progressBar_siganl_Strength"));
        progressBar_siganl_Strength->setValue(0);
        progressBar_siganl_Strength->setTextDirection(QProgressBar::BottomToTop);

        verticalLayout_5->addWidget(progressBar_siganl_Strength);

        progressBar_siganl_Quality = new QProgressBar(groupBox_satellite);
        progressBar_siganl_Quality->setObjectName(QString::fromUtf8("progressBar_siganl_Quality"));
        progressBar_siganl_Quality->setValue(0);
        progressBar_siganl_Quality->setTextDirection(QProgressBar::BottomToTop);

        verticalLayout_5->addWidget(progressBar_siganl_Quality);

        label_4 = new QLabel(groupBox_satellite);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(175, 0));
        label_4->setMaximumSize(QSize(175, 16777215));

        verticalLayout_5->addWidget(label_4);


        horizontalLayout_6->addLayout(verticalLayout_5);


        verticalLayout->addLayout(horizontalLayout_6);


        verticalLayout_3->addWidget(groupBox_satellite);

        groupBox_Filme = new QGroupBox(StatusClass);
        groupBox_Filme->setObjectName(QString::fromUtf8("groupBox_Filme"));
        groupBox_Filme->setFont(font);
        verticalLayout_2 = new QVBoxLayout(groupBox_Filme);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_5 = new QLabel(groupBox_Filme);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(175, 0));
        label_5->setMaximumSize(QSize(175, 16777215));

        horizontalLayout->addWidget(label_5);


        verticalLayout_6->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_6 = new QLabel(groupBox_Filme);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(175, 0));
        label_6->setMaximumSize(QSize(175, 16777215));

        horizontalLayout_3->addWidget(label_6);


        verticalLayout_6->addLayout(horizontalLayout_3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_8 = new QLabel(groupBox_Filme);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(175, 0));
        label_8->setMaximumSize(QSize(175, 16777215));

        horizontalLayout_7->addWidget(label_8);


        verticalLayout_6->addLayout(horizontalLayout_7);


        horizontalLayout_2->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_filmName = new QLabel(groupBox_Filme);
        label_filmName->setObjectName(QString::fromUtf8("label_filmName"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_filmName->sizePolicy().hasHeightForWidth());
        label_filmName->setSizePolicy(sizePolicy);

        horizontalLayout_8->addWidget(label_filmName);


        verticalLayout_7->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_Receiver = new QLabel(groupBox_Filme);
        label_Receiver->setObjectName(QString::fromUtf8("label_Receiver"));

        horizontalLayout_9->addWidget(label_Receiver);


        verticalLayout_7->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        progressBar_Revceiver_length = new QProgressBar(groupBox_Filme);
        progressBar_Revceiver_length->setObjectName(QString::fromUtf8("progressBar_Revceiver_length"));
        progressBar_Revceiver_length->setValue(0);

        horizontalLayout_10->addWidget(progressBar_Revceiver_length);


        verticalLayout_7->addLayout(horizontalLayout_10);


        horizontalLayout_2->addLayout(verticalLayout_7);


        verticalLayout_2->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        verticalLayout_2->addLayout(gridLayout);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_10);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        label_9 = new QLabel(groupBox_Filme);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(175, 0));
        label_9->setMaximumSize(QSize(175, 16777215));

        verticalLayout_9->addWidget(label_9);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer);


        horizontalLayout_5->addLayout(verticalLayout_9);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        textBrowser = new QTextBrowser(groupBox_Filme);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout_10->addWidget(textBrowser);


        horizontalLayout_5->addLayout(verticalLayout_10);


        verticalLayout_2->addLayout(horizontalLayout_5);


        verticalLayout_3->addWidget(groupBox_Filme);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        retranslateUi(StatusClass);

        QMetaObject::connectSlotsByName(StatusClass);
    } // setupUi

    void retranslateUi(QWidget *StatusClass)
    {
        StatusClass->setWindowTitle(QApplication::translate("StatusClass", "Status", 0, QApplication::UnicodeUTF8));
        groupBox_satellite->setTitle(QApplication::translate("StatusClass", "Satellite", 0, QApplication::UnicodeUTF8));
        agc->setText(QApplication::translate("StatusClass", "AGC:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StatusClass", "SNR:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("StatusClass", "Lock:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("StatusClass", "No", 0, QApplication::UnicodeUTF8));
        groupBox_Filme->setTitle(QApplication::translate("StatusClass", "Receive", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("StatusClass", "Film Name:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("StatusClass", "Received/Total:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("StatusClass", "Received Ratio:", 0, QApplication::UnicodeUTF8));
        label_filmName->setText(QApplication::translate("StatusClass", "N/A", 0, QApplication::UnicodeUTF8));
        label_Receiver->setText(QApplication::translate("StatusClass", "0/0", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("StatusClass", "Status:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(StatusClass);
    } // retranslateUi

};

namespace Ui {
    class StatusClass: public Ui_StatusClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATUS_H
