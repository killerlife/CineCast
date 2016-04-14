#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QtGui>
#include <QtNetwork>
#include "my_struct.h"        //�Զ����������� 
#include "baseoperation.h"    //�Զ����������



#include <QThread>


//�Ƿ���Ըĳ��߳���
class TcpClientSocket : public QTcpSocket//,public QThread
{
	Q_OBJECT

		//new
public:
	void run();

public:
	TcpClientSocket(int socketDescriptor,QObject *parent = 0);
	~TcpClientSocket();
	void crc_encry_send(char* buf,int size,int val);   //���CRC�ֶ�,Ȼ����ܷ��� pos��ʾ����ƫ��

public:
	AuthenREQUEST AuthenRequest;     //���浱ǰ���ӵ���֤������
	UpdateFileBACK UpdateFileBack;   //���浱ǰ���ӵ������ļ�����
	UpdateFileINFO  UpdateFileInfo;  //�ݴ��������ļ���Ϣ,������������ʱ���õ���������к�


	//����������   
	void login_request_process();         //��¼����  ����     
	void authon_request_process();        //��֤����     
	void update_file_back_process();      //�����ļ��������ͷ��� ����               
	void log_back_process();               //��־���Ļش�,�ļ����ܱȽϴ�,���⴦��
	void log_process();
	void heart_breat_process();
	void heart_breat_request();
	void md5_process();
	void lost_report_process();
	void md5_res_process();


	void do_send_file();                   //�����ļ�������
	void do_update_start();                //������ջ�,��������
	void do_get_Log();                     //�ֶ���ȡ��־
	void do_get_md5res();
	void do_reboot();
	void do_Log();
	QHostAddress getRemote();

	//������֤��Ϣ,�Ͽ�����ʱҪɾ��
	//    QList<AuthenREQUEST> QListAuthenRequest;    //�ŵ�TcpServer

	//������server�˱�����Ѿ�ע��Ľ��ջ�ID����Ӳ��ָ��
	QList<RegisterIDHARDKEY> ListRegisterIDHardKey;

	char buf[1500000];                 //ȫ�����ݻ���--�ŵ�Socket��������,��ֹ���̷߳��ʳ�ͻ
	bool isblocking;                   //�����������գ�����datereceive�������أ������߼�����
	char c_buf[500];
	char *v_buf;
	char* m_buf;

	int  flag_update;   //���̵߳�����״̬,δ��ʼ0,����ִ������,1 �������2,
	char MeetKey[16];   //�ݴ浱ǰ���ӵĻ�����Կ
	char HardKey[16];   //�ݴ浱ǰ���Ӷ�Ӧ���ջ���Ӳ����Կ
	char RandomCode_tmp[16];

	//�����������źŴ��νṹ��,�ٸĽ�,ע�������ͻ��ȫ�������SL�ṹ�塣
	char pathname_updatefile[256];    //�������ļ�·��
	LOGREQUEST LOGRequest;
	
	QString strLog;
	QString strRemote;

signals:
	void signal_socketDataReady(int socketID);
	void signal_socketDisConnect(int socketID,TcpClientSocket *tcpClientSocket);  //ͬʱ���ݵ�ǰ����ָ��,
	void signal_updateClients(QString,int);      //�����½�仯
	void signal_CMD_SocketToUI(int SocketID,int cmdtype,int val);   //socket�̷߳��͸�UI������

private slots:
	void slot_dataReceived();      
	void slot_Disconnected();

public slots:
	void UIcmd_process(int SocketID,int cmdtype,QByteArray ba);   //	����UI������������
	void Error(QAbstractSocket::SocketError socketError);

public:
	int socketID;//����id��== this->socketDescriptor()������this->socketDescriptor()�ͻ��˶Ͽ��ᱻ�ͷţ�
	//�Ͽ��ź���this->socketDescriptor()�ò�����ȷֵ
	void SaveProto(char* buf);
};

#endif 
