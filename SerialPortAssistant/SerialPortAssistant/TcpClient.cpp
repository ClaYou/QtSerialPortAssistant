#include "TcpClient.h"
#include <QTextCodec>

TcpClient::TcpClient(QObject *parent)
	: QTcpSocket(parent)
{
	connect(this, &QTcpSocket::readyRead, this, &TcpClient::readClient);
	connect(this, &QTcpSocket::disconnected, this, &TcpClient::disconnectedSlot);
}

TcpClient::~TcpClient()
{
}

/**
* @brief		���յ�socket����Ϣ
* @return		void
*/
void TcpClient::readClient()
{
	//QTextCodec *tc = QTextCodec::codecForName("GBK");//����������������
	//receiveData(tc->toUnicode(this->readAll()));	 //�򵥵ķ������ж�������Ϣ
	emit receiveData(this->readAll());
}

/**
* @brief		������Ϣ��Socket
* @param data	���͵�����
*
* @return		void
*/
void TcpClient::sendMessage(const QByteArray &data)
{
	this->write(data);
}

/**
* @brief		Socket�Ͽ�����
* @return		void
*/
void TcpClient::disconnectedSlot()
{
	int i = this->socketDescriptor();
	emit clientDisconnected(this->socketDescriptor());
}