/********************************************************************************
** Form generated from reading UI file 'leoniscinemacinecast.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEONISCINEMACINECAST_H
#define UI_LEONISCINEMACINECAST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LEONISCINEMACineCast
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_logoBig;
    QLabel *label_LOGO;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QVBoxLayout *verticalLayout_2;
    QMdiArea *mdiArea_statue;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_2;
    QMdiArea *mdiArea_Maintain;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_3;
    QMdiArea *mdiArea_ContentManagement;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_4;
    QMdiArea *mdiArea_Setup;

    void setupUi(QWidget *LEONISCINEMACineCast)
    {
        if (LEONISCINEMACineCast->objectName().isEmpty())
            LEONISCINEMACineCast->setObjectName(QStringLiteral("LEONISCINEMACineCast"));
        LEONISCINEMACineCast->resize(1118, 629);
        LEONISCINEMACineCast->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(LEONISCINEMACineCast);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalSpacer_3 = new QSpacerItem(1097, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_logoBig = new QLabel(LEONISCINEMACineCast);
        label_logoBig->setObjectName(QStringLiteral("label_logoBig"));

        horizontalLayout_3->addWidget(label_logoBig);

        label_LOGO = new QLabel(LEONISCINEMACineCast);
        label_LOGO->setObjectName(QStringLiteral("label_LOGO"));
        QFont font;
        font.setFamily(QStringLiteral("Book Antiqua"));
        font.setPointSize(26);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        label_LOGO->setFont(font);

        horizontalLayout_3->addWidget(label_LOGO);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        tabWidget = new QTabWidget(LEONISCINEMACineCast);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(14);
        tabWidget->setFont(font1);
        tabWidget->setAutoFillBackground(true);
        tabWidget->setStyleSheet(QLatin1String("QTabWidget#Tab1>QTabBar::tab:first\n"
"{\n"
"    border-image: url(:/NULLbackground.png);\n"
"}\n"
"QTabWidget#Tab1>QTabBar::tab:first:hover\n"
"{\n"
"    border-image: url(:/NULLbackground.png);\n"
"}\n"
"QTabWidget#Tab1>QTabBar::tab:first:pressed\n"
"{\n"
"    border-image: url(:/NULLbackground.png);\n"
"}\n"
"QTabWidget#Tab1>QTabBar::tab:last\n"
"{\n"
"    border-image: url(:/NULLbackground.png);\n"
"}\n"
"QTabWidget#Tab1>QTabBar::tab:last:hover\n"
"{\n"
"    border-image: url(:/NULLbackground.png);\n"
"}\n"
"QTabWidget#Tab1>QTabBar::tab:last:pressed\n"
"{\n"
"    border-image: url(:NULLbackground.png);\n"
"}"));
        tabWidget->setTabShape(QTabWidget::Triangular);
        tabWidget->setIconSize(QSize(20, 35));
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(true);
        tab_1 = new QWidget();
        tab_1->setObjectName(QStringLiteral("tab_1"));
        verticalLayout_2 = new QVBoxLayout(tab_1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        mdiArea_statue = new QMdiArea(tab_1);
        mdiArea_statue->setObjectName(QStringLiteral("mdiArea_statue"));

        verticalLayout_2->addWidget(mdiArea_statue);

        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalLayout_2 = new QHBoxLayout(tab_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        mdiArea_Maintain = new QMdiArea(tab_2);
        mdiArea_Maintain->setObjectName(QStringLiteral("mdiArea_Maintain"));

        horizontalLayout_2->addWidget(mdiArea_Maintain);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_3 = new QVBoxLayout(tab_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        mdiArea_ContentManagement = new QMdiArea(tab_3);
        mdiArea_ContentManagement->setObjectName(QStringLiteral("mdiArea_ContentManagement"));

        verticalLayout_3->addWidget(mdiArea_ContentManagement);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        verticalLayout_4 = new QVBoxLayout(tab_4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        mdiArea_Setup = new QMdiArea(tab_4);
        mdiArea_Setup->setObjectName(QStringLiteral("mdiArea_Setup"));
        QFont font2;
        font2.setPointSize(10);
        mdiArea_Setup->setFont(font2);

        verticalLayout_4->addWidget(mdiArea_Setup);

        tabWidget->addTab(tab_4, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(LEONISCINEMACineCast);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(LEONISCINEMACineCast);
    } // setupUi

    void retranslateUi(QWidget *LEONISCINEMACineCast)
    {
        LEONISCINEMACineCast->setWindowTitle(QApplication::translate("LEONISCINEMACineCast", "LEONISCINEMACineCast", 0));
        label_logoBig->setText(QString());
        label_LOGO->setText(QApplication::translate("LEONISCINEMACineCast", " CineCast", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("LEONISCINEMACineCast", "   \347\212\266\346\200\201   ", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("LEONISCINEMACineCast", "   \347\273\264\346\212\244   ", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("LEONISCINEMACineCast", " \345\206\205\345\256\271\347\256\241\347\220\206 ", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("LEONISCINEMACineCast", " \345\256\211\350\243\205\350\256\276\347\275\256 ", 0));
    } // retranslateUi

};

namespace Ui {
    class LEONISCINEMACineCast: public Ui_LEONISCINEMACineCast {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEONISCINEMACINECAST_H
