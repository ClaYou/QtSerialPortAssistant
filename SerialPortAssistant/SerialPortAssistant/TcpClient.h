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
	void receiveData(QByteArray);		//收到消息，发送给服务端
	void clientDisconnected(int);		//断开连接

private:
	Q_SLOT void readClient();
	Q_SLOT void disconnectedSlot();
};
