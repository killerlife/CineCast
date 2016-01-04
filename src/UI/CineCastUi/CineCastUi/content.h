#ifndef CONTENT_H
#define CONTENT_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>


#include "ui_content.h"


enum{
	C_ID = 0,
	C_NAME,
	C_PROGRESS,
	C_STATUS,
	C_FORMAT,
	C_FILM_LENGTH,
	C_PROGRAM_LENGTH,
	C_STEREO,
	C_ISSUER,
	C_ISSUEDATE,
	C_TIMERANGE,
	C_RECVSEGMENT,
	C_TOTAL_SEGMENT,
	C_RECV_DATETIME,
	C_LOCATE,
};


struct InfoData 
{
public:
	std::string  pData[16];
/*    ʾڽϵϸϢ
"Film ID:",  0
"Ŀ:", 1
"ɶ:", 2
"״̬:", 3
"Ƶʽ:", 4
"ļ:", 5
"Ŀ:", 6 
"2D/3D:", 7
":", 8
	":",
	"ڵĿʼʱ:"T"ڵĽʱ:",
	":",
	"йҡ:",
"Օrg:",
"·",
*/
}; 





class ContentItem : public QTreeWidgetItem, public InfoData
{
public:
	ContentItem(int type = Type):QTreeWidgetItem(type)
	{};
	~ContentItem(){};
	ContentItem& operator = (ContentItem& item);

	void MakeItem_HDD(void);
    void MakeItem_USB(void);   //new


	void UpdateState();
};

class QTcpSocket;
class Content : public QWidget
{
	Q_OBJECT

public:
	Content(QTcpSocket* pSocket, QWidget *parent = 0);
	~Content();

	void UpdateContent_HDD();      //б,¼شݣѯ״̬,ȡ3
	void UpdateContent_USB();

	void ContentisReady_HDD();     //Ƿ׼,׼ֱӵLoadContent_HDD 
	void ContentisReady_USB();

	void LoadContent_HDD();
	void LoadContent_USB();

    int getCopyProgress();   //ʱѯļ

	void UpdateContentUI();   //ͻ,ڿ,Ҫ½



private:
    ContentItem* item_HDD[100];    //ٸʵ֣ӰƬļб
    ContentItem* item_USB[100];    //ٸʵ

	int m_query_timer;      //ʱѯ
	int m_isReady_timer_HDD;              //ʱѯǷ׼
	int m_isReady_timer_USB;              //ʱѯǷ׼

	Ui::ContentClass ui;
	QTcpSocket *pSocket;

private:
	void Init();

private slots:
	void on_pushButton_Refush_Disk_clicked();
	void on_progressBar_CopyDir_valueChanged(int);

	void on_pushButton_DeleteDir_USB_clicked();
	void on_pushButton_DeleteDir_HDD_clicked();
	void on_pushButton_ExporttoUSB_clicked();

	void on_treeWidget_HDD_info_clicked(const QModelIndex &);
	void on_treeWidget_USB_info_clicked(const QModelIndex &);

//	void on_pushButton_USB_Mount_clicked();
    void on_pushButton_USB_UnMount_clicked();

	void timerEvent(QTimerEvent * te);

};

#endif // CONTENT_H
