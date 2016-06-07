#ifndef DELCONFIRM_H
#define DELCONFIRM_H

#include <QDialog>
#include <QList>
#include <string>
#include "ui_delconfirm.h"

class QTcpSocket;
class ContentItem;

class DelConfirm : public QDialog
{
	Q_OBJECT

public:
	DelConfirm(QTcpSocket* pSocket, QWidget *parent = 0);
	~DelConfirm();

	void SetDeleteList(QList<ContentItem*> &raidList, QList<ContentItem*> &rdList);

private:
	void Init();
	void DeleteFilm(std::string path);

private:
	Ui::DelConfirmClass ui;
	QTcpSocket *pSocket;

private slots:
	void on_pushButtonDelete_clicked();
	void on_pushButtonCancel_clicked();
};

#endif // DELCONFIRM_H
