#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include "tcpclientsocket.h"
#include <QHostAddress>
#include <QThread>



class CTcpServer : public QTcpServer		
{
    Q_OBJECT

//NEW
public:
    explicit CTcpServer(QObject *parent = 0,int port=0);
    void SendData(int socketID,QString IP,int Port,QByteArray data);

public:
	int port;    //��ñ��浱ǰQTcpServer�Ķ˿�
    QList<TcpClientSocket *> tcpClientSocketList;   //�ⲿ���ʣ���ȡIP��ַ
    QList<int> ClientSocketID;

protected:
    void incomingConnection(int socketDescriptor);
    

 	//OLD
signals:
    void signal_socketDataReady(int socketID);
    void signal_clientChange();   //�ͻ���������,�Ͽ���������,�����źţ�֪ͨ����,ˢ�¿ͻ�����Ϣ�б�
    void signal_updateServerUI(QString,int);

    void signal_CMD_TcpServertoUI(int SocketID,int cmdtype); //����������UI


    void UIcmd_toSocket(int SocketID,int cmdtype,QByteArray ba);   //����UI��������������ת

	void signal_CMD_SocketToUI(int SocketID,int cmdtype,int val);   //�ź���ת

public slots:
    void slot_updateClients(QString,int);
    void slot_ReadData(int socketID);
    void slot_DisConnect(int socketID,TcpClientSocket *tcpClientSocket);
};


#endif
