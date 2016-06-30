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
	ui.label_Receiver_2->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.label_filmName->setStyleSheet("QLabel{font-size: 14px; font-family:'Book Antiqua';}");
	ui.progressBar_siganl_Strength->setStyleSheet("QProgressBar{font-size: 14px; font-family:'Book Antiqua';}");
	ui.progressBar_siganl_Quality->setStyleSheet("QProgressBar{font-size: 14px; font-family:'Book Antiqua';}");
	ui.progressBar_Revceiver_length->setStyleSheet("QProgressBar{font-size: 14px; font-family:'Book Antiqua';}");
	ui.textBrowser->setStyleSheet("QTextBrowser{font-size: 14px; font-family:'Book Antiqua';}");
	ui.textBrowser->setFocusPolicy(Qt::NoFocus);

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
	s.sprintf("%lld", tInfo->nReceiveLength);
	int n = s.size();
	for(int i = 1; i <= (n-1)/3; i++)
	{
		s.insert(n - i * 3, ",");
	}
	QString dd = tr(" Bytes");
	QString ss = s + dd;
	s.sprintf("%lld", tInfo->nFileLength);
	n = s.size();
	for(int i = 1; i <= (n-1)/3; i++)
	{
		s.insert(n - i * 3, ",");
	}
	ss += " / ";
	ss += s;
	ss += dd;
	ui.label_Receiver->setText(ss);
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
	QString sRate;
	size_t pos;
	if ((pos = tInfo->strExtend.find("RATE:")) != std::string::npos)
	{
		sRate = tInfo->strExtend.c_str() + pos + 5;
		QStringList slist = sRate.split("|");
		sRate = slist.at(0);
	}
	else
		sRate = "0";

	QString round = QString::number(tInfo->nReceiveStatus>>16);
	QString crc = QString::number(tInfo->nCrcErrorSegment);
	QString total = QString::number(tInfo->nTotalSegment);
	QString recvs = QString::number(tInfo->nReceiveSegment);
	if(creator.isEmpty())
	{
		creator = "\t";
#ifdef WIN32
 		round += "\t";
#endif
		issuer += "\t";
#ifdef WIN32
		crc += "\t";
#endif
		total += "\t";
	}
#ifdef WIN32
	if((creator.toLocal8Bit().size() - round.toLocal8Bit().size()) >= 3)
		round += "\t";
#endif
	if((issuer.toLocal8Bit().size() - total.toLocal8Bit().size()) > 4)
	{
		total += "\t";
//		crc += "\t";
	}
	else if((total.toLocal8Bit().size() - issuer.toLocal8Bit().size()) >= 3)
	{
		issuer += "\t";
	}
#ifdef WIN32
	if((total.size() - crc.size()) > 4)
		crc += "\t";
#endif
#if 0
	txt = QString(tr("<pre style=\"font-size: 14px; font-family:Book Antiqua\">Creator: %2\tIssuer: %3\tIssueDate: %4\nRound: %9\tTotal Segment: %5\tReceived Segment: %6\nRecvRate: %10 MB/S\tCRC Error: %7\tLost Segment:%8</pre>"))
		.arg(creator)
		.arg(issuer)
		.arg(tInfo->strIssueDate.c_str())
		.arg(total)
		.arg(recvs)
		.arg(crc)
		.arg(QString::number(tInfo->nLostSegment))
		.arg(round)
		.arg(sRate);
#else
	round += "\t";
	crc += "\t";
	txt = QString(tr("<pre style=\"font-size: 14px; font-family:Book Antiqua\">Creator: %2\tIssuer: %3\tIssueDate: %4\nRound: %9\tTotal Segment: %5\tReceived Segment: %6\tCRC Error: %7\tLost Segment:%8</pre>"))
		.arg(creator)
		.arg(issuer)
		.arg(tInfo->strIssueDate.c_str())
		.arg(total)
		.arg(recvs)
		.arg(crc)
		.arg(QString::number(tInfo->nLostSegment))
		.arg(round);
#endif
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
			QString round = QString::number(tInfo->nReceiveStatus>>16);
			QString crc = "0";
			QString total = QString::number(tInfo->nTotalSegment);
			QString recvs = QString::number(tInfo->nReceiveSegment);
#ifdef WIN32
			if((creator.toLocal8Bit().size() - round.toLocal8Bit().size()) >= 3)
				round += "\t";
#endif
			if((issuer.toLocal8Bit().size() - total.toLocal8Bit().size()) > 4)
			{
				total += "\t";
				//		crc += "\t";
			}
			else if((total.toLocal8Bit().size() - issuer.toLocal8Bit().size()) >= 3)
			{
				issuer += "\t";
			}
#ifdef WIN32
			if((total.size() - crc.size()) > 4)
				crc += "\t";
#endif
			ui.label_38->setText(tr("Please power-off and take out the removeable disk."));
#if 0
			txt = QString(tr("<pre style=\"font-size: 14px; font-family:Book Antiqua\">Creator: %2\tIssuer: %3\tIssueDate: %4\nRound: %9\tTotal Segment: %5\tReceived Segment: %6\nRecvRate: %10 MB/S\tCRC Error: %7\tLost Segment:%8</pre>"))
				.arg(creator)
				.arg(issuer)
				.arg(tInfo->strIssueDate.c_str())
				.arg(total)
				.arg(recvs)
				.arg(crc)
				.arg(0)
				.arg(round)
				.arg(sRate);
#else
			round += "\t";
			crc += "\t";
			txt = QString(tr("<pre style=\"font-size: 14px; font-family:Book Antiqua\">Creator: %2\tIssuer: %3\tIssueDate: %4\nRound: %9\tTotal Segment: %5\tReceived Segment: %6\tCRC Error: %7\tLost Segment:%8</pre>"))
				.arg(creator)
				.arg(issuer)
				.arg(tInfo->strIssueDate.c_str())
				.arg(total)
				.arg(recvs)
				.arg(crc)
				.arg(0)
				.arg(round);
#endif
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