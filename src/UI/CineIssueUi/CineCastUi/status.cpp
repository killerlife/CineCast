#include "status.h"
#include <QString>
#include <QTextCodec>
#include <QTextStream>

Status::Status(QWidget *parent)
	: QWidget(parent), p(parent)
{
	ui.setupUi(this);
	Init();
}

Status::~Status()
{

}

void Status::Init()
{
	ui.agc->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.label_2->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
// 	ui.label_3->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.label_4->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.label_5->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
// 	ui.label_6->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.label_8->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.label_9->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.label_Receiver->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.label_filmName->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.progressBar_siganl_Strength->setStyleSheet("QProgressBar{font-size: 14px; font-family:'Book Antiqua';}");
	ui.progressBar_siganl_Quality->setStyleSheet("QProgressBar{font-size: 14px; font-family:'Book Antiqua';}");
	ui.progressBar_Revceiver_length->setStyleSheet("QProgressBar{font-size: 14px; font-family:'Book Antiqua';}");
	ui.textBrowser->setStyleSheet("QTextBrowser{font-size: 14px; font-family:'Book Antiqua';}");

	ui.label_10->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_12->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_14->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_16->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_18->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_20->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_22->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_24->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_26->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_28->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_30->setStyleSheet("QLabel{font-size: 12px; font-family:'Book Antiqua';}");
	ui.label_38->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua'; color:#0000FF;}");
	ui.groupBox_Filme->setStyleSheet("QGroupBox{font-size: 14px; font-family:'Book Antiqua';}");
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
		ui.progressBar_Revceiver_length->setValue(tInfo->nReceiveLength*10000/tInfo->nFileLength);
	}
	else
		ui.progressBar_Revceiver_length->setValue(0);
	QString txt;
	QTextCodec *gbk = QTextCodec::codecForName("gb18030");
	QString creator = gbk->toUnicode(tInfo->strCreator.c_str());
	QString issuer = gbk->toUnicode(tInfo->strIssuer.c_str());
	txt = QString(tr("<pre style=\"font-size: 14px; font-family:Book Antiqua\">Creator: %2\tIssuer: %3\tIssueDate: %4\nRound: %9\tTotal Segment: %5\tReceived Segment: %6\nCRC Error: %7\tLost Segment:%8</pre>"))
		.arg(creator)
		.arg(issuer)
		.arg(tInfo->strIssueDate.c_str())
		.arg(QString::number(tInfo->nTotalSegment))
		.arg(QString::number(tInfo->nReceiveSegment))
		.arg(QString::number(tInfo->nCrcErrorSegment))
		.arg(QString::number(tInfo->nLostSegment))
		.arg(QString::number(tInfo->nReceiveStatus>>16));
	ui.textBrowser->setText(txt);
	m_Status = tInfo->nReceiveStatus & 0xffff;
	switch(tInfo->nReceiveStatus & 0xffff)
	{
	case 0:
// 		ui.label_38->setText(tr("Please power-off and take out the removeable disk."));
		ui.label_38->setText("");
		ui.label_10->setEnabled(true);
		ui.label_11->setEnabled(true);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		break;
	case 1:
		ui.label_38->setText("");
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(true);
		ui.label_13->setEnabled(true);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		break;
	case 2:
		ui.label_38->setText("");
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_21->setEnabled(true);
		ui.label_20->setEnabled(true);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		break;
	case 3:
		ui.label_38->setText("");
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_14->setEnabled(true);
		ui.label_15->setEnabled(true);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		break;
	case 5:
		ui.label_38->setText("");
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(true);
		ui.label_17->setEnabled(true);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		break;
	case 6:
		ui.label_38->setText("");
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_23->setEnabled(true);
		ui.label_22->setEnabled(true);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		break;
	case 7:
		ui.label_38->setText("");
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(true);
		ui.label_24->setEnabled(true);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		break;
	case 8:
		ui.label_38->setText("");
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(true);
		ui.label_26->setEnabled(true);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		break;
	case 9:
		ui.label_38->setText("");
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(true);
		ui.label_28->setEnabled(true);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		break;
	case 10:
		ui.label_38->setText("");
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(true);
		ui.label_30->setEnabled(true);
		ui.label_18->setEnabled(false);
		ui.label_7->setEnabled(false);
		break;
	case 11:
		ui.label_10->setEnabled(false);
		ui.label_11->setEnabled(false);
		ui.label_12->setEnabled(false);
		ui.label_13->setEnabled(false);
		ui.label_21->setEnabled(false);
		ui.label_20->setEnabled(false);
		ui.label_14->setEnabled(false);
		ui.label_15->setEnabled(false);
		ui.label_16->setEnabled(false);
		ui.label_17->setEnabled(false);
		ui.label_23->setEnabled(false);
		ui.label_22->setEnabled(false);
		ui.label_25->setEnabled(false);
		ui.label_24->setEnabled(false);
		ui.label_27->setEnabled(false);
		ui.label_26->setEnabled(false);
		ui.label_29->setEnabled(false);
		ui.label_28->setEnabled(false);
		ui.label_31->setEnabled(false);
		ui.label_30->setEnabled(false);
		ui.label_18->setEnabled(true);
		ui.label_7->setEnabled(true);
		if((tInfo->nFileID % 1000) >= 900)
			ui.label_38->setText(tr("Get update file. To update system, please reboot system."));
		else
		{
			ui.label_38->setText(tr("Please power-off and take out the removeable disk."));
			txt = QString(tr("<pre style=\"font-size: 14px; font-family:Book Antiqua\">Creator: %2\tIssuer: %3\tIssueDate: %4\nRound: %9\tTotal Segment: %5\tReceived Segment: %6\nCRC Error: %7\tLost Segment:%8</pre>"))
				.arg(creator)
				.arg(issuer)
				.arg(tInfo->strIssueDate.c_str())
				.arg(QString::number(tInfo->nTotalSegment))
				.arg(QString::number(tInfo->nReceiveSegment))
				.arg(0)
				.arg(0)
				.arg(QString::number(tInfo->nReceiveStatus>>16));
			ui.textBrowser->setText(txt);
			s.sprintf("%lld/%lld", tInfo->nFileLength, tInfo->nFileLength);
			ui.label_Receiver->setText(s);
		}
		
		break;
	}
	if (tInfo->strExtend.find("REMOTE:1") != std::string::npos)
		ui.label_40->setEnabled(true);
	else
		ui.label_40->setEnabled(false);
	if(tInfo->strExtend.find("REMOVEABLEDISK:0") != std::string::npos)
	{
		ui.label_38->setText(tr("Mount removeable disk error, please check disk."));
		((QTabWidget*)p)->setTabEnabled(1, false);
		((QTabWidget*)p)->setTabEnabled(2, false);
		((QTabWidget*)p)->setTabEnabled(3, false);
		ui.label->setEnabled(false);
		ui.label_4->setEnabled(false);
		ui.agc->setEnabled(false);
		ui.label_2->setEnabled(false);
		ui.progressBar_siganl_Strength->setEnabled(false);
		ui.progressBar_siganl_Quality->setEnabled(false);
		ui.groupBox_Filme->setEnabled(false);
	}
	ui.textBrowser->moveCursor(QTextCursor::End);
}