#include "TcpServer.h"
#include "TcpClient.h"

TcpServer::TcpServer(QObject *parent)
	: QTcpServer(parent)
{
}

TcpServer::~TcpServer()
{
}

/**
* @brief		有新的客户端连接
* @return		void
*/
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	TcpClient *socket = new TcpClient;
	socket->setSocketDescriptor(socketDescriptor);
	QString address = socket->peerAddress().toString();
	quint16 port = socket->peerPort();
	emit newConnect(address, port);		//发送给界面

	connect(socket, &TcpClient::receiveData, this, &TcpServer::receiveFromClient);	//接收到客户端来到的消息
	connect(socket, &TcpClient::clientDisconnected, this, &TcpServer::disconnectedFromClient);	//接收到客户端断开连接

	m_listClient.append(socket);
}

/**
* @brief		获取当前客户端的连接数
* @return		数量
*/
int TcpServer::getCurClientCount()
{
	return m_listClient.size();
}

/**
* @brief		给所有client发送信息
* @param data	发送的内容
*
* @return		void
*/
int TcpServer::sendMessage(const QByteArray &strMsg)
{
	int length = 0;

	QListIterator<TcpClient *> iter(m_listClient);
	while (iter.hasNext())
	{
		iter.next()->sendMessage(strMsg);
		length += strMsg.length();
	}

	return length;
}

/**
* @brief		 接收到client的信息
* @param strData 发送的内容
*
* @return		 void
*/
void TcpServer::receiveFromClient(QByteArray strData)
{
	emit updateMessage(strData);
}

/**
* @brief		断开所有客户端的连接
* @return		void
*/
void TcpServer::clearClient()
{
	for (int i = 0; i < m_listClient.size(); i++)
	{
		m_listClient[i]->disconnectFromHost();
	}
}

/**
* @brief				接收到client断开的信号
* @param iDescriptor	client标识
*
* @return				void
*/
void TcpServer::disconnectedFromClient(int iDescriptor)
{
	for (int i = 0; i < m_listClient.size(); i++)
	{
		if (m_listClient[i]->socketDescriptor() == iDescriptor)
		{
			//删除已经断开的连接
			auto address = m_listClient[i]->peerAddress().toString();
			auto port = m_listClient[i]->peerPort(); 
			emit clientDisconnected(address, port);

			m_listClient.removeAt(i);
		}
	}
}