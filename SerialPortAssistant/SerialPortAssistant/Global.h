#pragma once
#include <QFlag>

/*模式*/
#define TCPUDP	  0
#define TCPCLIENT 0
#define TCPSERVER 1
#define SERIALPORT 2

#define MAGICNUMBER (quint32)0xA0B0C0D0
/*文件MagicNumber*/
const quint32 MagicNumber = 0xA0B0C0D0;

/*连接的方式*/
enum EnmConnOption
{
	/*客户端连接*/
	TcpClientOpt = 0,
	/*服务器开启*/
	TcpServerOpt,
	/*串口连接*/
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

/*设置的信息*/
struct SettingCollect
{
	EnmConnOption conOpt;	//连接方式

	QString serialPortName;	//端口名
	int baudRate;			//波特率
	int dataBits;			//数据位
	QString parity;			//校验位
	float stopBits;			//停止位
	QString flowControl;	//流控

	QString ip;				//ip
	quint16 port;			//端口

	EnmEncodeOpt receOpt;	//接收方式
	bool wordwrap;			//自动换行
	bool showSend;			//显示发送
	bool showTime;			//显示时间
	QString timeFormat;		//时间格式
	int minInterval;		//

	EnmEncodeOpt sendOpt;	//发送方式
	bool sendRepeat;		//是否重复发送
	int repeatTimes;		//重复发送的间隔
	EnmTimeSuffix timeSuffix;	//时间的单位
	bool sptNote;			//是否支持注释
	bool sendFormat;		//是否格式化

	bool saveLog;			//是否保存日志
	QString logFilePath;	//日志文件路径
	int maxFileSize;		//日志最大支持(MB)
	bool openWriteCache;	//打开写入缓存
	int cacheSize;			//缓存大小(KB)

	int maxLines;			//最多消息行数
	QString font;			//字体
	bool displayFormatColor;//显示颜色
	QString receMsgColor;	//接收消息的颜色
	QString sendMsgColor;	//发送消息的颜色
};

/*解决从文件中读枚举EnmConnOption*/
inline QDataStream &operator >> (QDataStream &in, EnmConnOption &conOpt)
{
	int tmp;
	in >> tmp;
	conOpt = (EnmConnOption)tmp;
	return in;
}

/*解决从文件中读枚举EnmEncodeOpt*/
inline QDataStream &operator >> (QDataStream &in, EnmEncodeOpt &codeOpt)
{
	int tmp;
	in >> tmp;
	codeOpt = (EnmEncodeOpt)tmp;
	return in;
}

/*解决从文件中读枚举EnmTimeSuffix*/
inline QDataStream &operator >> (QDataStream &in, EnmTimeSuffix &timeSuf)
{
	int tmp;
	in >> tmp;
	timeSuf = (EnmTimeSuffix)tmp;
	return in;
}

/*重载 << 运算符，可以写入结构体*/
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

/*重载 << 运算符，可以读结构体*/
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

/*输出消息的方式*/
enum EnmLogOption
{
	NoneLog		= 0x0000,
	/*控制台输出*/
	Console		= 0x0010,
	/*日志文件*/
	LogFile		= 0x0020,
	/*界面上*/
	Interface	= 0x0040
};

Q_DECLARE_FLAGS(EnmLogOptions, EnmLogOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(EnmLogOptions)

/*当前发送者*/
enum EnmCurSendOption
{
	NoneSend = 0,
	/*Tcp服务器发送*/
	TcpServerSend,
	/*Tcp客户端发送*/
	TcpClientSend,
	/*串口发送*/
	SerialPortSend
};

/*当前的运行状态*/
enum EnmCurRunState
{
	/*打开状态*/
	StateStart = 0,
	/*暂停状态*/
	StatePause,
	/*停止状态*/
	StateStop
};

/*更新状态栏RX TX的信息*/
enum EnmUpdateRxTx
{
	/*清空RxTx数量*/
	ClearRxTx = 0,
	/*增加Rx的计数*/
	AddRx,
	/*增加Tx的计数*/
	AddTx,
};

/*记录上一次的布局状态*/
enum EnmLastLayout
{
	/*水平布局*/
	HBoxLayout,
	/*垂直布局*/
	VBoxLayout,
	/*网格布局*/
	GridLayout,
	/*自定义布局*/
	CustLayout,
};