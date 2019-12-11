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
* @brief		���µĿͻ�������
* @return		void
*/
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	TcpClient *socket = new TcpClient;
	socket->setSocketDescriptor(socketDescriptor);
	QString address = socket->peerAddress().toString();
	quint16 port = socket->peerPort();
	emit newConnect(address, port);		//���͸�����

	connect(socket, &TcpClient::receiveData, this, &TcpServer::receiveFromClient);	//���յ��ͻ�����������Ϣ
	connect(socket, &TcpClient::clientDisconnected, this, &TcpServer::disconnectedFromClient);	//���յ��ͻ��˶Ͽ�����

	m_listClient.append(socket);
}

/**
* @brief		��ȡ��ǰ�ͻ��˵�������
* @return		����
*/
int TcpServer::getCurClientCount()
{
	return m_listClient.size();
}

/**
* @brief		������client������Ϣ
* @param data	���͵�����
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
* @brief		 ���յ�client����Ϣ
* @param strData ���͵�����
*
* @return		 void
*/
void TcpServer::receiveFromClient(QByteArray strData)
{
	emit updateMessage(strData);
}

/**
* @brief		�Ͽ����пͻ��˵�����
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
* @brief				���յ�client�Ͽ����ź�
* @param iDescriptor	client��ʶ
*
* @return				void
*/
void TcpServer::disconnectedFromClient(int iDescriptor)
{
	for (int i = 0; i < m_listClient.size(); i++)
	{
		if (m_listClient[i]->socketDescriptor() == iDescriptor)
		{
			//ɾ���Ѿ��Ͽ�������
			auto address = m_listClient[i]->peerAddress().toString();
			auto port = m_listClient[i]->peerPort(); 
			emit clientDisconnected(address, port);

			m_listClient.removeAt(i);
		}
	}
}