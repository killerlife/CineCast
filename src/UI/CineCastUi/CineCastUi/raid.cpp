#include "raid.h"

Raid::Raid(QTcpSocket * socket, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
}

Raid::~Raid()
{

}

void Raid::Init()
{
	ui.groupBox->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.groupBox_2->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.groupBox_3->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.groupBox_4->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.groupBox_5->setStyleSheet("QGroupBox{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_7->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_6->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_5->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_4->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_3->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_2->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_9->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_8->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_11->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_13->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_14->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_15->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_17->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_16->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_19->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_18->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_20->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_21->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_22->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_23->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_24->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_25->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");

	ui.pushButton->setStyleSheet("QPushButton{font-size: 20px; font-family:'Book Antiqua';}");

	QFont font;
	font.setFamily("Book Antiqua");
	font.setPixelSize(18);
	ui.treeWidget_HDD_info->headerItem()->setFont(0, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(1, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(2, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(3, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(4, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(5, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(6, font);
	ui.treeWidget_HDD_info->headerItem()->setFont(7, font);

	ui.treeWidget_HDD_info_2->headerItem()->setFont(0, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(1, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(2, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(3, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(4, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(5, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(6, font);
	ui.treeWidget_HDD_info_2->headerItem()->setFont(7, font);
}