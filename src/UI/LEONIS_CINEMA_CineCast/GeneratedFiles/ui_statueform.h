/********************************************************************************
** Form generated from reading UI file 'statueform.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATUEFORM_H
#define UI_STATUEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatueForm
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_satellite;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label;
    QProgressBar *progressBar_siganl_Strength;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_2;
    QProgressBar *progressBar_siganl_Quality;
    QHBoxLayout *horizontalLayout_16;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_17;
    QGroupBox *groupBox_Filme;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_12;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLabel *label_filmName;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *horizontalSpacer_13;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QLabel *label_Receiver;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_3;
    QProgressBar *progressBar_Revceiver_length;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *horizontalSpacer_15;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_7;
    QTextBrowser *textBrowser;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *StatueForm)
    {
        if (StatueForm->objectName().isEmpty())
            StatueForm->setObjectName(QStringLiteral("StatueForm"));
        StatueForm->resize(752, 714);
        verticalLayout_3 = new QVBoxLayout(StatueForm);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox_satellite = new QGroupBox(StatueForm);
        groupBox_satellite->setObjectName(QStringLiteral("groupBox_satellite"));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(13);
        font.setStyleStrategy(QFont::PreferAntialias);
        groupBox_satellite->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox_satellite);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label = new QLabel(groupBox_satellite);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_11->addWidget(label);

        progressBar_siganl_Strength = new QProgressBar(groupBox_satellite);
        progressBar_siganl_Strength->setObjectName(QStringLiteral("progressBar_siganl_Strength"));
        progressBar_siganl_Strength->setValue(24);

        horizontalLayout_11->addWidget(progressBar_siganl_Strength);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_2 = new QLabel(groupBox_satellite);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_12->addWidget(label_2);

        progressBar_siganl_Quality = new QProgressBar(groupBox_satellite);
        progressBar_siganl_Quality->setObjectName(QStringLiteral("progressBar_siganl_Quality"));
        progressBar_siganl_Quality->setValue(24);
        progressBar_siganl_Quality->setTextDirection(QProgressBar::BottomToTop);

        horizontalLayout_12->addWidget(progressBar_siganl_Quality);


        verticalLayout->addLayout(horizontalLayout_12);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_11);


        verticalLayout->addLayout(horizontalLayout_16);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addWidget(groupBox_satellite);

        horizontalSpacer_17 = new QSpacerItem(726, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_3->addItem(horizontalSpacer_17);

        groupBox_Filme = new QGroupBox(StatueForm);
        groupBox_Filme->setObjectName(QStringLiteral("groupBox_Filme"));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(13);
        font1.setBold(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        font1.setStrikeOut(false);
        font1.setKerning(true);
        font1.setStyleStrategy(QFont::PreferAntialias);
        groupBox_Filme->setFont(font1);
        verticalLayout_2 = new QVBoxLayout(groupBox_Filme);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_8);


        verticalLayout_2->addLayout(horizontalLayout_13);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_12);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(groupBox_Filme);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        label_filmName = new QLabel(groupBox_Filme);
        label_filmName->setObjectName(QStringLiteral("label_filmName"));

        horizontalLayout_5->addWidget(label_filmName);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_9);


        verticalLayout_2->addLayout(horizontalLayout_14);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_13);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_6 = new QLabel(groupBox_Filme);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_3->addWidget(label_6);

        label_Receiver = new QLabel(groupBox_Filme);
        label_Receiver->setObjectName(QStringLiteral("label_Receiver"));

        horizontalLayout_3->addWidget(label_Receiver);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_3 = new QLabel(groupBox_Filme);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_15->addWidget(label_3);

        progressBar_Revceiver_length = new QProgressBar(groupBox_Filme);
        progressBar_Revceiver_length->setObjectName(QStringLiteral("progressBar_Revceiver_length"));
        progressBar_Revceiver_length->setValue(24);

        horizontalLayout_15->addWidget(progressBar_Revceiver_length);


        verticalLayout_2->addLayout(horizontalLayout_15);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_9 = new QLabel(groupBox_Filme);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout->addWidget(label_9);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_10);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer_15);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_7 = new QLabel(groupBox_Filme);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_4->addWidget(label_7);

        textBrowser = new QTextBrowser(groupBox_Filme);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        horizontalLayout_4->addWidget(textBrowser);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer = new QSpacerItem(20, 173, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout_3->addWidget(groupBox_Filme);


        retranslateUi(StatueForm);

        QMetaObject::connectSlotsByName(StatueForm);
    } // setupUi

    void retranslateUi(QWidget *StatueForm)
    {
        StatueForm->setWindowTitle(QApplication::translate("StatueForm", "Form", 0));
        groupBox_satellite->setTitle(QApplication::translate("StatueForm", "\345\215\253\346\230\237", 0));
        label->setText(QApplication::translate("StatueForm", "\344\277\241\345\217\267\345\274\272\345\272\246\357\274\232 ", 0));
        label_2->setText(QApplication::translate("StatueForm", "\344\277\241\345\217\267\350\264\250\351\207\217\357\274\232 ", 0));
        groupBox_Filme->setTitle(QApplication::translate("StatueForm", "\345\275\261\347\211\207\346\216\245\346\224\266", 0));
        label_5->setText(QApplication::translate("StatueForm", "\345\275\261\347\211\207\357\274\232", 0));
        label_filmName->setText(QApplication::translate("StatueForm", "Leonis Cinema                                                                                                            ", 0));
        label_6->setText(QApplication::translate("StatueForm", "\345\267\262\346\216\245\346\224\266/\347\211\207\351\225\277\357\274\232", 0));
        label_Receiver->setText(QApplication::translate("StatueForm", "x/1000000                                                    ", 0));
        label_3->setText(QApplication::translate("StatueForm", " .                ", 0));
        label_9->setText(QApplication::translate("StatueForm", "\346\216\245\346\224\266\347\212\266\346\200\201\357\274\232", 0));
        label_7->setText(QApplication::translate("StatueForm", ".               .", 0));
    } // retranslateUi

};

namespace Ui {
    class StatueForm: public Ui_StatueForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATUEFORM_H
