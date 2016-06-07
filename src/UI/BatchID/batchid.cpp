#include "batchid.h"
#include <QFileDialog>
#include <openssl/rand.h>

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QByteArray>
#include <QDomProcessingInstruction>
#include <QTextStream>

BatchID::BatchID(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

BatchID::~BatchID()
{

}

void BatchID::on_pushButtonBrowse_clicked()
{
	strBatchXml = QFileDialog::getOpenFileName(this, 
		tr("Open Batch XML"), 
		"./",
		tr("Batch files (*.xml)"));
	ui.lineEdit->setText(strBatchXml);
}

void BatchID::on_pushButtonBrowse_2_clicked()
{
	strOutDir = QFileDialog::getExistingDirectory(this,
		tr("Open Directory"),
		"./",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui.lineEdit_4->setText(strOutDir);
}

void BatchID::on_pushButton_clicked()
{
	int begin = ui.spinBox->value();
	int end = ui.spinBox_2->value();
	strBatchXml = ui.lineEdit->text();
	strOutDir = ui.lineEdit_4->text();

	if(strBatchXml == "")
		return;
	if(strOutDir == "")
		return;
	QFile file(strBatchXml);
	if(!file.open(QFile::ReadOnly|QFile::Text))
		return;
	QString errStr;
	int errLine;
	int errColumn;
	QDomDocument doc;
	if (!doc.setContent(&file, true, &errStr, &errLine, &errColumn))
	{
		file.close();
		return;
	}
	file.close();
	id_list.clear();
	QDomElement root = doc.documentElement();
	QDomNodeList list = root.elementsByTagName("Device");
	for (int i = 0; i < list.count(); i++)
	{
		QDomElement e = list.at(i).toElement();
		QDomElement t = e.firstChildElement("ID");
		_ID_ ID;
		ID.id = t.text().toLong();
		t = e.firstChildElement("Key");
		ID.key = t.text();
		id_list.append(ID);
	}
	for (int i = begin; i <= end; i++)
	{
		_ID_ id = id_list.at(i - 10060001);
		SaveShellScript(id);
	}
}

void BatchID::on_pushButtonBrowse_3_clicked()
{
	strSave = QFileDialog::getSaveFileName(this, tr("Save Batch XML"), "./", tr("Batch files (*.xml)"));
	ui.lineEdit_5->setText(strSave);
}

void BatchID::on_pushButton_2_clicked()
{
	strSave = ui.lineEdit_5->text();
	if(strSave == "")
		return;
	QDomDocument doc;
	QDomProcessingInstruction inst = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");
	doc.appendChild(inst);
	QDomElement l = doc.createElement("DeviceList");
	doc.appendChild(l);
	for(int i = 1; i <= 9999; i++)
	{
		QDomElement d = doc.createElement("Device");
		l.appendChild(d);

		QDomElement item = doc.createElement("ID");
		QDomText t = doc.createTextNode(QString::number(10060000+i));
		item.appendChild(t);
		d.appendChild(item);

		item = doc.createElement("Key");
		unsigned char k[16];
		RAND_pseudo_bytes(k, 16);
		QString s;
		s.sprintf("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7],
			k[8], k[9], k[10], k[11], k[12], k[13], k[14], k[15]);
		t = doc.createTextNode(s);
		item.appendChild(t);
		d.appendChild(item);
	}
	QFile savefile(strSave);
	savefile.open(QFile::ReadWrite|QFile::Text|QFile::Truncate);
	QTextStream out(&savefile);
	doc.save(out, 4);
}

void BatchID::SaveShellScript(_ID_ id)
{
	QString file;
	file.sprintf("%s/%d.sh", strOutDir.toStdString().c_str(), id.id);
	FILE *fp = fopen(file.toStdString().c_str(), "wb");
	QString s = "#!/bin/bash\n";
	fwrite(s.toStdString().c_str(), s.size(), 1, fp);
	s = "rm -rf /etc/CineCast/ID\n";
	fwrite(s.toStdString().c_str(), s.size(), 1, fp);
	s = "echo \"[ID_HardKey]\" > /etc/CineCast/ID\n";
	fwrite(s.toStdString().c_str(), s.size(), 1, fp);
	s.sprintf("echo \"ID=%d\" >> /etc/CineCast/ID\n", id.id);
	fwrite(s.toStdString().c_str(), s.size(), 1, fp);
	s.sprintf("echo \"HardKey=%s\" >> /etc/CineCast/ID\n", id.key.toStdString().c_str());
	fwrite(s.toStdString().c_str(), s.size(), 1, fp);
	s.sprintf("echo \"CineCast-%d\" > /etc/hostname", id.id);
	fwrite(s.toStdString().c_str(), s.size(), 1, fp);
	fclose(fp);
}