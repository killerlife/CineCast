#include <QtNetwork>
#include "CTcpServer.h"
#include <QDebug>



CTcpServer::CTcpServer(QObject *parent,int port)
    : QTcpServer(parent)
{
    this->port=port;                   //保存传递进来的端口，
	
    //可以制定监听某个IP 与端口
    if(!listen(QHostAddress::Any,port))
    {
        this->close();
		printf("CTcpServer::CTcpServer():listen error");
		return;
    }

    setMaxPendingConnections(30);
    //设定待处理的连接最大数目为(默认30),当超过了最大连接数后，客户端仍旧可以连接服务端，但是服务端不在接受连接，操作系统会把这些链接保存在一个队列中。
}



//新的连接到来
void CTcpServer::incomingConnection(int socketDescriptor)
{
	//继承自QTcpSocket()
	TcpClientSocket *tcpClientSocket = new TcpClientSocket(socketDescriptor);  //this,
    QThread* thread = new QThread(tcpClientSocket); 

    //可以进一步修改mei来多少个请求才创建一个线程
	static int NumThread=0;

	//获取连接的IP地址,用户名,端口方法,QTcpSocket()指针获取;
	qDebug()<<"perr:"<<tcpClientSocket->peerAddress().toString();
    qDebug()<<"name:"<<tcpClientSocket->peerName();
    qDebug()<<"port:"<<tcpClientSocket->peerPort();

    connect(tcpClientSocket,SIGNAL(signal_updateClients(QString,int)),this,SLOT(slot_updateClients(QString,int)));
    connect(tcpClientSocket,SIGNAL(signal_socketDisConnect(int,TcpClientSocket*)),this,SLOT(slot_DisConnect(int,TcpClientSocket*)));

    //关联CTcpServer中转的信号,操作socket线程 
	connect(this,SIGNAL(UIcmd_toSocket(int,int,QByteArray)),tcpClientSocket,SLOT(UIcmd_process(int,int,QByteArray)));

    //关联CTcpServer中转的信号  中转来着socket线程的信号, 交给UI处理
	connect(tcpClientSocket,SIGNAL(signal_CMD_SocketToUI(int,int,int)),this,SIGNAL(signal_CMD_SocketToUI(int,int,int)));

	//要想moveToThread,对象创建时不能有父指针
    tcpClientSocket->moveToThread(thread); //把tcp类移动到新的线程
    thread->start();//线程开始运行

	tcpClientSocketList.append(tcpClientSocket);

    printf("tcpClientSocketList.count=%d\n",tcpClientSocketList.count());

	//可删
    ClientSocketID.append(socketDescriptor);  //把ID号也保存

	//在此可以更新登录的IP地址列表
    //改成认证请求成功后,在UI里面显示客户端列表

    emit signal_clientChange();   //客户端有增减,断开或新连接,发送信号，通知界面,刷新客户端信息列表
}

void CTcpServer::slot_updateClients(QString msg,int length)
{
	//emit signal_updateServerUI(msg,length);
	emit signal_clientChange();  
}


void CTcpServer::slot_ReadData(int socketID)
{
    emit signal_socketDataReady(socketID);
}

void CTcpServer::slot_DisConnect(int SocketID,TcpClientSocket *tcpClientSocket)
{

	
    for (int i=0;i<ClientSocketID.count();i++)
    {
        if (ClientSocketID[i]==SocketID)
        {
            ClientSocketID.removeAt(i);    //从列表中移除活跃的socketID
			 break;
        }
    }
	
	    for (int i=0;i<tcpClientSocketList.count();i++)
    {
        if (tcpClientSocketList[i]==tcpClientSocket)
        {
            tcpClientSocketList.removeAt(i);    //从列表中移除该连接
            break;
        }
    }
	emit signal_clientChange();   //客户端有增减,断开或新连接,发送信号，通知界面,刷新客户端信息列表
  
    emit signal_CMD_TcpServertoUI(SocketID,SV_SOCKET_DISCONNECT); //服务器发给UI
}

//指定客户端连接发消息
void CTcpServer::SendData(int socketID,QString IP,int Port,QByteArray data)
{
    for (int i=0;i<ClientSocketID.count();i++)
    {
        if (ClientSocketID[i]==socketID)
        {
           tcpClientSocketList[i]->write(data);
            break;
        }
    }
}
