#include "tkr.h"

Tkr::Tkr(QTcpSocket * socket, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
}

Tkr::~Tkr()
{

}

void Tkr::Init()
{
	QFont font;
	font.setFamily("Book Antiqua");
	font.setPixelSize(18);
	ui.treeWidget->headerItem()->setFont(0, font);
	ui.treeWidget->headerItem()->setFont(1, font);
	ui.treeWidget->headerItem()->setFont(2, font);
	ui.treeWidget->headerItem()->setFont(3, font);
	ui.treeWidget->headerItem()->setFont(4, font);

	ui.label->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_2->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_3->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.lineEdit->setStyleSheet("QLineEdit{font-size: 20px; font-family:'Book Antiqua';}");
	ui.lineEdit_2->setStyleSheet("QLineEdit{font-size: 20px; font-family:'Book Antiqua';}");
	ui.lineEdit_3->setStyleSheet("QLineEdit{font-size: 20px; font-family:'Book Antiqua';}");

	ui.groupBox->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_2->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_3->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.pushButton_4->setStyleSheet("QPushButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.lineEdit_4->setStyleSheet("QLineEdit{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton->setChecked(true);
#if 0
	ui.groupBox->setEnabled(false);
	ui.groupBox_2->setEnabled(false);
	ui.treeWidget->setEnabled(false);
	ui.pushButton->setEnabled(false);
	ui.pushButton_2->setEnabled(false);
	ui.pushButton_3->setEnabled(false);
#endif
}


void Tkr::on_treeWidget_clicked(const QModelIndex &)
{

}

void Tkr::on_groupBox_2_toggled(bool)
{

}