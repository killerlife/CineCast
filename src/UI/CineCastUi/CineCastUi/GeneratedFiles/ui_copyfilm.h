/********************************************************************************
** Form generated from reading ui file 'copyfilm.ui'
**
** Created: Tue Aug 16 10:38:28 2016
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_COPYFILM_H
#define UI_COPYFILM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CopyFilmClass
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QProgressBar *progressBar_2;
    QVBoxLayout *verticalLayout_5;
    QFrame *line;
    QListWidget *listWidget;
    QLabel *label_6;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *CopyFilmClass)
    {
        if (CopyFilmClass->objectName().isEmpty())
            CopyFilmClass->setObjectName(QString::fromUtf8("CopyFilmClass"));
        CopyFilmClass->resize(601, 411);
        CopyFilmClass->setContextMenuPolicy(Qt::DefaultContextMenu);
        verticalLayout = new QVBoxLayout(CopyFilmClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_3 = new QLabel(CopyFilmClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(80, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("Book Antiqua"));
        font.setPointSize(18);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        label_3->setFont(font);

        horizontalLayout_6->addWidget(label_3);

        label_2 = new QLabel(CopyFilmClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        horizontalLayout_6->addWidget(label_2);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_5 = new QLabel(CopyFilmClass);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(80, 16777215));
        label_5->setFont(font);

        horizontalLayout_7->addWidget(label_5);

        label_4 = new QLabel(CopyFilmClass);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        horizontalLayout_7->addWidget(label_4);


        verticalLayout_3->addLayout(horizontalLayout_7);


        verticalLayout->addLayout(verticalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(CopyFilmClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        verticalLayout_2->addWidget(progressBar);


        horizontalLayout_2->addWidget(groupBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        groupBox_2 = new QGroupBox(CopyFilmClass);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font);
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setMargin(11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        progressBar_2 = new QProgressBar(groupBox_2);
        progressBar_2->setObjectName(QString::fromUtf8("progressBar_2"));
        progressBar_2->setValue(0);

        verticalLayout_4->addWidget(progressBar_2);


        horizontalLayout_3->addWidget(groupBox_2);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        line = new QFrame(CopyFilmClass);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(line);

        listWidget = new QListWidget(CopyFilmClass);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMaximumSize(QSize(0, 0));

        verticalLayout_5->addWidget(listWidget);

        label_6 = new QLabel(CopyFilmClass);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Book Antiqua"));
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        font1.setStrikeOut(false);
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_6);

        label_7 = new QLabel(CopyFilmClass);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font1);
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_7);


        verticalLayout->addLayout(verticalLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(CopyFilmClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_8->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_8);


        retranslateUi(CopyFilmClass);

        QMetaObject::connectSlotsByName(CopyFilmClass);
    } // setupUi

    void retranslateUi(QDialog *CopyFilmClass)
    {
        CopyFilmClass->setWindowTitle(QApplication::translate("CopyFilmClass", "CopyFilm", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CopyFilmClass", "Films:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CopyFilmClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CopyFilmClass", "Total:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CopyFilmClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CopyFilmClass", "Current", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("CopyFilmClass", "Total", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CopyFilmClass", "Warning", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CopyFilmClass", "Can't stop copy operation while copying films.", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("CopyFilmClass", "Start", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CopyFilmClass);
    } // retranslateUi

};

namespace Ui {
    class CopyFilmClass: public Ui_CopyFilmClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COPYFILM_H
