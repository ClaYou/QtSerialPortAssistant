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
* @brief		接收到socket的信息
* @return		void
*/
void TcpClient::readClient()
{
	//QTextCodec *tc = QTextCodec::codecForName("GBK");//处理中文乱码问题
	//receiveData(tc->toUnicode(this->readAll()));	 //简单的发送所有读到的信息
	emit receiveData(this->readAll());
}

/**
* @brief		发送消息给Socket
* @param data	发送的内容
*
* @return		void
*/
void TcpClient::sendMessage(const QByteArray &data)
{
	this->write(data);
}

/**
* @brief		Socket断开连接
* @return		void
*/
void TcpClient::disconnectedSlot()
{
	int i = this->socketDescriptor();
	emit clientDisconnected(this->socketDescriptor());
}