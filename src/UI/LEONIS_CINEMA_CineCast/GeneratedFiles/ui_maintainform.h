/********************************************************************************
** Form generated from reading UI file 'maintainform.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINTAINFORM_H
#define UI_MAINTAINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaintainForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_format;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *MaintainForm)
    {
        if (MaintainForm->objectName().isEmpty())
            MaintainForm->setObjectName(QStringLiteral("MaintainForm"));
        MaintainForm->resize(709, 482);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        MaintainForm->setFont(font);
        verticalLayout_2 = new QVBoxLayout(MaintainForm);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(MaintainForm);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(13);
        groupBox->setFont(font1);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 172, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        horizontalLayout_4->addWidget(progressBar);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_format = new QPushButton(groupBox);
        pushButton_format->setObjectName(QStringLiteral("pushButton_format"));
        pushButton_format->setMaximumSize(QSize(120, 35));

        horizontalLayout->addWidget(pushButton_format);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 171, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addWidget(groupBox);


        retranslateUi(MaintainForm);

        QMetaObject::connectSlotsByName(MaintainForm);
    } // setupUi

    void retranslateUi(QWidget *MaintainForm)
    {
        MaintainForm->setWindowTitle(QApplication::translate("MaintainForm", "Form", 0));
        groupBox->setTitle(QApplication::translate("MaintainForm", "\346\212\275\345\217\226\345\274\217\347\241\254\347\233\230\347\256\241\347\220\206", 0));
        pushButton_format->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MaintainForm: public Ui_MaintainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINTAINFORM_H
