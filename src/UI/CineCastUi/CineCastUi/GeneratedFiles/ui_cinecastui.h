/********************************************************************************
** Form generated from reading ui file 'cinecastui.ui'
**
** Created: Mon Nov 9 10:38:42 2015
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CINECASTUI_H
#define UI_CINECASTUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMdiArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CineCastUiClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_logoBig;
    QLabel *label_LOGO;
    QSpacerItem *horizontalSpacer_2;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QVBoxLayout *verticalLayout_2;
    QMdiArea *mdiArea_statue;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_3;
    QMdiArea *mdiArea_ContentManagement;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_2;
    QMdiArea *mdiArea_Maintain;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_4;
    QMdiArea *mdiArea_Setup;

    void setupUi(QWidget *CineCastUiClass)
    {
        if (CineCastUiClass->objectName().isEmpty())
            CineCastUiClass->setObjectName(QString::fromUtf8("CineCastUiClass"));
        CineCastUiClass->resize(818, 572);
        verticalLayout = new QVBoxLayout(CineCastUiClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_logoBig = new QLabel(CineCastUiClass);
        label_logoBig->setObjectName(QString::fromUtf8("label_logoBig"));
        label_logoBig->setPixmap(QPixmap(QString::fromUtf8(":/CineCastUi/Resources/logobig.png")));

        horizontalLayout_3->addWidget(label_logoBig);

        label_LOGO = new QLabel(CineCastUiClass);
        label_LOGO->setObjectName(QString::fromUtf8("label_LOGO"));
        QFont font;
        font.setFamily(QString::fromUtf8("Book Antiqua"));
        font.setPointSize(30);
        font.setBold(true);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(75);
        font.setStrikeOut(false);
        label_LOGO->setFont(font);

        horizontalLayout_3->addWidget(label_LOGO);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        tabWidget = new QTabWidget(CineCastUiClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Book Antiqua"));
        font1.setPointSize(20);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        font1.setStrikeOut(false);
        tabWidget->setFont(font1);
        tabWidget->setAutoFillBackground(true);
        tabWidget->setStyleSheet(QString::fromUtf8("QTabWidget#Tab1>QTabBar::tab:first\n"
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
        tabWidget->setProperty("tabsClosable", QVariant(false));
        tabWidget->setProperty("movable", QVariant(true));
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        verticalLayout_2 = new QVBoxLayout(tab_1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        mdiArea_statue = new QMdiArea(tab_1);
        mdiArea_statue->setObjectName(QString::fromUtf8("mdiArea_statue"));

        verticalLayout_2->addWidget(mdiArea_statue);

        tabWidget->addTab(tab_1, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_3 = new QVBoxLayout(tab_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setMargin(11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        mdiArea_ContentManagement = new QMdiArea(tab_3);
        mdiArea_ContentManagement->setObjectName(QString::fromUtf8("mdiArea_ContentManagement"));

        verticalLayout_3->addWidget(mdiArea_ContentManagement);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_2 = new QHBoxLayout(tab_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setMargin(11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        mdiArea_Maintain = new QMdiArea(tab_2);
        mdiArea_Maintain->setObjectName(QString::fromUtf8("mdiArea_Maintain"));

        horizontalLayout_2->addWidget(mdiArea_Maintain);

        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_4 = new QVBoxLayout(tab_4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setMargin(11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        mdiArea_Setup = new QMdiArea(tab_4);
        mdiArea_Setup->setObjectName(QString::fromUtf8("mdiArea_Setup"));
        QFont font2;
        font2.setPointSize(10);
        mdiArea_Setup->setFont(font2);

        verticalLayout_4->addWidget(mdiArea_Setup);

        tabWidget->addTab(tab_4, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(CineCastUiClass);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(CineCastUiClass);
    } // setupUi

    void retranslateUi(QWidget *CineCastUiClass)
    {
        CineCastUiClass->setWindowTitle(QApplication::translate("CineCastUiClass", "CineCastUi", 0, QApplication::UnicodeUTF8));
        label_logoBig->setText(QString());
        label_LOGO->setText(QApplication::translate("CineCastUiClass", "  CineCast", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("CineCastUiClass", "Status", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("CineCastUiClass", "Content", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("CineCastUiClass", "Manager", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("CineCastUiClass", "Setup", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CineCastUiClass);
    } // retranslateUi

};

namespace Ui {
    class CineCastUiClass: public Ui_CineCastUiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CINECASTUI_H
