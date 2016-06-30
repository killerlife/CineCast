#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QtGui>
#include <QtNetwork>
#include "my_struct.h"        //Զ 
#include "baseoperation.h"    //Զ



#include <QThread>


//ǷԸĳ߳
class TcpClientSocket : public QTcpSocket//,public QThread
{
	Q_OBJECT

		//new
public:
	void run();

public:
	TcpClientSocket(int socketDescriptor,QObject *parent = 0);
	~TcpClientSocket();
	void crc_encry_send(char* buf,int size,int val);   //CRCֶ,Ȼܷ posʾƫ

public:
	AuthenREQUEST AuthenRequest;     //浱ǰӵ֤
	UpdateFileBACK UpdateFileBack;   //浱ǰӵļ
	UpdateFileINFO  UpdateFileInfo;  //ݴļϢ,ʱõк


	//   
	void login_request_process();         //¼       
	void authon_request_process();        //֤     
	void update_file_back_process();      //ļͷ                
	void log_back_process();               //־Ļش,ļܱȽϴ,⴦
	void log_process();
	void heart_breat_process();
	void heart_breat_request();
	void md5_process();
	void lost_report_process();
	void md5_res_process();


	void do_send_file();                   //ļ
	void do_update_start();                //ջ,
	void do_get_Log();                     //ֶȡ־
	void do_get_md5res();
	void do_reboot();
	void do_Log();
	QHostAddress getRemote();

	//֤Ϣ,ϿʱҪɾ
	//    QList<AuthenREQUEST> QListAuthenRequest;    //ŵTcpServer

	//server˱ѾעĽջIDӲָ
	QList<RegisterIDHARDKEY> ListRegisterIDHardKey;

	char buf[1500000];                 //ȫݻ--ŵSocket,̷ֹ߳ʳͻ
	bool isblocking;                   //գdatereceiveأ߼
	char c_buf[500];
	char *v_buf;
	char* m_buf;

	int  flag_update;   //̵߳״̬,δʼ0,ִ,1 2,
	char MeetKey[20];   //ݴ浱ǰӵĻԿ
	char HardKey[20];   //ݴ浱ǰӶӦջӲԿ
	char RandomCode_tmp[20];

	//źŴνṹ,ٸĽ,עͻȫSLṹ塣
	char pathname_updatefile[256];    //ļ·
	LOGREQUEST LOGRequest;
	
	QString strLog;
	QString strRemote;

signals:
	void signal_socketDataReady(int socketID);
	void signal_socketDisConnect(int socketID,TcpClientSocket *tcpClientSocket);  //ͬʱݵǰָ,
	void signal_updateClients(QString,int);      //½仯
	void signal_CMD_SocketToUI(int SocketID,int cmdtype,int val);   //socket̷߳͸UI

private slots:
	void slot_dataReceived();      
	void slot_Disconnected();

public slots:
	void UIcmd_process(int SocketID,int cmdtype,QByteArray ba);   //	UI
	void Error(QAbstractSocket::SocketError socketError);

public:
	int socketID;//id== this->socketDescriptor()this->socketDescriptor()ͻ˶Ͽᱻͷţ
	//Ͽźthis->socketDescriptor()òȷֵ
	void SaveProto(char* buf);
};

#endif 
