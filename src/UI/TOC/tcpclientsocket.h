#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QtGui>
#include <QtNetwork>
#include "my_struct.h"        //自定义数据类型 
#include "baseoperation.h"    //自定义基本操作



#include <QThread>


//是否可以改成线程类
class TcpClientSocket : public QTcpSocket//,public QThread
{
	Q_OBJECT

		//new
public:
	void run();

public:
	TcpClientSocket(int socketDescriptor,QObject *parent = 0);
	~TcpClientSocket();
	void crc_encry_send(char* buf,int size,int val);   //填充CRC字段,然后加密发送 pos表示加密偏移

public:
	AuthenREQUEST AuthenRequest;     //保存当前连接的认证请求报文
	UpdateFileBACK UpdateFileBack;   //保存当前连接的升级文件反馈
	UpdateFileINFO  UpdateFileInfo;  //暂存升级包文件信息,发送升级命令时候用到里面的序列号


	//各种请求处理   
	void login_request_process();         //登录请求  处理     
	void authon_request_process();        //认证请求     
	void update_file_back_process();      //升级文件包推送送反馈 处理               
	void log_back_process();               //日志报文回传,文件可能比较大,特殊处理
	void log_process();
	void heart_breat_process();
	void heart_breat_request();
	void md5_process();
	void lost_report_process();
	void md5_res_process();


	void do_send_file();                   //升级文件包推送
	void do_update_start();                //命令接收机,可以升级
	void do_get_Log();                     //手动获取日志
	void do_get_md5res();
	void do_reboot();
	void do_Log();
	QHostAddress getRemote();

	//保存认证信息,断开连接时要删除
	//    QList<AuthenREQUEST> QListAuthenRequest;    //放到TcpServer

	//服务器server端保存的已经注册的接收机ID及其硬件指纹
	QList<RegisterIDHARDKEY> ListRegisterIDHardKey;

	char buf[1500000];                 //全局数据缓存--放到Socket对象里面,防止多线程访问冲突
	bool isblocking;                   //正在阻塞接收，触发datereceive立即返回，否则逻辑混乱
	char c_buf[500];
	char *v_buf;
	char* m_buf;

	int  flag_update;   //本线程的升级状态,未开始0,正在执行升级,1 升级完成2,
	char MeetKey[16];   //暂存当前链接的会晤密钥
	char HardKey[16];   //暂存当前链接对应接收机的硬件密钥
	char RandomCode_tmp[16];

	//几个不方便信号传参结构体,再改进,注册新类型或改全局数组加SL结构体。
	char pathname_updatefile[256];    //升级包文件路径
	LOGREQUEST LOGRequest;
	
	QString strLog;
	QString strRemote;

signals:
	void signal_socketDataReady(int socketID);
	void signal_socketDisConnect(int socketID,TcpClientSocket *tcpClientSocket);  //同时传递当前对象指针,
	void signal_updateClients(QString,int);      //处理登陆变化
	void signal_CMD_SocketToUI(int SocketID,int cmdtype,int val);   //socket线程发送给UI的命令

private slots:
	void slot_dataReceived();      
	void slot_Disconnected();

public slots:
	void UIcmd_process(int SocketID,int cmdtype,QByteArray ba);   //	处理UI发过来的命令
	void Error(QAbstractSocket::SocketError socketError);

public:
	int socketID;//保存id，== this->socketDescriptor()；但是this->socketDescriptor()客户端断开会被释放，
	//断开信号用this->socketDescriptor()得不到正确值
	void SaveProto(char* buf);
};

#endif 
