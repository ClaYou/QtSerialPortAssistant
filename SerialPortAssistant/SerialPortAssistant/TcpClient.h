#pragma once

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QTcpSocket
{
	Q_OBJECT

public:
	TcpClient(QObject *parent = nullptr);
	~TcpClient();
	void sendMessage(const QByteArray &data);

signals:
	void receiveData(QByteArray);		//�յ���Ϣ�����͸������
	void clientDisconnected(int);		//�Ͽ�����

private:
	Q_SLOT void readClient();
	Q_SLOT void disconnectedSlot();
};
