#pragma once
#include <QFlag>

/*ģʽ*/
#define TCPUDP	  0
#define TCPCLIENT 0
#define TCPSERVER 1
#define SERIALPORT 2

#define MAGICNUMBER (quint32)0xA0B0C0D0
/*�ļ�MagicNumber*/
const quint32 MagicNumber = 0xA0B0C0D0;

/*���ӵķ�ʽ*/
enum EnmConnOption
{
	/*�ͻ�������*/
	TcpClientOpt = 0,
	/*����������*/
	TcpServerOpt,
	/*��������*/
	SerialPortOpt
};

struct ConnectInfo
{
	EnmConnOption option;

};

struct ClientInfo
{
	QString ip;
	quint16 port;
};

struct ServerInfo
{
	quint16 port;
};

struct SerialPortInfo
{

};

enum EnmEncodeOpt
{
	ASCII = 0,
	Hex
};

enum EnmTimeSuffix
{
	MilliSecond = 0,
	Second,
	Minute,
	Hour
};

/*���õ���Ϣ*/
struct SettingCollect
{
	EnmConnOption conOpt;	//���ӷ�ʽ

	QString serialPortName;	//�˿���
	int baudRate;			//������
	int dataBits;			//����λ
	QString parity;			//У��λ
	float stopBits;			//ֹͣλ
	QString flowControl;	//����

	QString ip;				//ip
	quint16 port;			//�˿�

	EnmEncodeOpt receOpt;	//���շ�ʽ
	bool wordwrap;			//�Զ�����
	bool showSend;			//��ʾ����
	bool showTime;			//��ʾʱ��
	QString timeFormat;		//ʱ���ʽ
	int minInterval;		//

	EnmEncodeOpt sendOpt;	//���ͷ�ʽ
	bool sendRepeat;		//�Ƿ��ظ�����
	int repeatTimes;		//�ظ����͵ļ��
	EnmTimeSuffix timeSuffix;	//ʱ��ĵ�λ
	bool sptNote;			//�Ƿ�֧��ע��
	bool sendFormat;		//�Ƿ��ʽ��

	bool saveLog;			//�Ƿ񱣴���־
	QString logFilePath;	//��־�ļ�·��
	int maxFileSize;		//��־���֧��(MB)
	bool openWriteCache;	//��д�뻺��
	int cacheSize;			//�����С(KB)

	int maxLines;			//�����Ϣ����
	QString font;			//����
	bool displayFormatColor;//��ʾ��ɫ
	QString receMsgColor;	//������Ϣ����ɫ
	QString sendMsgColor;	//������Ϣ����ɫ
};

/*������ļ��ж�ö��EnmConnOption*/
inline QDataStream &operator >> (QDataStream &in, EnmConnOption &conOpt)
{
	int tmp;
	in >> tmp;
	conOpt = (EnmConnOption)tmp;
	return in;
}

/*������ļ��ж�ö��EnmEncodeOpt*/
inline QDataStream &operator >> (QDataStream &in, EnmEncodeOpt &codeOpt)
{
	int tmp;
	in >> tmp;
	codeOpt = (EnmEncodeOpt)tmp;
	return in;
}

/*������ļ��ж�ö��EnmTimeSuffix*/
inline QDataStream &operator >> (QDataStream &in, EnmTimeSuffix &timeSuf)
{
	int tmp;
	in >> tmp;
	timeSuf = (EnmTimeSuffix)tmp;
	return in;
}

/*���� << �����������д��ṹ��*/
inline QDataStream &operator<<(QDataStream & out, const SettingCollect &setInfo)
{
	out << int(setInfo.conOpt) << setInfo.serialPortName << setInfo.baudRate << setInfo.dataBits << setInfo.parity
		<< setInfo.stopBits << setInfo.flowControl << setInfo.ip << setInfo.port << (int)setInfo.receOpt << setInfo.wordwrap
		<< setInfo.showSend << setInfo.showTime << setInfo.timeFormat << setInfo.minInterval << (int)setInfo.sendOpt << setInfo.sendRepeat
		<< setInfo.repeatTimes << setInfo.timeSuffix << setInfo.sptNote << setInfo.sendFormat << setInfo.saveLog
		<< setInfo.logFilePath << setInfo.maxFileSize << setInfo.openWriteCache << setInfo.cacheSize << setInfo.maxLines << setInfo.font
		<< setInfo.displayFormatColor << setInfo.receMsgColor << setInfo.sendMsgColor;
	return out;
}

/*���� << ����������Զ��ṹ��*/
inline QDataStream &operator >> (QDataStream &in, SettingCollect &setInfo)
{
	in >> setInfo.conOpt >> setInfo.serialPortName >> setInfo.baudRate >> setInfo.dataBits >> setInfo.parity
		>> setInfo.stopBits >> setInfo.flowControl >> setInfo.ip >> setInfo.port >> setInfo.receOpt >> setInfo.wordwrap
		>> setInfo.showSend >> setInfo.showTime >>  setInfo.timeFormat >> setInfo.minInterval >> setInfo.sendOpt >> setInfo.sendRepeat
		>> setInfo.repeatTimes >> setInfo.timeSuffix >> setInfo.sptNote >> setInfo.sendFormat >> setInfo.saveLog
		>> setInfo.logFilePath >> setInfo.maxFileSize >> setInfo.openWriteCache >> setInfo.cacheSize >> setInfo.maxLines >> setInfo.font
		>> setInfo.displayFormatColor >> setInfo.receMsgColor >> setInfo.sendMsgColor;
	return in;
}

/*�����Ϣ�ķ�ʽ*/
enum EnmLogOption
{
	NoneLog		= 0x0000,
	/*����̨���*/
	Console		= 0x0010,
	/*��־�ļ�*/
	LogFile		= 0x0020,
	/*������*/
	Interface	= 0x0040
};

Q_DECLARE_FLAGS(EnmLogOptions, EnmLogOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(EnmLogOptions)

/*��ǰ������*/
enum EnmCurSendOption
{
	NoneSend = 0,
	/*Tcp����������*/
	TcpServerSend,
	/*Tcp�ͻ��˷���*/
	TcpClientSend,
	/*���ڷ���*/
	SerialPortSend
};

/*��ǰ������״̬*/
enum EnmCurRunState
{
	/*��״̬*/
	StateStart = 0,
	/*��ͣ״̬*/
	StatePause,
	/*ֹͣ״̬*/
	StateStop
};

/*����״̬��RX TX����Ϣ*/
enum EnmUpdateRxTx
{
	/*���RxTx����*/
	ClearRxTx = 0,
	/*����Rx�ļ���*/
	AddRx,
	/*����Tx�ļ���*/
	AddTx,
};

/*��¼��һ�εĲ���״̬*/
enum EnmLastLayout
{
	/*ˮƽ����*/
	HBoxLayout,
	/*��ֱ����*/
	VBoxLayout,
	/*���񲼾�*/
	GridLayout,
	/*�Զ��岼��*/
	CustLayout,
};