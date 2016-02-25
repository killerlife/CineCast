#ifndef COPYFILM_H
#define COPYFILM_H

#include <QDialog>
#include <QList>
#include <string>
#include "../../../dvb/mytype.h"
#include "ui_copyfilm.h"

class QTcpSocket;
class ContentItem;

class CopyFilm : public QDialog
{
	Q_OBJECT

public:
	CopyFilm(QTcpSocket* pSocket, QWidget *parent = 0);
	~CopyFilm();
	void SetCopyList(QList<ContentItem*> &clist, std::string strDest);

public slots:
	void reject();
	void accept();

private:
	void Init();
	void CopyNextFilm();
	void CopyProgress();

private:
	Ui::CopyFilmClass ui;
	QTcpSocket *pSocket;
	std::string strDest;
	int mTimer;
	bool bCopy;
	bool bRunNew;
	int nCurrent;
	int nTotal;
	uint64 lenTotal, lenCurrent;

private slots:
	void on_pushButton_clicked();
	void timerEvent(QTimerEvent * te);
};

#endif // COPYFILM_H
