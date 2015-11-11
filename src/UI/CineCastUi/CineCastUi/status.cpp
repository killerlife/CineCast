#include "status.h"
#include <QString>

Status::Status(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Init();
}

Status::~Status()
{

}

void Status::Init()
{
	ui.agc->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_2->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_3->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_4->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_5->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_6->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_8->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_9->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_Receiver->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_filmName->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.progressBar_siganl_Strength->setStyleSheet("QProgressBar{font-size: 18px; font-family:'Book Antiqua';}");
	ui.progressBar_siganl_Quality->setStyleSheet("QProgressBar{font-size: 18px; font-family:'Book Antiqua';}");
	ui.progressBar_Revceiver_length->setStyleSheet("QProgressBar{font-size: 18px; font-family:'Book Antiqua';}");
}

void Status::UpdateSatellite(TUNER_INFO* tInfo)
{
	ui.progressBar_siganl_Strength->setValue(tInfo->nAGC);
	ui.progressBar_siganl_Quality->setValue(tInfo->nSNR);
	if(tInfo->nLock)
		ui.label_4->setText(tr("Yes"));
	else
		ui.label_4->setText(tr("No"));
}

void Status::UpdateRecv(RECEIVE_INFO* tInfo)
{
	ui.label_filmName->setText(tInfo->strFilmName.c_str());
	QString s;
	s.sprintf("%lld/%lld", tInfo->nReceiveLength, tInfo->nFileLength);
	ui.label_Receiver->setText(s);
	if(tInfo->nFileLength > 0)
	{
		ui.progressBar_Revceiver_length->setValue(tInfo->nReceiveLength/tInfo->nFileLength*100);
	}
}