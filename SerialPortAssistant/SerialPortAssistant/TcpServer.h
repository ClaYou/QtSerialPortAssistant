#pragma once

#include <QTcpServer>
#include <QList>
#include "TcpClient.h"

class TcpServer : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(QObject *parent);
	~TcpServer();
	int sendMessage(const QByteArray &strMsg);	//发送消息给客户端,返回发送的长度
	int getCurClientCount();		//获取当前客户端连接的数量
	void clearClient();				//清空已经建立连接的客户端

signals:
	void updateMessage(QByteArray strData);
	void newConnect(QString, quint16);
	void clientDisconnected(QString, int);	//发送IP 与 端口

protected:
	void incomingConnection(qintptr socketDescriptor);

private:
	QList<TcpClient *> m_listClient;

	Q_SLOT void receiveFromClient(QByteArray strData);
	Q_SLOT void disconnectedFromClient(int iDescriptor);
};
