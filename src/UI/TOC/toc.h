#ifndef TOC_H
#define TOC_H

#include <QtGui/QWidget>
#include "my_struct.h"
#include "ui_toc.h"

class CTcpServer;

enum{
	C_ID = 0,
	C_NAME,
	C_PROGRESS,
	C_STATUS,
	C_FORMAT,
	C_FILM_LENGTH,
	C_PROGRAM_LENGTH,
	C_STEREO,
	C_ISSUER
};

class TcpClientSocket;

struct InfoData
{
public:
	//	std::string  pData[10];
	//	char  pData[10][256];
	QString  pData[10];
};
class ContentItem : public QTreeWidgetItem, public InfoData
{
public:
	ContentItem(int type = Type):QTreeWidgetItem(type){};
	~ContentItem(){};
	ContentItem& operator = (ContentItem& item);
	void MakeItem(void);
	void UpdateState();

	AuthenREQUEST AuthenRequest;         //���浱ǰ���ӵ���֤������
	TcpClientSocket *tcpClientSocket;    //����һ������ָ��,ͨ������ȡ�ͻ���IP �˿� �û�����

};

class TOC : public QWidget
{
	Q_OBJECT

public:
	TOC(QWidget *parent = 0, Qt::WFlags flags = 0);
	~TOC();

private:
	Ui::TOCClass ui;

public slots:
// 	void slot_updateServerUI(QString,int);
// 	void updateClientList();         //���µ�����������߶Ͽ�,���¿ͻ����б���Ϣ
	void slot_CMD_SocketToUI_process(int SocketID,int cmdtype,int val);   //socket�������������
	void slot_CMD_TcpServertoUI_process(int SocketID,int cmdtype);        //����������UI

signals:
	void UIcmd_toSocket(int SocketID,int cmdtype,QByteArray ba);   //���������Socket ��ҪCTcpServer��ת

public:
	void Init(void);
	void StartServer(void);
	void do_updateListOfClient();  

private:
	CTcpServer *server_login;	    //������˿�
	CTcpServer *server_main;	    //��½��������˿�

private slots:
	void on_pushButtonProtocol_clicked();
	void on_pushButtonCancel_clicked();
	void on_pushButtonFinish_clicked();
	void on_pushButtonData_clicked();
	void on_pushButtonPMT_clicked();
	void on_pushButtonPAT_clicked();
	void on_pushButtonStart_clicked();
	void on_pushButtonNotify_clicked();
	void on_pushButtonNext_clicked();
	void on_pushButtonGetFilmInfo_clicked();
	void on_pushButtonDelFilm_clicked();
	void on_pushButtonGetFilmList_clicked();
	void on_pushButtonReboot_clicked();
	void on_pushButtonLog_clicked();
	void on_pushButtonMD5_clicked();

private:
	QString printfn(QString fn);
	QString print_hex2(char* buf, int len);
	QString print_kh(char* buf, int len);
	void decrypt(char* buf, int len);
	QString print_crc(char *buf, int len);
	QString print_proto(char *buf, int len);
};

#endif // TOC_H