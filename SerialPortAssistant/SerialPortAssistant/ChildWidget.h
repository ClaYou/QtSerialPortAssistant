#pragma once

#include <QtWidgets/QMainWindow>
#include <QAbstractSocket>
#include <QMap>
#include <QSerialPort>
#include "Global.h"

class QLabel;
class QTreeWidget;
class QPushButton;
class QComboBox;
class QRadioButton;
class QCheckBox;
class QSpinBox;
class QTextEdit;
class TcpServer;
class QTcpSocket;
class QLabel;

class ChildWidget : public  QMainWindow
{
	Q_OBJECT

public:
	ChildWidget(QWidget *parent = nullptr);
	~ChildWidget();
	void initSetting(const SettingCollect&);
	SettingCollect getSetting();
	bool doConnect();
	void doDisconnect();
	void doClearEdit();
	void doQuckSetting(bool bOn);					//是否打开快速设置窗口
	void setLogState(bool bOn);						//是否打开日志
	void setLogPath(const QString &logPath);		//设置日志路径
	void restoreDefault();							//恢复默认

private:
#pragma region 界面相关
	QWidget *m_pCenWidget;
	QWidget *m_pLeftSetWgt;		//左边的布局窗体
	QTextEdit *m_pEditMsg;		//显示消息列表
	QTextEdit *m_pEditSend;		//要发送的消息
	QPushButton *m_pBtnSend;	//发送
	QComboBox *m_pCmbHistory;	//记录曾经发送的记录

	QComboBox *m_pCmbCommWay;	//通信的方式
	QComboBox *m_pCmbMode;
	QComboBox *m_pCmbIp;
	QComboBox *m_pCmbPort;
	QLabel *m_pLabMode;
	QLabel *m_pLabIP;
	QLabel *m_pLabPort;

	//串口通信
	QLabel *m_pLabBaudRate;
	QLabel *m_pLabDataBits;
	QLabel *m_pLabParity;
	QLabel *m_pLabStopBits;
	QLabel *m_pLabFlowControl;
	QComboBox *m_pCmbBaudRate;
	QComboBox *m_pCmbDataBits;		//数据位
	QComboBox *m_pCmbParity;		//校验位
	QComboBox *m_pCmbStopBits;		//停止位
	QComboBox *m_pCmbFlowControl;	//流控制

									//连接列表 服务端下有效
	QTreeWidget *m_pTreeWgtClient;

	//接收
	QRadioButton *m_pRadbtnRecASCII;
	QRadioButton *m_pRadbtnRecHex;
	QCheckBox *m_pChkWordwrap;
	QCheckBox *m_pChkShowSend;
	QCheckBox *m_pChkShowTime;

	//发送
	QRadioButton *m_pRadbtnSendASCII;
	QRadioButton *m_pRadbtnSendHex;
	QCheckBox *m_pChkRepeatSend;
	QSpinBox *m_pSpboxTimes;

	//状态栏
	QLabel *m_pLabConnInfo;
	QLabel *m_pLabTx;
	QLabel *m_pLabRx;
	QLabel *m_pLabPlace;
#pragma  endregion

private:
	TcpServer *m_pTcpServer;		//服务器监听
	QTcpSocket *m_pTcpClient;		//客户端连接socket
	QSerialPort *m_pSerialPort;		//串口通信

	QMap<quint16, QString> m_mapClient;	//保存当前客户端列表

	QTimer *m_pRepeatSendTimer;			//重复发送定时器
	EnmCurSendOption m_curSendOpt;		//当前连接的状态

	int m_iRxCount;		//Rx 计数
	int m_iTxCount;		//Tx 计数

	QString m_strTimeFormat;	//时间格式
	bool m_bSptNote;			//支持注释
	bool m_bSendFormat;			//发送是否格式化
	bool m_bSaveLog;			//是否写日志
	QString m_strLogPath;		//日志文件路径
	int m_iMaxLines;			//消息最多行数
	QString m_msgFont;			//显示消息的字体
	bool m_bDisplayColor;		//是否显示颜色
	QString m_receMsgColor;		//接收的字体颜色
	QString m_sendMsgColor;		//发送的字体颜色

protected:
	void closeEvent(QCloseEvent *event);

private:
	void initViewer();
	void initConnect();
	void initStatusBar();
	void addPortList();
	void createCenWgt();
	EnmConnOption getConnWay();		//获取一下当前的连接方式
	void setLabelTextColor(QLabel *pLabel, const QColor &color);	//设置label颜色
	void updateRxTxInfo(EnmUpdateRxTx state, int count = 0);		//更新状态栏的RxTx信息
	void showMessage(QString strMsg, EnmLogOptions ways = EnmLogOption::Console | EnmLogOption::Interface | EnmLogOption::LogFile);
	void setRepeatTimes(int times, EnmTimeSuffix timeSuf);
	void setSerialPortInfo(const SettingCollect &collect);
	void appendToComboBox(QComboBox *pCmbBox, const QString &strMsg);

	Q_SLOT void commWayChanged(int index);
	Q_SLOT void tcpudpModeChanged(int index);
	Q_SLOT void sendBtnClicked();			//发送按钮
	Q_SLOT void sendHistorySelcted();		//选择右下角历史发送
	Q_SLOT void repeatSend(int iSend);		//选择重复发送
	Q_SLOT void readDataFromClient(QByteArray strMsg);
	Q_SLOT void newConnectFromServer(QString strAddr, quint16 iport);
	Q_SLOT void readDataFromServer();
	Q_SLOT void disconnectedFromServer(QString strAddr, int iport);
	Q_SLOT void clientSocketError(QAbstractSocket::SocketError socketError);
	Q_SLOT void readDataFromSerialPort();
	Q_SLOT void serialPortError(QSerialPort::SerialPortError error);
};
