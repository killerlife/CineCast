/********************************************************************************
** Form generated from reading ui file 'status.ui'
**
** Created: Sun Jun 5 22:57:59 2016
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
#include <QtGui/QFrame>
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
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_23;
    QLabel *label;
    QLabel *label_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *agc;
    QLabel *label_2;
    QVBoxLayout *verticalLayout_5;
    QProgressBar *progressBar_siganl_Strength;
    QProgressBar *progressBar_siganl_Quality;
    QGroupBox *groupBox_Filme;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_filmName;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_10;
    QProgressBar *progressBar_Revceiver_length;
    QLabel *label_Receiver;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_9;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_40;
    QVBoxLayout *verticalLayout_10;
    QTextBrowser *textBrowser;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_11;
    QLabel *label_10;
    QVBoxLayout *verticalLayout_12;
    QFrame *line;
    QLabel *label_19;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_13;
    QLabel *label_12;
    QVBoxLayout *verticalLayout_22;
    QFrame *line_2;
    QLabel *label_32;
    QVBoxLayout *verticalLayout_15;
    QLabel *label_21;
    QLabel *label_20;
    QVBoxLayout *verticalLayout_68;
    QFrame *line_26;
    QLabel *label_100;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_15;
    QLabel *label_14;
    QVBoxLayout *verticalLayout_24;
    QFrame *line_3;
    QLabel *label_33;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_17;
    QLabel *label_16;
    QVBoxLayout *verticalLayout_25;
    QFrame *line_4;
    QLabel *label_34;
    QVBoxLayout *verticalLayout_17;
    QLabel *label_23;
    QLabel *label_22;
    QVBoxLayout *verticalLayout_26;
    QFrame *line_5;
    QLabel *label_35;
    QVBoxLayout *verticalLayout_18;
    QLabel *label_25;
    QLabel *label_24;
    QVBoxLayout *verticalLayout_27;
    QFrame *line_6;
    QLabel *label_36;
    QVBoxLayout *verticalLayout_19;
    QLabel *label_27;
    QLabel *label_26;
    QVBoxLayout *verticalLayout_28;
    QFrame *line_7;
    QLabel *label_37;
    QVBoxLayout *verticalLayout_20;
    QLabel *label_29;
    QLabel *label_28;
    QVBoxLayout *verticalLayout_30;
    QFrame *line_9;
    QLabel *label_39;
    QVBoxLayout *verticalLayout_21;
    QLabel *label_31;
    QLabel *label_30;
    QVBoxLayout *verticalLayout_69;
    QFrame *line_27;
    QLabel *label_101;
    QVBoxLayout *verticalLayout_16;
    QLabel *label_7;
    QLabel *label_18;
    QLabel *label_38;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *StatusClass)
    {
        if (StatusClass->objectName().isEmpty())
            StatusClass->setObjectName(QString::fromUtf8("StatusClass"));
        StatusClass->resize(1044, 395);
        QFont font;
        font.setFamily(QString::fromUtf8("Book Antiqua"));
        font.setPointSize(14);
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
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_23 = new QVBoxLayout();
        verticalLayout_23->setSpacing(6);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        label = new QLabel(StatusClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/satellite.png")));

        verticalLayout_23->addWidget(label);

        label_4 = new QLabel(StatusClass);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(50, 0));
        label_4->setMaximumSize(QSize(175, 16777215));
        label_4->setFont(font);

        verticalLayout_23->addWidget(label_4);


        horizontalLayout_6->addLayout(verticalLayout_23);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        agc = new QLabel(StatusClass);
        agc->setObjectName(QString::fromUtf8("agc"));
        agc->setMinimumSize(QSize(70, 0));
        agc->setMaximumSize(QSize(85, 16777215));
        agc->setFont(font);

        verticalLayout_4->addWidget(agc);

        label_2 = new QLabel(StatusClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(70, 0));
        label_2->setMaximumSize(QSize(85, 16777215));
        label_2->setFont(font);

        verticalLayout_4->addWidget(label_2);


        horizontalLayout_6->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        progressBar_siganl_Strength = new QProgressBar(StatusClass);
        progressBar_siganl_Strength->setObjectName(QString::fromUtf8("progressBar_siganl_Strength"));
        progressBar_siganl_Strength->setFont(font);
        progressBar_siganl_Strength->setValue(0);
        progressBar_siganl_Strength->setTextDirection(QProgressBar::BottomToTop);

        verticalLayout_5->addWidget(progressBar_siganl_Strength);

        progressBar_siganl_Quality = new QProgressBar(StatusClass);
        progressBar_siganl_Quality->setObjectName(QString::fromUtf8("progressBar_siganl_Quality"));
        progressBar_siganl_Quality->setFont(font);
        progressBar_siganl_Quality->setValue(0);
        progressBar_siganl_Quality->setTextDirection(QProgressBar::BottomToTop);

        verticalLayout_5->addWidget(progressBar_siganl_Quality);


        horizontalLayout_6->addLayout(verticalLayout_5);


        verticalLayout_3->addLayout(horizontalLayout_6);

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
        label_5->setMinimumSize(QSize(80, 0));
        label_5->setMaximumSize(QSize(175, 16777215));

        horizontalLayout->addWidget(label_5);


        verticalLayout_6->addLayout(horizontalLayout);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_8 = new QLabel(groupBox_Filme);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(80, 0));
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

        verticalLayout_7->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        progressBar_Revceiver_length = new QProgressBar(groupBox_Filme);
        progressBar_Revceiver_length->setObjectName(QString::fromUtf8("progressBar_Revceiver_length"));
        progressBar_Revceiver_length->setMaximum(10000);
        progressBar_Revceiver_length->setValue(0);

        horizontalLayout_10->addWidget(progressBar_Revceiver_length);

        label_Receiver = new QLabel(groupBox_Filme);
        label_Receiver->setObjectName(QString::fromUtf8("label_Receiver"));
        label_Receiver->setMinimumSize(QSize(150, 0));
        label_Receiver->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(label_Receiver);


        verticalLayout_7->addLayout(horizontalLayout_10);


        horizontalLayout_2->addLayout(verticalLayout_7);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        label_9 = new QLabel(groupBox_Filme);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(80, 0));
        label_9->setMaximumSize(QSize(175, 16777215));

        verticalLayout_9->addWidget(label_9);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_40 = new QLabel(groupBox_Filme);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setEnabled(false);
        label_40->setMinimumSize(QSize(80, 0));
        label_40->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/Remote.png")));
        label_40->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_11->addWidget(label_40);


        verticalLayout_9->addLayout(horizontalLayout_11);


        horizontalLayout_5->addLayout(verticalLayout_9);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        textBrowser = new QTextBrowser(groupBox_Filme);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setMaximumSize(QSize(16777215, 80));

        verticalLayout_10->addWidget(textBrowser);


        horizontalLayout_5->addLayout(verticalLayout_10);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_11 = new QLabel(groupBox_Filme);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/idle.png")));
        label_11->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(label_11);

        label_10 = new QLabel(groupBox_Filme);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(label_10);


        horizontalLayout_4->addLayout(verticalLayout_8);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        line = new QFrame(groupBox_Filme);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMinimumSize(QSize(0, 32));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_12->addWidget(line);

        label_19 = new QLabel(groupBox_Filme);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setMaximumSize(QSize(16777215, 29));

        verticalLayout_12->addWidget(label_19);


        horizontalLayout_4->addLayout(verticalLayout_12);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        label_13 = new QLabel(groupBox_Filme);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setEnabled(false);
        label_13->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/recv.png")));
        label_13->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(label_13);

        label_12 = new QLabel(groupBox_Filme);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setEnabled(false);
        label_12->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(label_12);


        horizontalLayout_4->addLayout(verticalLayout_11);

        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setSpacing(6);
        verticalLayout_22->setObjectName(QString::fromUtf8("verticalLayout_22"));
        line_2 = new QFrame(groupBox_Filme);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setMinimumSize(QSize(0, 32));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_22->addWidget(line_2);

        label_32 = new QLabel(groupBox_Filme);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setMaximumSize(QSize(16777215, 29));

        verticalLayout_22->addWidget(label_32);


        horizontalLayout_4->addLayout(verticalLayout_22);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        label_21 = new QLabel(groupBox_Filme);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setEnabled(false);
        label_21->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/idle.png")));
        label_21->setAlignment(Qt::AlignCenter);

        verticalLayout_15->addWidget(label_21);

        label_20 = new QLabel(groupBox_Filme);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setEnabled(false);
        label_20->setAlignment(Qt::AlignCenter);

        verticalLayout_15->addWidget(label_20);


        horizontalLayout_4->addLayout(verticalLayout_15);

        verticalLayout_68 = new QVBoxLayout();
        verticalLayout_68->setSpacing(6);
        verticalLayout_68->setObjectName(QString::fromUtf8("verticalLayout_68"));
        line_26 = new QFrame(groupBox_Filme);
        line_26->setObjectName(QString::fromUtf8("line_26"));
        line_26->setMinimumSize(QSize(0, 32));
        line_26->setFrameShape(QFrame::HLine);
        line_26->setFrameShadow(QFrame::Sunken);

        verticalLayout_68->addWidget(line_26);

        label_100 = new QLabel(groupBox_Filme);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setMaximumSize(QSize(16777215, 29));

        verticalLayout_68->addWidget(label_100);


        horizontalLayout_4->addLayout(verticalLayout_68);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        label_15 = new QLabel(groupBox_Filme);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setEnabled(false);
        label_15->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/analysis.png")));
        label_15->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(label_15);

        label_14 = new QLabel(groupBox_Filme);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setEnabled(false);
        label_14->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(label_14);


        horizontalLayout_4->addLayout(verticalLayout_13);

        verticalLayout_24 = new QVBoxLayout();
        verticalLayout_24->setSpacing(6);
        verticalLayout_24->setObjectName(QString::fromUtf8("verticalLayout_24"));
        line_3 = new QFrame(groupBox_Filme);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setMinimumSize(QSize(0, 32));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_24->addWidget(line_3);

        label_33 = new QLabel(groupBox_Filme);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setMaximumSize(QSize(16777215, 29));

        verticalLayout_24->addWidget(label_33);


        horizontalLayout_4->addLayout(verticalLayout_24);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        label_17 = new QLabel(groupBox_Filme);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setEnabled(false);
        label_17->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/report.png")));
        label_17->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(label_17);

        label_16 = new QLabel(groupBox_Filme);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setEnabled(false);
        label_16->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(label_16);


        horizontalLayout_4->addLayout(verticalLayout_14);

        verticalLayout_25 = new QVBoxLayout();
        verticalLayout_25->setSpacing(6);
        verticalLayout_25->setObjectName(QString::fromUtf8("verticalLayout_25"));
        line_4 = new QFrame(groupBox_Filme);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setMinimumSize(QSize(0, 32));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_25->addWidget(line_4);

        label_34 = new QLabel(groupBox_Filme);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setMaximumSize(QSize(16777215, 29));

        verticalLayout_25->addWidget(label_34);


        horizontalLayout_4->addLayout(verticalLayout_25);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        label_23 = new QLabel(groupBox_Filme);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setEnabled(false);
        label_23->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/idle.png")));
        label_23->setAlignment(Qt::AlignCenter);

        verticalLayout_17->addWidget(label_23);

        label_22 = new QLabel(groupBox_Filme);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setEnabled(false);
        label_22->setAlignment(Qt::AlignCenter);

        verticalLayout_17->addWidget(label_22);


        horizontalLayout_4->addLayout(verticalLayout_17);

        verticalLayout_26 = new QVBoxLayout();
        verticalLayout_26->setSpacing(6);
        verticalLayout_26->setObjectName(QString::fromUtf8("verticalLayout_26"));
        line_5 = new QFrame(groupBox_Filme);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setMinimumSize(QSize(0, 32));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_26->addWidget(line_5);

        label_35 = new QLabel(groupBox_Filme);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setMaximumSize(QSize(16777215, 29));

        verticalLayout_26->addWidget(label_35);


        horizontalLayout_4->addLayout(verticalLayout_26);

        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        label_25 = new QLabel(groupBox_Filme);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setEnabled(false);
        label_25->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/idle.png")));
        label_25->setAlignment(Qt::AlignCenter);

        verticalLayout_18->addWidget(label_25);

        label_24 = new QLabel(groupBox_Filme);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setEnabled(false);
        label_24->setAlignment(Qt::AlignCenter);

        verticalLayout_18->addWidget(label_24);


        horizontalLayout_4->addLayout(verticalLayout_18);

        verticalLayout_27 = new QVBoxLayout();
        verticalLayout_27->setSpacing(6);
        verticalLayout_27->setObjectName(QString::fromUtf8("verticalLayout_27"));
        line_6 = new QFrame(groupBox_Filme);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setMinimumSize(QSize(0, 32));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);

        verticalLayout_27->addWidget(line_6);

        label_36 = new QLabel(groupBox_Filme);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setMaximumSize(QSize(16777215, 29));

        verticalLayout_27->addWidget(label_36);


        horizontalLayout_4->addLayout(verticalLayout_27);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        label_27 = new QLabel(groupBox_Filme);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setEnabled(false);
        label_27->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/analysis.png")));
        label_27->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(label_27);

        label_26 = new QLabel(groupBox_Filme);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setEnabled(false);
        label_26->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(label_26);


        horizontalLayout_4->addLayout(verticalLayout_19);

        verticalLayout_28 = new QVBoxLayout();
        verticalLayout_28->setSpacing(6);
        verticalLayout_28->setObjectName(QString::fromUtf8("verticalLayout_28"));
        line_7 = new QFrame(groupBox_Filme);
        line_7->setObjectName(QString::fromUtf8("line_7"));
        line_7->setMinimumSize(QSize(0, 32));
        line_7->setFrameShape(QFrame::HLine);
        line_7->setFrameShadow(QFrame::Sunken);

        verticalLayout_28->addWidget(line_7);

        label_37 = new QLabel(groupBox_Filme);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setMaximumSize(QSize(16777215, 29));

        verticalLayout_28->addWidget(label_37);


        horizontalLayout_4->addLayout(verticalLayout_28);

        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        label_29 = new QLabel(groupBox_Filme);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setEnabled(false);
        label_29->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/idle.png")));
        label_29->setAlignment(Qt::AlignCenter);

        verticalLayout_20->addWidget(label_29);

        label_28 = new QLabel(groupBox_Filme);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setEnabled(false);
        label_28->setAlignment(Qt::AlignCenter);

        verticalLayout_20->addWidget(label_28);


        horizontalLayout_4->addLayout(verticalLayout_20);

        verticalLayout_30 = new QVBoxLayout();
        verticalLayout_30->setSpacing(6);
        verticalLayout_30->setObjectName(QString::fromUtf8("verticalLayout_30"));
        line_9 = new QFrame(groupBox_Filme);
        line_9->setObjectName(QString::fromUtf8("line_9"));
        line_9->setMinimumSize(QSize(0, 32));
        line_9->setFrameShape(QFrame::HLine);
        line_9->setFrameShadow(QFrame::Sunken);

        verticalLayout_30->addWidget(line_9);

        label_39 = new QLabel(groupBox_Filme);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setMaximumSize(QSize(16777215, 29));

        verticalLayout_30->addWidget(label_39);


        horizontalLayout_4->addLayout(verticalLayout_30);

        verticalLayout_21 = new QVBoxLayout();
        verticalLayout_21->setSpacing(6);
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        label_31 = new QLabel(groupBox_Filme);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setEnabled(false);
        label_31->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/recv.png")));
        label_31->setAlignment(Qt::AlignCenter);

        verticalLayout_21->addWidget(label_31);

        label_30 = new QLabel(groupBox_Filme);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setEnabled(false);
        label_30->setAlignment(Qt::AlignCenter);

        verticalLayout_21->addWidget(label_30);


        horizontalLayout_4->addLayout(verticalLayout_21);

        verticalLayout_69 = new QVBoxLayout();
        verticalLayout_69->setSpacing(6);
        verticalLayout_69->setObjectName(QString::fromUtf8("verticalLayout_69"));
        line_27 = new QFrame(groupBox_Filme);
        line_27->setObjectName(QString::fromUtf8("line_27"));
        line_27->setMinimumSize(QSize(0, 32));
        line_27->setFrameShape(QFrame::HLine);
        line_27->setFrameShadow(QFrame::Sunken);

        verticalLayout_69->addWidget(line_27);

        label_101 = new QLabel(groupBox_Filme);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setMaximumSize(QSize(16777215, 29));

        verticalLayout_69->addWidget(label_101);


        horizontalLayout_4->addLayout(verticalLayout_69);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        label_7 = new QLabel(groupBox_Filme);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setEnabled(false);
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/finish.png")));
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_16->addWidget(label_7);

        label_18 = new QLabel(groupBox_Filme);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setEnabled(false);
        label_18->setAlignment(Qt::AlignCenter);

        verticalLayout_16->addWidget(label_18);


        horizontalLayout_4->addLayout(verticalLayout_16);


        verticalLayout_2->addLayout(horizontalLayout_4);


        verticalLayout_3->addWidget(groupBox_Filme);

        label_38 = new QLabel(StatusClass);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Book Antiqua"));
        font1.setPointSize(20);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        font1.setStrikeOut(false);
        label_38->setFont(font1);
        label_38->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_38);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        retranslateUi(StatusClass);

        QMetaObject::connectSlotsByName(StatusClass);
    } // setupUi

    void retranslateUi(QWidget *StatusClass)
    {
        StatusClass->setWindowTitle(QApplication::translate("StatusClass", "Status", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_4->setText(QApplication::translate("StatusClass", "No", 0, QApplication::UnicodeUTF8));
        agc->setText(QApplication::translate("StatusClass", "AGC:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StatusClass", "SNR:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("StatusClass", "Film Name:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("StatusClass", "Received Ratio:", 0, QApplication::UnicodeUTF8));
        label_filmName->setText(QApplication::translate("StatusClass", "N/A", 0, QApplication::UnicodeUTF8));
        label_Receiver->setText(QApplication::translate("StatusClass", "0/0", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("StatusClass", "Status:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_40->setToolTip(QApplication::translate("StatusClass", "Remote connection status, it'll turn green after connected", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_10->setText(QApplication::translate("StatusClass", "Idle", 0, QApplication::UnicodeUTF8));
        label_19->setText(QString());
        label_12->setText(QApplication::translate("StatusClass", "Receive", 0, QApplication::UnicodeUTF8));
        label_32->setText(QString());
        label_20->setText(QApplication::translate("StatusClass", "Received", 0, QApplication::UnicodeUTF8));
        label_100->setText(QString());
        label_14->setText(QApplication::translate("StatusClass", "Analysis", 0, QApplication::UnicodeUTF8));
        label_33->setText(QString());
        label_16->setText(QApplication::translate("StatusClass", "Report", 0, QApplication::UnicodeUTF8));
        label_34->setText(QString());
        label_22->setText(QApplication::translate("StatusClass", "Confirm", 0, QApplication::UnicodeUTF8));
        label_35->setText(QString());
        label_24->setText(QApplication::translate("StatusClass", "MD5 Req.", 0, QApplication::UnicodeUTF8));
        label_36->setText(QString());
        label_26->setText(QApplication::translate("StatusClass", "MD5 Confirm", 0, QApplication::UnicodeUTF8));
        label_37->setText(QString());
        label_28->setText(QApplication::translate("StatusClass", "Verify", 0, QApplication::UnicodeUTF8));
        label_39->setText(QString());
        label_30->setText(QApplication::translate("StatusClass", "Verified", 0, QApplication::UnicodeUTF8));
        label_101->setText(QString());
        label_18->setText(QApplication::translate("StatusClass", "Finish", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(StatusClass);
    } // retranslateUi

};

namespace Ui {
    class StatusClass: public Ui_StatusClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATUS_H
