#ifndef BATCHID_H
#define BATCHID_H

#include <QtGui/QDialog>
#include "ui_batchid.h"
#include <QList>

class _ID_
{
public:
	unsigned long id;
	QString key;
};

class BatchID : public QDialog
{
	Q_OBJECT

public:
	BatchID(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BatchID();

private:
	void SaveShellScript(_ID_ id);

private:
	Ui::BatchIDClass ui;

private slots:
	void on_pushButton_2_clicked();
	void on_pushButtonBrowse_3_clicked();
	void on_pushButton_clicked();
	void on_pushButtonBrowse_2_clicked();
	void on_pushButtonBrowse_clicked();

private:
	QString strBatchXml, strOutDir, strSave;
	QList<_ID_> id_list;
};

#endif // BATCHID_H
