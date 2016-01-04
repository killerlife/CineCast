#ifndef CINECASTUI_H
#define CINECASTUI_H

#include <QtGui/QWidget>
#include <QMdiSubWindow>
#include <QtNetwork>
#include <QMessageBox>



#include "ui_cinecastui.h"

class Status;
class Setup;
class Disk;
class Content;
class Raid;
class Live;
class Tkr;

class CineCastUi : public QWidget
{
	Q_OBJECT

public:
	CineCastUi(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CineCastUi();

private:
	void Init();

private slots:
	void on_pushButton_Shutdown_clicked();     //	ػ
	void on_pushButton_Reboot_clicked();       //	  

	void on_tabWidget_currentChanged(int);
	void timerEvent(QTimerEvent * te);
	void Error(QAbstractSocket::SocketError socketError);
	void Connected();
	void UiFilter();

private:
	Ui::CineCastUiClass ui;
	Status *statusForm;
	Setup *setupForm;
	Disk *diskForm;
	Content *contentForm;
	Raid *raidForm;
	Live *liveForm;
	Tkr *tkrForm;

	QMdiSubWindow *statusWid;
	QMdiSubWindow *setupWid;
	QMdiSubWindow *diskWid;
	QMdiSubWindow *contentWid;
	QMdiSubWindow *raidWid;
	QMdiSubWindow *liveWid;
	QMdiSubWindow *tkrWid;

	QTcpSocket socket;
	int m_network_timer;
    int m_UpdateSatellite_timer;   //ʱѯ״̬ʱ
};

#endif // CINECASTUI_H
