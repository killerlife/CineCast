#include "live.h"

Live::Live(QTcpSocket * socket, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
}

Live::~Live()
{

}

void Live::Init()
{
	ui.groupBox->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.groupBox_2->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_2->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.pushButton->setStyleSheet("QPushButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.pushButton_2->setStyleSheet("QPushButton{font-size: 20px; font-family:'Book Antiqua';}");

	QFont font;
	font.setFamily("Book Antiqua");
	font.setPixelSize(18);
	ui.treeWidget->headerItem()->setFont(0, font);

	ui.radioButton_3->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_4->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_5->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_6->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_7->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_8->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_9->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_10->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");
	ui.radioButton_11->setStyleSheet("QRadioButton{font-size: 20px; font-family:'Book Antiqua';}");

	ui.label_3->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_4->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_5->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_6->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_7->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");

	ui.checkBox->setStyleSheet("QCheckBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.checkBox_2->setStyleSheet("QCheckBox{font-size: 20px; font-family:'Book Antiqua';}");

	ui.lineEdit->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");
	ui.lineEdit_2->setStyleSheet("QLineEdit{font-size:18px;font-family:'Book Antiqua';}");

	ui.radioButton->setChecked(true);
	ui.radioButton_2->setChecked(false);
	ui.radioButton_6->setChecked(true);
	ui.checkBox_2->setChecked(true);
	ui.checkBox->setChecked(true);
	ui.radioButton_10->setChecked(true);
	ui.groupBox_4->setEnabled(false);
}

void Live::on_radioButton_clicked()
{
	ui.radioButton->setChecked(true);
	ui.radioButton_2->setChecked(false);
	ui.groupBox_3->setEnabled(true);
	ui.groupBox_4->setEnabled(false);
}

void Live::on_radioButton_2_clicked()
{
	ui.radioButton->setChecked(false);
	ui.radioButton_2->setChecked(true);
	ui.groupBox_3->setEnabled(false);
	ui.groupBox_4->setEnabled(true);
}