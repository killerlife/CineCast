/********************************************************************************
** Form generated from reading ui file 'setup.ui'
**
** Created: Tue Nov 10 11:48:32 2015
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetupClass
{
public:
    QVBoxLayout *verticalLayout_17;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pushButton_2;
    QLabel *labe_symbl_rate_10;
    QPushButton *pushButton;
    QGroupBox *groupBox_staeliite;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_14;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_frequency;
    QLabel *labe_symbl_rate;
    QLabel *label_frequency_2;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_Frequency;
    QLineEdit *lineEdit_symbol_rate;
    QComboBox *comboBox_DVB;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_6;
    QLabel *labe_symbl_rate_4;
    QLabel *labe_symbl_rate_5;
    QLabel *labe_symbl_rate_8;
    QVBoxLayout *verticalLayout_3;
    QComboBox *comboBox_xy;
    QComboBox *comboBox_V;
    QComboBox *comboBox_SPK;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_10;
    QLabel *labe_symbl_rate_6;
    QLabel *labe_symbl_rate_9;
    QVBoxLayout *verticalLayout_12;
    QComboBox *comboBox_decimals;
    QLabel *labe_symbl_rate_12;
    QLabel *labe_symbl_rate_11;
    QGroupBox *groupBox_Network;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_20;
    QComboBox *comboBox_ECHs;
    QVBoxLayout *verticalLayout_21;
    QCheckBox *checkBox_DHCP;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_ip;
    QVBoxLayout *verticalLayout_9;
    QLineEdit *lineEdit_Ip;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_Netm_mask;
    QVBoxLayout *verticalLayout_14;
    QLineEdit *lineEdit_Net_AsK;
    QSpacerItem *horizontalSpacer_6;
    QVBoxLayout *verticalLayout_15;
    QLabel *label_Gateway;
    QVBoxLayout *verticalLayout_16;
    QLineEdit *lineEdit_GateWay;
    QGroupBox *groupBox_Network_2;
    QVBoxLayout *verticalLayout_22;
    QHBoxLayout *horizontalLayout_17;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_18;
    QLabel *label_DNS;
    QVBoxLayout *verticalLayout_19;
    QLineEdit *lineEdit_DNS;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_DNS_2;
    QVBoxLayout *verticalLayout_23;
    QLineEdit *lineEdit_DNS_2;
    QSpacerItem *horizontalSpacer_8;
    QVBoxLayout *verticalLayout_26;
    QLabel *label_Gateway_2;
    QVBoxLayout *verticalLayout_27;
    QLineEdit *lineEdit_Remote_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *SetupClass)
    {
        if (SetupClass->objectName().isEmpty())
            SetupClass->setObjectName(QString::fromUtf8("SetupClass"));
        SetupClass->resize(1111, 487);
        verticalLayout_17 = new QVBoxLayout(SetupClass);
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setMargin(11);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        pushButton_2 = new QPushButton(SetupClass);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        QFont font;
        font.setFamily(QString::fromUtf8("Book Antiqua"));
        font.setPointSize(18);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        pushButton_2->setFont(font);

        horizontalLayout_2->addWidget(pushButton_2);

        labe_symbl_rate_10 = new QLabel(SetupClass);
        labe_symbl_rate_10->setObjectName(QString::fromUtf8("labe_symbl_rate_10"));
        labe_symbl_rate_10->setFont(font);

        horizontalLayout_2->addWidget(labe_symbl_rate_10);

        pushButton = new QPushButton(SetupClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(90, 0));
        pushButton->setFont(font);

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout_17->addLayout(horizontalLayout_2);

        groupBox_staeliite = new QGroupBox(SetupClass);
        groupBox_staeliite->setObjectName(QString::fromUtf8("groupBox_staeliite"));
        groupBox_staeliite->setFont(font);
        verticalLayout_11 = new QVBoxLayout(groupBox_staeliite);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setMargin(11);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_frequency = new QLabel(groupBox_staeliite);
        label_frequency->setObjectName(QString::fromUtf8("label_frequency"));
        label_frequency->setFont(font);

        verticalLayout_5->addWidget(label_frequency);

        labe_symbl_rate = new QLabel(groupBox_staeliite);
        labe_symbl_rate->setObjectName(QString::fromUtf8("labe_symbl_rate"));
        labe_symbl_rate->setFont(font);

        verticalLayout_5->addWidget(labe_symbl_rate);

        label_frequency_2 = new QLabel(groupBox_staeliite);
        label_frequency_2->setObjectName(QString::fromUtf8("label_frequency_2"));
        label_frequency_2->setFont(font);

        verticalLayout_5->addWidget(label_frequency_2);


        horizontalLayout_14->addLayout(verticalLayout_5);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lineEdit_Frequency = new QLineEdit(groupBox_staeliite);
        lineEdit_Frequency->setObjectName(QString::fromUtf8("lineEdit_Frequency"));
        lineEdit_Frequency->setMinimumSize(QSize(180, 0));
        lineEdit_Frequency->setMaximumSize(QSize(200, 16777215));

        verticalLayout->addWidget(lineEdit_Frequency);

        lineEdit_symbol_rate = new QLineEdit(groupBox_staeliite);
        lineEdit_symbol_rate->setObjectName(QString::fromUtf8("lineEdit_symbol_rate"));
        lineEdit_symbol_rate->setMinimumSize(QSize(200, 0));
        lineEdit_symbol_rate->setMaximumSize(QSize(180, 16777215));

        verticalLayout->addWidget(lineEdit_symbol_rate);

        comboBox_DVB = new QComboBox(groupBox_staeliite);
        comboBox_DVB->setObjectName(QString::fromUtf8("comboBox_DVB"));
        comboBox_DVB->setMinimumSize(QSize(180, 0));

        verticalLayout->addWidget(comboBox_DVB);


        horizontalLayout_14->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        labe_symbl_rate_4 = new QLabel(groupBox_staeliite);
        labe_symbl_rate_4->setObjectName(QString::fromUtf8("labe_symbl_rate_4"));
        labe_symbl_rate_4->setFont(font);

        verticalLayout_6->addWidget(labe_symbl_rate_4);

        labe_symbl_rate_5 = new QLabel(groupBox_staeliite);
        labe_symbl_rate_5->setObjectName(QString::fromUtf8("labe_symbl_rate_5"));
        labe_symbl_rate_5->setFont(font);

        verticalLayout_6->addWidget(labe_symbl_rate_5);

        labe_symbl_rate_8 = new QLabel(groupBox_staeliite);
        labe_symbl_rate_8->setObjectName(QString::fromUtf8("labe_symbl_rate_8"));
        labe_symbl_rate_8->setFont(font);

        verticalLayout_6->addWidget(labe_symbl_rate_8);


        horizontalLayout_14->addLayout(verticalLayout_6);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        comboBox_xy = new QComboBox(groupBox_staeliite);
        comboBox_xy->setObjectName(QString::fromUtf8("comboBox_xy"));
        comboBox_xy->setMinimumSize(QSize(180, 0));

        verticalLayout_3->addWidget(comboBox_xy);

        comboBox_V = new QComboBox(groupBox_staeliite);
        comboBox_V->setObjectName(QString::fromUtf8("comboBox_V"));
        comboBox_V->setMinimumSize(QSize(180, 0));
        comboBox_V->setMaximumSize(QSize(180, 16777215));

        verticalLayout_3->addWidget(comboBox_V);

        comboBox_SPK = new QComboBox(groupBox_staeliite);
        comboBox_SPK->setObjectName(QString::fromUtf8("comboBox_SPK"));
        comboBox_SPK->setMinimumSize(QSize(180, 0));

        verticalLayout_3->addWidget(comboBox_SPK);


        horizontalLayout_14->addLayout(verticalLayout_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_2);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        labe_symbl_rate_6 = new QLabel(groupBox_staeliite);
        labe_symbl_rate_6->setObjectName(QString::fromUtf8("labe_symbl_rate_6"));
        labe_symbl_rate_6->setFont(font);

        verticalLayout_10->addWidget(labe_symbl_rate_6);

        labe_symbl_rate_9 = new QLabel(groupBox_staeliite);
        labe_symbl_rate_9->setObjectName(QString::fromUtf8("labe_symbl_rate_9"));
        labe_symbl_rate_9->setFont(font);

        verticalLayout_10->addWidget(labe_symbl_rate_9);


        horizontalLayout_14->addLayout(verticalLayout_10);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        comboBox_decimals = new QComboBox(groupBox_staeliite);
        comboBox_decimals->setObjectName(QString::fromUtf8("comboBox_decimals"));
        comboBox_decimals->setMinimumSize(QSize(180, 0));

        verticalLayout_12->addWidget(comboBox_decimals);

        labe_symbl_rate_12 = new QLabel(groupBox_staeliite);
        labe_symbl_rate_12->setObjectName(QString::fromUtf8("labe_symbl_rate_12"));
        labe_symbl_rate_12->setFont(font);

        verticalLayout_12->addWidget(labe_symbl_rate_12);

        labe_symbl_rate_11 = new QLabel(groupBox_staeliite);
        labe_symbl_rate_11->setObjectName(QString::fromUtf8("labe_symbl_rate_11"));
        labe_symbl_rate_11->setFont(font);

        verticalLayout_12->addWidget(labe_symbl_rate_11);


        horizontalLayout_14->addLayout(verticalLayout_12);


        verticalLayout_11->addLayout(horizontalLayout_14);


        verticalLayout_17->addWidget(groupBox_staeliite);

        groupBox_Network = new QGroupBox(SetupClass);
        groupBox_Network->setObjectName(QString::fromUtf8("groupBox_Network"));
        groupBox_Network->setFont(font);
        verticalLayout_2 = new QVBoxLayout(groupBox_Network);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        comboBox_ECHs = new QComboBox(groupBox_Network);
        comboBox_ECHs->setObjectName(QString::fromUtf8("comboBox_ECHs"));
        comboBox_ECHs->setMaximumSize(QSize(120, 16777215));

        verticalLayout_20->addWidget(comboBox_ECHs);


        horizontalLayout_3->addLayout(verticalLayout_20);

        verticalLayout_21 = new QVBoxLayout();
        verticalLayout_21->setSpacing(6);
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        checkBox_DHCP = new QCheckBox(groupBox_Network);
        checkBox_DHCP->setObjectName(QString::fromUtf8("checkBox_DHCP"));
        checkBox_DHCP->setMaximumSize(QSize(120, 16777215));

        verticalLayout_21->addWidget(checkBox_DHCP);


        horizontalLayout_3->addLayout(verticalLayout_21);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_ip = new QLabel(groupBox_Network);
        label_ip->setObjectName(QString::fromUtf8("label_ip"));

        verticalLayout_8->addWidget(label_ip);


        horizontalLayout_3->addLayout(verticalLayout_8);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        lineEdit_Ip = new QLineEdit(groupBox_Network);
        lineEdit_Ip->setObjectName(QString::fromUtf8("lineEdit_Ip"));

        verticalLayout_9->addWidget(lineEdit_Ip);


        horizontalLayout_3->addLayout(verticalLayout_9);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        label_Netm_mask = new QLabel(groupBox_Network);
        label_Netm_mask->setObjectName(QString::fromUtf8("label_Netm_mask"));

        verticalLayout_13->addWidget(label_Netm_mask);


        horizontalLayout_3->addLayout(verticalLayout_13);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        lineEdit_Net_AsK = new QLineEdit(groupBox_Network);
        lineEdit_Net_AsK->setObjectName(QString::fromUtf8("lineEdit_Net_AsK"));

        verticalLayout_14->addWidget(lineEdit_Net_AsK);


        horizontalLayout_3->addLayout(verticalLayout_14);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        label_Gateway = new QLabel(groupBox_Network);
        label_Gateway->setObjectName(QString::fromUtf8("label_Gateway"));

        verticalLayout_15->addWidget(label_Gateway);


        horizontalLayout_3->addLayout(verticalLayout_15);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        lineEdit_GateWay = new QLineEdit(groupBox_Network);
        lineEdit_GateWay->setObjectName(QString::fromUtf8("lineEdit_GateWay"));

        verticalLayout_16->addWidget(lineEdit_GateWay);


        horizontalLayout_3->addLayout(verticalLayout_16);


        verticalLayout_4->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(verticalLayout_4);


        verticalLayout_17->addWidget(groupBox_Network);

        groupBox_Network_2 = new QGroupBox(SetupClass);
        groupBox_Network_2->setObjectName(QString::fromUtf8("groupBox_Network_2"));
        groupBox_Network_2->setFont(font);
        verticalLayout_22 = new QVBoxLayout(groupBox_Network_2);
        verticalLayout_22->setSpacing(6);
        verticalLayout_22->setMargin(11);
        verticalLayout_22->setObjectName(QString::fromUtf8("verticalLayout_22"));
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        label_DNS = new QLabel(groupBox_Network_2);
        label_DNS->setObjectName(QString::fromUtf8("label_DNS"));

        verticalLayout_18->addWidget(label_DNS);


        horizontalLayout_7->addLayout(verticalLayout_18);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        lineEdit_DNS = new QLineEdit(groupBox_Network_2);
        lineEdit_DNS->setObjectName(QString::fromUtf8("lineEdit_DNS"));

        verticalLayout_19->addWidget(lineEdit_DNS);


        horizontalLayout_7->addLayout(verticalLayout_19);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_DNS_2 = new QLabel(groupBox_Network_2);
        label_DNS_2->setObjectName(QString::fromUtf8("label_DNS_2"));

        verticalLayout_7->addWidget(label_DNS_2);


        horizontalLayout_7->addLayout(verticalLayout_7);

        verticalLayout_23 = new QVBoxLayout();
        verticalLayout_23->setSpacing(6);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        lineEdit_DNS_2 = new QLineEdit(groupBox_Network_2);
        lineEdit_DNS_2->setObjectName(QString::fromUtf8("lineEdit_DNS_2"));

        verticalLayout_23->addWidget(lineEdit_DNS_2);


        horizontalLayout_7->addLayout(verticalLayout_23);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_8);

        verticalLayout_26 = new QVBoxLayout();
        verticalLayout_26->setSpacing(6);
        verticalLayout_26->setObjectName(QString::fromUtf8("verticalLayout_26"));
        label_Gateway_2 = new QLabel(groupBox_Network_2);
        label_Gateway_2->setObjectName(QString::fromUtf8("label_Gateway_2"));

        verticalLayout_26->addWidget(label_Gateway_2);


        horizontalLayout_7->addLayout(verticalLayout_26);

        verticalLayout_27 = new QVBoxLayout();
        verticalLayout_27->setSpacing(6);
        verticalLayout_27->setObjectName(QString::fromUtf8("verticalLayout_27"));
        lineEdit_Remote_2 = new QLineEdit(groupBox_Network_2);
        lineEdit_Remote_2->setObjectName(QString::fromUtf8("lineEdit_Remote_2"));

        verticalLayout_27->addWidget(lineEdit_Remote_2);


        horizontalLayout_7->addLayout(verticalLayout_27);


        horizontalLayout_17->addLayout(horizontalLayout_7);


        verticalLayout_22->addLayout(horizontalLayout_17);


        verticalLayout_17->addWidget(groupBox_Network_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_17->addItem(verticalSpacer);


        retranslateUi(SetupClass);

        QMetaObject::connectSlotsByName(SetupClass);
    } // setupUi

    void retranslateUi(QWidget *SetupClass)
    {
        SetupClass->setWindowTitle(QApplication::translate("SetupClass", "Setup", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("SetupClass", "Default", 0, QApplication::UnicodeUTF8));
        labe_symbl_rate_10->setText(QString());
        pushButton->setText(QApplication::translate("SetupClass", "Save", 0, QApplication::UnicodeUTF8));
        groupBox_staeliite->setTitle(QApplication::translate("SetupClass", "Satellite", 0, QApplication::UnicodeUTF8));
        label_frequency->setText(QApplication::translate("SetupClass", "Frequency:", 0, QApplication::UnicodeUTF8));
        labe_symbl_rate->setText(QApplication::translate("SetupClass", "Symbol Rate:", 0, QApplication::UnicodeUTF8));
        label_frequency_2->setText(QApplication::translate("SetupClass", "Delivery System:", 0, QApplication::UnicodeUTF8));
        comboBox_DVB->clear();
        comboBox_DVB->insertItems(0, QStringList()
         << QApplication::translate("SetupClass", "DVB-S2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "DVB-S", 0, QApplication::UnicodeUTF8)
        );
        labe_symbl_rate_4->setText(QApplication::translate("SetupClass", "FEC:", 0, QApplication::UnicodeUTF8));
        labe_symbl_rate_5->setText(QApplication::translate("SetupClass", "Polarization:", 0, QApplication::UnicodeUTF8));
        labe_symbl_rate_8->setText(QApplication::translate("SetupClass", "Modulation:", 0, QApplication::UnicodeUTF8));
        comboBox_xy->clear();
        comboBox_xy->insertItems(0, QStringList()
         << QApplication::translate("SetupClass", "1/2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "2/3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "3/4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "3/5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "4/5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "5/6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "6/7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "7/8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "8/9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "9/10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "auto", 0, QApplication::UnicodeUTF8)
        );
        comboBox_V->clear();
        comboBox_V->insertItems(0, QStringList()
         << QApplication::translate("SetupClass", "V", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "H", 0, QApplication::UnicodeUTF8)
        );
        comboBox_SPK->clear();
        comboBox_SPK->insertItems(0, QStringList()
         << QApplication::translate("SetupClass", "QPSK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "8PSK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "16APSK", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "VSB16", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "VSB8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "QAM16", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "QAM32", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "QAM64", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "QAM128", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "QAM256", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "QAMAUTO", 0, QApplication::UnicodeUTF8)
        );
        labe_symbl_rate_6->setText(QApplication::translate("SetupClass", "Roll Off:", 0, QApplication::UnicodeUTF8));
        labe_symbl_rate_9->setText(QString());
        comboBox_decimals->clear();
        comboBox_decimals->insertItems(0, QStringList()
         << QApplication::translate("SetupClass", "0.20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "0.25", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "0.35", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetupClass", "auto", 0, QApplication::UnicodeUTF8)
        );
        labe_symbl_rate_12->setText(QString());
        labe_symbl_rate_11->setText(QString());
        groupBox_Network->setTitle(QApplication::translate("SetupClass", "Network Device", 0, QApplication::UnicodeUTF8));
        checkBox_DHCP->setText(QApplication::translate("SetupClass", "DHCP", 0, QApplication::UnicodeUTF8));
        label_ip->setText(QApplication::translate("SetupClass", "IP Address:", 0, QApplication::UnicodeUTF8));
        label_Netm_mask->setText(QApplication::translate("SetupClass", "Netmask:", 0, QApplication::UnicodeUTF8));
        lineEdit_Net_AsK->setInputMask(QString());
        label_Gateway->setText(QApplication::translate("SetupClass", "Gateway:", 0, QApplication::UnicodeUTF8));
        lineEdit_GateWay->setInputMask(QString());
        groupBox_Network_2->setTitle(QApplication::translate("SetupClass", "Internet", 0, QApplication::UnicodeUTF8));
        label_DNS->setText(QApplication::translate("SetupClass", "DNS 1:", 0, QApplication::UnicodeUTF8));
        label_DNS_2->setText(QApplication::translate("SetupClass", "DNS 2:", 0, QApplication::UnicodeUTF8));
        lineEdit_DNS_2->setInputMask(QString());
        label_Gateway_2->setText(QApplication::translate("SetupClass", "Remote:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SetupClass);
    } // retranslateUi

};

namespace Ui {
    class SetupClass: public Ui_SetupClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUP_H
