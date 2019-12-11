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
	int sendMessage(const QByteArray &strMsg);	//������Ϣ���ͻ���,���ط��͵ĳ���
	int getCurClientCount();		//��ȡ��ǰ�ͻ������ӵ�����
	void clearClient();				//����Ѿ��������ӵĿͻ���

signals:
	void updateMessage(QByteArray strData);
	void newConnect(QString, quint16);
	void clientDisconnected(QString, int);	//����IP �� �˿�

protected:
	void incomingConnection(qintptr socketDescriptor);

private:
	QList<TcpClient *> m_listClient;

	Q_SLOT void receiveFromClient(QByteArray strData);
	Q_SLOT void disconnectedFromClient(int iDescriptor);
};
