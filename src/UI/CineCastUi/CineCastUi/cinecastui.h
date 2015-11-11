#ifndef CINECASTUI_H
#define CINECASTUI_H

#include <QtGui/QWidget>
#include <QMdiSubWindow>
#include <QtNetwork>
#include "ui_cinecastui.h"

class Status;
class Setup;

class CineCastUi : public QWidget
{
	Q_OBJECT

public:
	CineCastUi(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CineCastUi();

private:
	void Init();

private slots:
	void on_tabWidget_currentChanged(int);
	void timerEvent(QTimerEvent * te);
	void Error(QAbstractSocket::SocketError socketError);
	void Connected();
	void UiFilter();

private:
	Ui::CineCastUiClass ui;
	Status *statusForm;
	Setup *setupForm;
	QMdiSubWindow *statusWid;
	QMdiSubWindow *setupWid;
	QTcpSocket socket;
	int m_timer;
};

#endif // CINECASTUI_H
