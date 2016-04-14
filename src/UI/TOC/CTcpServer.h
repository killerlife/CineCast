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
	int port;    //最好保存当前QTcpServer的端口
    QList<TcpClientSocket *> tcpClientSocketList;   //外部访问，获取IP地址
    QList<int> ClientSocketID;

protected:
    void incomingConnection(int socketDescriptor);
    

 	//OLD
signals:
    void signal_socketDataReady(int socketID);
    void signal_clientChange();   //客户端有增减,断开或新连接,发送信号，通知界面,刷新客户端信息列表
    void signal_updateServerUI(QString,int);

    void signal_CMD_TcpServertoUI(int SocketID,int cmdtype); //服务器发给UI


    void UIcmd_toSocket(int SocketID,int cmdtype,QByteArray ba);   //处理UI发过来的命令中转

	void signal_CMD_SocketToUI(int SocketID,int cmdtype,int val);   //信号中转

public slots:
    void slot_updateClients(QString,int);
    void slot_ReadData(int socketID);
    void slot_DisConnect(int socketID,TcpClientSocket *tcpClientSocket);
};


#endif
