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
	ui.textBrowser->setStyleSheet("QTextBrowser{font-size: 18px; font-family:'Book Antiqua';}");
	ui.textBrowser->setFocusPolicy(Qt::NoFocus);

	ui.label_10->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_12->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_14->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_16->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_18->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_20->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_22->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_24->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_26->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_28->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_30->setStyleSheet("QLabel{font-size: 20px; font-family:'Book Antiqua';}");
	ui.label_38->setStyleSheet("QLabel{font-size: 40px; font-family:'Book Antiqua'; color:#0000FF;}");
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
#if 0
	txt = QString(tr("Film Name: %1<br>Creator: %3<br>Issuer: %4<br>IssueDate: %5<br>RecvRate: %2 MB/S<pre style=\"font-size: 18px; font-family:Book Antiqua\">Round: %10\tTotal Segment: %6\tReceived Segment: %7\tCRC Error: %8\tLost Segment:%9</pre><br>"))
		.arg(tInfo->strFilmName.c_str())
		.arg(sRate)
		.arg(creator)
		.arg(issuer)
		.arg(tInfo->strIssueDate.c_str())
		.arg(QString::number(tInfo->nTotalSegment))
		.arg(QString::number(tInfo->nReceiveSegment))
		.arg(QString::number(tInfo->nCrcErrorSegment))
		.arg(QString::number(tInfo->nLostSegment))
		.arg(QString::number(tInfo->nReceiveStatus>>16));
#else
	txt = QString(tr("Film Name: %1<br>Creator: %3<br>Issuer: %4<br>IssueDate: %5<br>UUID: %2<pre style=\"font-size: 18px; font-family:Book Antiqua\">Round: %10\tTotal Segment: %6\tReceived Segment: %7\tCRC Error: %8\tLost Segment:%9</pre><br>"))
		.arg(tInfo->strFilmName.c_str())
		.arg(tInfo->strUuid.c_str())
		.arg(creator)
		.arg(issuer)
		.arg(tInfo->strIssueDate.c_str())
		.arg(QString::number(tInfo->nTotalSegment))
		.arg(QString::number(tInfo->nReceiveSegment))
		.arg(QString::number(tInfo->nCrcErrorSegment))
		.arg(QString::number(tInfo->nLostSegment))
		.arg(QString::number(tInfo->nReceiveStatus>>16));
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
		ui.label_38->setText(tr("Please power-off and take out the removeable disk."));
#if 0
			txt = QString(tr("Film Name: %1<br>Creator: %3<br>Issuer: %4<br>IssueDate: %5<br>RecvRate: %2 MB/S<pre style=\"font-size: 18px; font-family:Book Antiqua\">Round: %10\tTotal Segment: %6\tReceived Segment: %7\tCRC Error: %8\tLost Segment:%9</pre><br>"))
				.arg(tInfo->strFilmName.c_str())
				.arg(sRate)
				.arg(creator)
				.arg(issuer)
				.arg(tInfo->strIssueDate.c_str())
				.arg(QString::number(tInfo->nTotalSegment))
				.arg(QString::number(tInfo->nReceiveSegment))
				.arg(0)
				.arg(0)
				.arg(QString::number(tInfo->nReceiveStatus>>16));
#else
			txt = QString(tr("Film Name: %1<br>Creator: %3<br>Issuer: %4<br>IssueDate: %5<br>UUID: %2<pre style=\"font-size: 18px; font-family:Book Antiqua\">Round: %10\tTotal Segment: %6\tReceived Segment: %7\tCRC Error: %8\tLost Segment:%9</pre><br>"))
				.arg(tInfo->strFilmName.c_str())
				.arg(tInfo->strUuid.c_str())
				.arg(creator)
				.arg(issuer)
				.arg(tInfo->strIssueDate.c_str())
				.arg(QString::number(tInfo->nTotalSegment))
				.arg(QString::number(tInfo->nReceiveSegment))
				.arg(0)
				.arg(0)
				.arg(QString::number(tInfo->nReceiveStatus>>16));
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
		ui.label_38->setStyleSheet("QLabel{font-size: 40px; font-family:'Book Antiqua'; color:#FF2222;}");
		ui.label_38->setText(tr("Mount removeable disk error, please check disk."));
		((QTabWidget*)p)->setTabEnabled(1, false);
		((QTabWidget*)p)->setTabEnabled(2, false);
		((QTabWidget*)p)->setTabEnabled(3, false);
		ui.groupBox_satellite->setEnabled(false);
		ui.groupBox_Filme->setEnabled(false);
	}
}