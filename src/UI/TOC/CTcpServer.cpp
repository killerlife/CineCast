#include <QtNetwork>
#include "CTcpServer.h"
#include <QDebug>



CTcpServer::CTcpServer(QObject *parent,int port)
    : QTcpServer(parent)
{
    this->port=port;                   //���洫�ݽ����Ķ˿ڣ�
	
    //�����ƶ�����ĳ��IP ��˿�
    if(!listen(QHostAddress::Any,port))
    {
        this->close();
		printf("CTcpServer::CTcpServer():listen error");
		return;
    }

    setMaxPendingConnections(30);
    //�趨����������������ĿΪ(Ĭ��30),������������������󣬿ͻ����Ծɿ������ӷ���ˣ����Ƿ���˲��ڽ������ӣ�����ϵͳ�����Щ���ӱ�����һ�������С�
}



//�µ����ӵ���
void CTcpServer::incomingConnection(int socketDescriptor)
{
	//�̳���QTcpSocket()
	TcpClientSocket *tcpClientSocket = new TcpClientSocket(socketDescriptor);  //this,
    QThread* thread = new QThread(tcpClientSocket); 

    //���Խ�һ���޸�mei�����ٸ�����Ŵ���һ���߳�
	static int NumThread=0;

	//��ȡ���ӵ�IP��ַ,�û���,�˿ڷ���,QTcpSocket()ָ���ȡ;
	qDebug()<<"perr:"<<tcpClientSocket->peerAddress().toString();
    qDebug()<<"name:"<<tcpClientSocket->peerName();
    qDebug()<<"port:"<<tcpClientSocket->peerPort();

    connect(tcpClientSocket,SIGNAL(signal_updateClients(QString,int)),this,SLOT(slot_updateClients(QString,int)));
    connect(tcpClientSocket,SIGNAL(signal_socketDisConnect(int,TcpClientSocket*)),this,SLOT(slot_DisConnect(int,TcpClientSocket*)));

    //����CTcpServer��ת���ź�,����socket�߳� 
	connect(this,SIGNAL(UIcmd_toSocket(int,int,QByteArray)),tcpClientSocket,SLOT(UIcmd_process(int,int,QByteArray)));

    //����CTcpServer��ת���ź�  ��ת����socket�̵߳��ź�, ����UI����
	connect(tcpClientSocket,SIGNAL(signal_CMD_SocketToUI(int,int,int)),this,SIGNAL(signal_CMD_SocketToUI(int,int,int)));

	//Ҫ��moveToThread,���󴴽�ʱ�����и�ָ��
    tcpClientSocket->moveToThread(thread); //��tcp���ƶ����µ��߳�
    thread->start();//�߳̿�ʼ����

	tcpClientSocketList.append(tcpClientSocket);

    printf("tcpClientSocketList.count=%d\n",tcpClientSocketList.count());

	//��ɾ
    ClientSocketID.append(socketDescriptor);  //��ID��Ҳ����

	//�ڴ˿��Ը��µ�¼��IP��ַ�б�
    //�ĳ���֤����ɹ���,��UI������ʾ�ͻ����б�

    emit signal_clientChange();   //�ͻ���������,�Ͽ���������,�����źţ�֪ͨ����,ˢ�¿ͻ�����Ϣ�б�
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
            ClientSocketID.removeAt(i);    //���б����Ƴ���Ծ��socketID
			 break;
        }
    }
	
	    for (int i=0;i<tcpClientSocketList.count();i++)
    {
        if (tcpClientSocketList[i]==tcpClientSocket)
        {
            tcpClientSocketList.removeAt(i);    //���б����Ƴ�������
            break;
        }
    }
	emit signal_clientChange();   //�ͻ���������,�Ͽ���������,�����źţ�֪ͨ����,ˢ�¿ͻ�����Ϣ�б�
  
    emit signal_CMD_TcpServertoUI(SocketID,SV_SOCKET_DISCONNECT); //����������UI
}

//ָ���ͻ������ӷ���Ϣ
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
