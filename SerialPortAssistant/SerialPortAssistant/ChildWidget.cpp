#include "ChildWidget.h"
#include <QStatusBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QTreeWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QSpinBox>
#include <QWidget>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <QTextCodec>
#include <qglobal.h>

#include "ToolsHelp.h"
#include "TcpServer.h"
#include "Logger.h"

ChildWidget::ChildWidget(QWidget *parent)
	: QMainWindow(parent)
{
	initViewer();

	setWindowFlags(Qt::FramelessWindowHint);
	m_curSendOpt = EnmCurSendOption::NoneSend;

	//!DEBUG 三个复选框选中
	m_pChkShowSend->setChecked(true);
	m_pChkShowTime->setChecked(true);
	m_pChkWordwrap->setChecked(true);
}

ChildWidget::~ChildWidget()
{
	qDebug() << QStringLiteral("通道删除");
}


/**
* @brief		初始化各种信息
* @return		void
*/
void ChildWidget::initViewer()
{
	createCenWgt();
	initConnect();
	initStatusBar();
	addPortList();

	m_pTcpServer = 0;
	m_pTcpClient = 0;
	m_pSerialPort = 0;
	m_pRepeatSendTimer = 0;
	this->m_iRxCount = 0;
	this->m_iTxCount = 0;

	this->commWayChanged(TCPUDP);
	this->tcpudpModeChanged(TCPCLIENT);

	qDebug() << this->m_pEditMsg->font().toString();
	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	font.setFamily(QStringLiteral("SimSun"));
	this->m_pEditMsg->setFont(font);
	

	this->m_bSptNote = false;
	this->m_bSendFormat = false;
	this->m_bSaveLog = false;
	this->m_iMaxLines = 500;
	this->m_bDisplayColor = false;
	this->m_strTimeFormat = "[hh:mm:ss.zzz]";
	this->m_msgFont = "Fixedsys,12";		//字体设置个默认值
	this->m_receMsgColor = "000000";
	this->m_sendMsgColor = "000000";
}

/**
* @brief		主界面单击连接按钮触发
* @return		void
*/
bool ChildWidget::doConnect()
{
	if (m_pCmbCommWay->currentIndex() == TCPUDP)
	{
		int iPort = 0;
		bool bok;

		iPort = m_pCmbPort->currentText().toInt(&bok);
		if (!bok)
		{
			QMessageBox::information(this, QStringLiteral("警告"), QStringLiteral("填写正确的端口"));
			this->m_pCmbPort->setFocus();
			return false;
		}
		//服务端监听
		if (m_pCmbMode->currentIndex() == TCPSERVER)
		{
			if (m_pTcpServer)
			{
				QObject::disconnect(m_pTcpServer, 0, 0, 0);
				m_pTcpServer->close();
				delete m_pTcpServer;
				m_pTcpServer = nullptr;
			}
			m_pTcpServer = new TcpServer(this);

			if (!m_pTcpServer->listen(QHostAddress::Any, iPort))
			{
				QMessageBox::information(this, QStringLiteral("警告"), QStringLiteral("TCP Server打开失败\n") + m_pTcpServer->errorString());
				m_pTcpServer->close();
				delete m_pTcpServer;
				m_pTcpServer = nullptr;

				return false;
			}

			connect(m_pTcpServer, &TcpServer::updateMessage, this, &ChildWidget::readDataFromClient);	//接到client发送来的数据，更新界面
			connect(m_pTcpServer, &TcpServer::newConnect, this, &ChildWidget::newConnectFromServer);
			connect(m_pTcpServer, &TcpServer::clientDisconnected, this, &ChildWidget::disconnectedFromServer);

			m_curSendOpt = TcpServerSend;

			QString strConn = "Waiting for Connection on " + QString("%0").arg(iPort) + "...";

			this->setLabelTextColor(m_pLabConnInfo, Qt::darkGreen);
			this->m_pLabConnInfo->setText(strConn);
		}
		else
		{
			//客户端
			QString strIp = "";

			if (this->m_pCmbIp->currentText().toLower() == "localhost")
			{
				strIp = "127.0.0.1";	//localhost
			}
			else
			{
				strIp = this->m_pCmbIp->currentText();
			}

			QRegExp regexp("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
			if (!regexp.exactMatch(strIp))
			{
				QMessageBox::information(this, QStringLiteral("警告"), QStringLiteral("填写正确的IP地址"));
				this->m_pCmbIp->setFocus();

				return false;
			}

			quint16 iport = 0;

			iport = m_pCmbPort->currentText().toInt(&bok);
			if (!bok)
			{
				QMessageBox::information(this, QStringLiteral("警告"), QStringLiteral("填写正确的端口"), QMessageBox::Ok);
				this->m_pCmbPort->setFocus();
			}
			else
			{
				if (m_pTcpClient)
				{
					QObject::disconnect(m_pTcpClient, 0, 0, 0);
					m_pTcpClient->close();
					delete m_pTcpClient;
					m_pTcpClient = nullptr;
				}
				m_pTcpClient = new QTcpSocket(this);
				m_pTcpClient->connectToHost(strIp, iport);

				if (!m_pTcpClient->waitForConnected(1000))
				{
					//连接服务器失败
					m_pLabConnInfo->setText(m_pTcpClient->errorString());
					delete m_pTcpClient;
					m_pTcpClient = nullptr;

					return false;
				}
				m_curSendOpt = TcpClientSend;		//当前状态标记为client

				appendToComboBox(m_pCmbIp, strIp);

				connect(m_pTcpClient, &QTcpSocket::readyRead, this, &ChildWidget::readDataFromServer);
				connect(m_pTcpClient, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &ChildWidget::clientSocketError);	//发生错误了
				this->setLabelTextColor(m_pLabConnInfo, Qt::darkGreen);
				m_pLabConnInfo->setText("Connection is established");
			}
		}
	}
	else
	{
		//串口模式
		if (m_pSerialPort)
		{
			disconnect(m_pSerialPort, 0, 0, 0);
			m_pSerialPort->close();
			delete m_pSerialPort;
			m_pSerialPort = nullptr;
		}
		m_pSerialPort = new QSerialPort(this);

		m_pSerialPort->setPortName(m_pCmbCommWay->currentText());									//设置串口号
		m_pSerialPort->setBaudRate(m_pCmbBaudRate->currentText().toInt());							//设置波特率
		m_pSerialPort->setDataBits((QSerialPort::DataBits)m_pCmbDataBits->currentText().toInt());	//数据位 直接转(不严谨)
		m_pSerialPort->setParity((QSerialPort::Parity)m_pCmbParity->currentIndex());				//校验位 直接转 
		m_pSerialPort->setStopBits((QSerialPort::StopBits)m_pCmbStopBits->currentIndex());			//停止位
		m_pSerialPort->setFlowControl((QSerialPort::FlowControl)m_pCmbFlowControl->currentIndex()); //流控制 

		if (!m_pSerialPort->open(QIODevice::ReadWrite))
		{
			qDebug() << m_pSerialPort->errorString();
			QMessageBox::information(this, QStringLiteral("警告"), QStringLiteral("串口打开失败\n") + m_pSerialPort->errorString());

			delete m_pSerialPort;
			m_pSerialPort = nullptr;

			setLabelTextColor(m_pLabConnInfo, Qt::red);
			m_pLabConnInfo->setText(QString("%1 CLOSED").arg(m_pCmbCommWay->currentText()));

			return false;
		}

		connect(m_pSerialPort, &QSerialPort::readyRead, this, &ChildWidget::readDataFromSerialPort);
		connect(m_pSerialPort, &QSerialPort::errorOccurred, this, &ChildWidget::serialPortError);

		//改变状态和显示信息
		m_curSendOpt = SerialPortSend;

		QString strConn = QString("%1 OPENED, %2, %3, %4, %5, %6").arg(m_pCmbPort->currentText()).arg(m_pCmbBaudRate->currentText())
			.arg(m_pCmbDataBits->currentText()).arg(m_pCmbParity->currentText())
			.arg(m_pCmbStopBits->currentText()).arg(m_pCmbFlowControl->currentText());

		this->setLabelTextColor(m_pLabConnInfo, Qt::darkGreen);
		this->m_pLabConnInfo->setText(strConn);
	}

	m_pBtnSend->setText(QStringLiteral("发送"));
	return true;
}

/**
* @brief		主界面单击断开按钮触发
* @return		void
*/
void ChildWidget::doDisconnect()
{
	if (m_curSendOpt == TcpClientSend && m_pTcpClient)
	{
		QObject::disconnect(m_pTcpClient, 0, 0, 0);
		m_pTcpClient->waitForDisconnected(1000);
		m_pTcpClient->deleteLater();
		m_pTcpClient = nullptr;

		setLabelTextColor(m_pLabConnInfo, Qt::red);
		m_pLabConnInfo->setText("The socket is not connected");
	}
	else if (m_curSendOpt == TcpServerSend && m_pTcpServer)
	{
		//服务器状态下
		QObject::disconnect(m_pTcpServer, 0, 0, 0);
		m_pTcpServer->clearClient();	//清空已经连接的客户端
		m_pTcpServer->close();
		m_pTcpServer->deleteLater();
		m_pTcpServer = nullptr;

		//清空保存的信息
		m_mapClient.clear();
		//清空界面树形连接信息
		m_pTreeWgtClient->clear();

		setLabelTextColor(m_pLabConnInfo, Qt::red);
		m_pLabConnInfo->setText("server closed");
	}
	else if (m_curSendOpt == SerialPortSend)
	{
		QObject::disconnect(m_pSerialPort, 0, 0, 0);
		m_pSerialPort->close();
		m_pSerialPort->deleteLater();
		m_pSerialPort = nullptr;

		setLabelTextColor(m_pLabConnInfo, Qt::red);
		m_pLabConnInfo->setText(QString("%1 CLOSED").arg(m_pCmbCommWay->currentText()));
	}

	m_curSendOpt = EnmCurSendOption::NoneSend;	//断开连接后记得把当前状态改变

	m_pBtnSend->setText(QStringLiteral("打开")); //发送按钮改为打开
}

/**
* @brief		接收到主界面的清除按钮
* @return		void
*/
void ChildWidget::doClearEdit()
{
	m_pEditMsg->clear();
}

/**
* @brief			 更新状态栏的RxTx信息
* @param bOn		 是否打开
* @return			 void
*/
void ChildWidget::doQuckSetting(bool bOn)
{
	//隐藏或显示左边窗体即可
	if (bOn)
	{
		m_pLeftSetWgt->show();
	}
	else
	{
		m_pLeftSetWgt->hide();
	}
}

/**
* @brief			 设置日志路径
* @param logPath	 日志路径
* @return			 void
*/
void ChildWidget::setLogPath(const QString &logPath)
{
	if (!logPath.isEmpty())
	{
		m_strLogPath = logPath;
	}
}

/**
* @brief			 是否打开日志
* @param bOn		 是否打开
* @return			 void
*/
void ChildWidget::setLogState(bool bOn)
{
	m_bSaveLog = bOn;
}

/**
* @brief			恢复初始状态
* @return			void
*/
void ChildWidget::restoreDefault()
{
	this->m_pCmbCommWay->setCurrentIndex(0);
	this->commWayChanged(TCPUDP);
	this->tcpudpModeChanged(TCPCLIENT);
	this->m_iRxCount = 0;
	this->m_iTxCount = 0;
	this->m_pLabRx->setText("Rx:0 Bytes");
	this->m_pLabTx->setText("Tx:0 Bytes");
	this->m_pLabConnInfo->setText("COM1 CLOSED");
	this->m_pChkRepeatSend->setChecked(false);
	this->m_pRadbtnRecASCII->setChecked(true);
	this->m_pRadbtnSendASCII->setChecked(true);
	this->m_pChkShowSend->setChecked(true);
	this->m_pChkShowTime->setChecked(true);
	this->m_pChkWordwrap->setChecked(true);
	this->m_bSptNote = false;
	this->m_bSendFormat = false;
	this->m_bSaveLog = false;
	this->m_iMaxLines = 500;
	this->m_strTimeFormat = "[hh:mm:ss.zzz]";
	this->m_pEditMsg->document()->setMaximumBlockCount(m_iMaxLines);
	this->m_pEditMsg->clear();
	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	font.setFamily(QStringLiteral("SimSun"));
	this->m_pEditMsg->setFont(font);
	this->m_bDisplayColor = false;
}

/**
* @brief			初始化设置的信息 public
* @param collect	设置的信息
*
* @return			void
*/
void ChildWidget::initSetting(const SettingCollect &setInfo)
{
	if (setInfo.conOpt == EnmConnOption::SerialPortOpt)
	{
		//根据设置信息设置
		setSerialPortInfo(setInfo);
	}
	else
	{
		m_pCmbCommWay->setCurrentIndex(0);

		if (setInfo.conOpt == EnmConnOption::TcpClientOpt)
		{
			m_pCmbMode->setCurrentIndex(0);
		}
		else
		{
			m_pCmbMode->setCurrentIndex(1);
		}
	}

	m_pRadbtnRecASCII->setChecked(setInfo.receOpt == EnmEncodeOpt::ASCII);
	m_pRadbtnRecHex->setChecked(setInfo.receOpt == EnmEncodeOpt::Hex);
	m_pChkWordwrap->setChecked(setInfo.wordwrap);
	m_pChkShowSend->setChecked(setInfo.showSend);
	m_pChkShowTime->setChecked(setInfo.showTime);
	m_strTimeFormat = setInfo.timeFormat;

	m_pRadbtnSendASCII->setChecked(setInfo.sendOpt == EnmEncodeOpt::ASCII);
	m_pRadbtnSendHex->setChecked(setInfo.sendOpt == EnmEncodeOpt::Hex);
	setRepeatTimes(setInfo.repeatTimes, setInfo.timeSuffix);
	m_pChkRepeatSend->setChecked(setInfo.sendRepeat);
	m_bSptNote = setInfo.sptNote;
	m_bSendFormat = setInfo.sendFormat;

	m_bSaveLog = setInfo.saveLog;
	m_strLogPath = setInfo.logFilePath;

	m_iMaxLines = setInfo.maxLines;
	this->m_pEditMsg->document()->setMaximumBlockCount(m_iMaxLines);
	//设置接收的字体。。
	m_msgFont = setInfo.font;
	QFont font;
	QStringList fontlist = m_msgFont.split(',');
	font.setFamily(fontlist[0]);
	font.setPointSize(fontlist[1].toInt());
	this->m_pEditMsg->setFont(font);

	m_bDisplayColor = setInfo.displayFormatColor;
	m_receMsgColor = setInfo.receMsgColor;
	m_sendMsgColor = setInfo.sendMsgColor;
}

/**
* @brief			初始化设置的信息 public
* @return			配置的信息
*/
SettingCollect ChildWidget::getSetting()
{
	SettingCollect setInfo;
	setInfo.conOpt				= this->getConnWay();
	setInfo.serialPortName		= this->m_pCmbCommWay->currentText();
	setInfo.baudRate			= this->m_pCmbBaudRate->currentText().toInt();
	setInfo.dataBits			= this->m_pCmbDataBits->currentText().toInt();
	setInfo.parity				= this->m_pCmbParity->currentText();
	setInfo.stopBits			= this->m_pCmbStopBits->currentText().toFloat();
	setInfo.flowControl			= this->m_pCmbFlowControl->currentText();
	setInfo.ip					= this->m_pCmbIp->currentText();
	setInfo.port				= this->m_pCmbPort->currentText().toInt();
	setInfo.receOpt				= this->m_pRadbtnRecASCII->isChecked() ? EnmEncodeOpt::ASCII : EnmEncodeOpt::Hex;
	setInfo.wordwrap			= this->m_pChkWordwrap->isChecked();
	setInfo.showSend			= this->m_pChkShowSend->isChecked();
	setInfo.showTime			= this->m_pChkShowTime->isChecked();
	setInfo.minInterval			= 0;
	setInfo.timeFormat			= this->m_strTimeFormat;
	setInfo.sendOpt				= this->m_pRadbtnSendASCII->isChecked() ? EnmEncodeOpt::ASCII : EnmEncodeOpt::Hex;
	setInfo.sendRepeat			= this->m_pChkRepeatSend->isChecked();
	setInfo.repeatTimes			= this->m_pSpboxTimes->value();
	setInfo.timeSuffix			= EnmTimeSuffix::MilliSecond;
	setInfo.sptNote				= this->m_bSptNote;
	setInfo.sendFormat			= this->m_bSendFormat;
	setInfo.saveLog				= this->m_bSaveLog;
	setInfo.logFilePath			= this->m_strLogPath;
	setInfo.maxFileSize			= 3000;
	setInfo.openWriteCache		= false;
	setInfo.cacheSize			= 4000;
	setInfo.maxLines			= m_iMaxLines;
	setInfo.font				= m_msgFont;
	setInfo.displayFormatColor	= m_bDisplayColor;
	setInfo.receMsgColor		= m_receMsgColor;
	setInfo.sendMsgColor		= m_sendMsgColor;

	return setInfo;
}

/**
* @brief			获取连接的方式 串口/TCPServer/TCPClient
* @return			连接的方式
*/
EnmConnOption ChildWidget::getConnWay()
{
	if (m_pCmbCommWay->currentIndex() == 0)
	{
		if (m_pCmbMode->currentIndex() == 0)
		{
			//客户端
			return EnmConnOption::TcpClientOpt;
		}
		else
		{
			//服务器
			return EnmConnOption::TcpServerOpt;
		}
	}
	else
	{
		return EnmConnOption::SerialPortOpt;
	}
}

/**
* @brief			设置串口的信息
* @param collect	设置的信息
*
* @return			void
*/
void ChildWidget::setSerialPortInfo(const SettingCollect &setInfo)
{
	//设置端口名
	for (int i = 0; i < m_pCmbCommWay->count(); i++)
	{
		if (m_pCmbCommWay->itemText(i) == setInfo.serialPortName)
		{
			m_pCmbCommWay->setCurrentIndex(i);
			break;
		}
	}

	//"9600", "19200", "38400", "57600", "115200"
	switch (setInfo.baudRate)
	{
	case 9600:
		m_pCmbBaudRate->setCurrentIndex(0);
		break;
	case 19200:
		m_pCmbBaudRate->setCurrentIndex(1);
		break;
	case 38400:
		m_pCmbBaudRate->setCurrentIndex(2);
		break;
	case 57600:
		m_pCmbBaudRate->setCurrentIndex(3);
		break;
	case 115200:
		m_pCmbBaudRate->setCurrentIndex(4);
		break;
	default:
		m_pCmbBaudRate->setCurrentIndex(0);
		break;
	}

	//"5", "6", "7", "8"
	switch (setInfo.dataBits)
	{
	case 5:
		m_pCmbDataBits->setCurrentIndex(0);
		break;
	case 6:
		m_pCmbDataBits->setCurrentIndex(1);
		break;
	case 7:
		m_pCmbDataBits->setCurrentIndex(2);
		break;
	case 8:
		m_pCmbDataBits->setCurrentIndex(3);
		break;
	default:
		m_pCmbDataBits->setCurrentIndex(0);
		break;
	}

	//"None", "Even", "Odd",  "Space", "Mark" 
	if (setInfo.parity == "None")
	{
		m_pCmbParity->setCurrentIndex(0);
	}
	else if (setInfo.parity == "Even")
	{
		m_pCmbParity->setCurrentIndex(1);
	}
	else if (setInfo.parity == "Odd")
	{
		m_pCmbParity->setCurrentIndex(2);
	}
	else if (setInfo.parity == "Space")
	{
		m_pCmbParity->setCurrentIndex(3);
	}
	else if (setInfo.parity == "Mark")
	{
		m_pCmbParity->setCurrentIndex(4);
	}
	else
	{
		m_pCmbParity->setCurrentIndex(0);
	}

	//"1", "1.5", "2"
	if (setInfo.stopBits == 1)
	{
		m_pCmbStopBits->setCurrentIndex(0);
	}
	else if (setInfo.stopBits == 1.5)
	{
		m_pCmbStopBits->setCurrentIndex(1);
	}
	else if (setInfo.stopBits == 2)
	{
		m_pCmbStopBits->setCurrentIndex(2);
	}
	else
	{
		m_pCmbStopBits->setCurrentIndex(0);
	}

	//"None", "Hardware", "Software"
	if (setInfo.flowControl == "None")
	{
		m_pCmbFlowControl->setCurrentIndex(0);
	}
	else if (setInfo.flowControl == "Hardware")
	{
		m_pCmbFlowControl->setCurrentIndex(1);
	}
	else if (setInfo.flowControl == "Software")
	{
		m_pCmbFlowControl->setCurrentIndex(2);
	}
	else
	{
		m_pCmbFlowControl->setCurrentIndex(0);
	}
}

/**
* @brief			设置窗体
* @return			void
*/
void ChildWidget::createCenWgt()
{
	m_pCenWidget = new QWidget;

	QGroupBox *gboxPortSet = new QGroupBox(QStringLiteral("串口设置"));
	QGroupBox *gboxReceiveSet = new QGroupBox(QStringLiteral("接收设置"));
	QGroupBox *gboxSendSet = new QGroupBox(QStringLiteral("发送设置"));

	//串口设置框
	QLabel *labCommWay = new QLabel(QStringLiteral("串口:"));
	m_pLabMode = new QLabel(QStringLiteral("Mode:"));
	m_pLabIP = new QLabel(QStringLiteral("IP:"));
	m_pLabPort = new QLabel(QStringLiteral("Port:"));
	m_pLabBaudRate = new QLabel(QStringLiteral("波特率:"));
	m_pLabDataBits = new QLabel(QStringLiteral("数据位:"));
	m_pLabParity = new QLabel(QStringLiteral("校验位:"));
	m_pLabStopBits = new QLabel(QStringLiteral("停止位:"));
	m_pLabFlowControl = new QLabel(QStringLiteral("流  控:"));
	m_pCmbCommWay = new QComboBox();
	m_pCmbMode = new QComboBox();
	m_pCmbIp = new QComboBox();
	m_pCmbIp->setEditable(true);
	m_pCmbPort = new QComboBox();
	m_pCmbPort->setEditable(true);
	m_pCmbBaudRate = new QComboBox();
	m_pCmbDataBits = new QComboBox();
	m_pCmbParity = new QComboBox();
	m_pCmbStopBits = new QComboBox();
	m_pCmbFlowControl = new QComboBox();

	QGridLayout *portSetLayout = new QGridLayout();
	portSetLayout->addWidget(labCommWay, 0, 0);
	portSetLayout->addWidget(m_pCmbCommWay, 0, 1);
	portSetLayout->addWidget(m_pLabMode, 1, 0);
	portSetLayout->addWidget(m_pCmbMode, 1, 1);
	portSetLayout->addWidget(m_pLabIP, 2, 0);
	portSetLayout->addWidget(m_pCmbIp, 2, 1);
	portSetLayout->addWidget(m_pLabPort, 3, 0);
	portSetLayout->addWidget(m_pCmbPort, 3, 1);
	portSetLayout->addWidget(m_pLabBaudRate, 4, 0);
	portSetLayout->addWidget(m_pCmbBaudRate, 4, 1);
	portSetLayout->addWidget(m_pLabDataBits, 5, 0);
	portSetLayout->addWidget(m_pCmbDataBits, 5, 1);
	portSetLayout->addWidget(m_pLabParity, 6, 0);
	portSetLayout->addWidget(m_pCmbParity, 6, 1);
	portSetLayout->addWidget(m_pLabStopBits, 7, 0);
	portSetLayout->addWidget(m_pCmbStopBits, 7, 1);
	portSetLayout->addWidget(m_pLabFlowControl, 8, 0);
	portSetLayout->addWidget(m_pCmbFlowControl, 8, 1);
	gboxPortSet->setLayout(portSetLayout);

	m_pTreeWgtClient = new QTreeWidget;
	m_pTreeWgtClient->setMaximumSize(190, 100);

	//接收设置框
	m_pRadbtnRecASCII = new QRadioButton(QStringLiteral("ASCII"));
	m_pRadbtnRecASCII->setChecked(true);
	m_pRadbtnRecHex = new QRadioButton(QStringLiteral("Hex"));
	m_pChkWordwrap = new QCheckBox(QStringLiteral("自动换行"));
	m_pChkShowSend = new QCheckBox(QStringLiteral("显示发送"));
	m_pChkShowTime = new QCheckBox(QStringLiteral("显示时间"));
	QGridLayout *recSetLayout = new QGridLayout;
	recSetLayout->addWidget(m_pRadbtnRecASCII, 0, 0);
	recSetLayout->addWidget(m_pRadbtnRecHex, 0, 1);
	recSetLayout->addWidget(m_pChkWordwrap, 1, 0);
	recSetLayout->addWidget(m_pChkShowSend, 2, 0);
	recSetLayout->addWidget(m_pChkShowTime, 3, 0);
	gboxReceiveSet->setLayout(recSetLayout);

	//发送设置
	m_pRadbtnSendASCII = new QRadioButton(QStringLiteral("ASCII"));
	m_pRadbtnSendASCII->setChecked(true);
	m_pRadbtnSendHex = new QRadioButton(QStringLiteral("Hex"));
	m_pChkRepeatSend = new QCheckBox(QStringLiteral("重复发送"));
	m_pSpboxTimes = new QSpinBox;
	m_pSpboxTimes->setMaximum(INT_MAX);
	m_pSpboxTimes->setValue(1000);
	QGridLayout *sendSetLayout = new QGridLayout;
	sendSetLayout->addWidget(m_pRadbtnSendASCII, 0, 0);
	sendSetLayout->addWidget(m_pRadbtnSendHex, 0, 1);
	sendSetLayout->addWidget(m_pChkRepeatSend, 1, 0);
	sendSetLayout->addWidget(m_pSpboxTimes, 1, 1);
	gboxSendSet->setLayout(sendSetLayout);

	//左边的布局
	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->addWidget(gboxPortSet);
	leftLayout->addWidget(m_pTreeWgtClient);
	leftLayout->addWidget(gboxReceiveSet);
	leftLayout->addWidget(gboxSendSet);
	leftLayout->addStretch(1);

	//用一个Widget 包含进去，方便进行隐藏显示
	m_pLeftSetWgt = new QWidget(this);
	m_pLeftSetWgt->setLayout(leftLayout);
	m_pLeftSetWgt->setMaximumWidth(220);

	m_pEditMsg = new QTextEdit;
	m_pBtnSend = new QPushButton(QStringLiteral("发送"));
	m_pEditSend = new QTextEdit;
	m_pCmbHistory = new QComboBox;

	//右边的布局
	QGridLayout *rightLayout = new QGridLayout();
	rightLayout->addWidget(m_pEditMsg, 0, 0, 1, 2);
	rightLayout->addWidget(m_pEditSend, 1, 0);
	rightLayout->addWidget(m_pBtnSend, 1, 1);
	rightLayout->addWidget(m_pCmbHistory, 2, 0, 1, 2);

	//size设置
	m_pCmbCommWay->setMinimumWidth(130);
	m_pEditSend->setMaximumHeight(70);

	//总布局
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addWidget(m_pLeftSetWgt);
	mainLayout->addLayout(rightLayout);
	m_pCenWidget->setLayout(mainLayout);
	this->setCentralWidget(m_pCenWidget);
}

/**
* @brief			初始化信号槽的连接
* @return			void
*/
void ChildWidget::initConnect()
{
	connect(this->m_pCmbCommWay, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ChildWidget::commWayChanged);//串口/网络切换
	connect(this->m_pCmbMode, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ChildWidget::tcpudpModeChanged);//client/server切换
	connect(this->m_pBtnSend, &QPushButton::clicked, this, &ChildWidget::sendBtnClicked);		//发送按钮
	connect(this->m_pCmbHistory, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ChildWidget::sendHistorySelcted);//选择历史发送消息
	connect(this->m_pChkRepeatSend, &QCheckBox::stateChanged, this, &ChildWidget::repeatSend);	//重复发送复选框
}

/**
* @brief			初始化状态栏
* @return			void
*/
void ChildWidget::initStatusBar()
{
	m_pLabConnInfo = new QLabel(QStringLiteral("COM1 CLOSED"));
	//m_pLabConnInfo->setStyleSheet("color:red");
	m_pLabRx = new QLabel(QStringLiteral("Rx:0 Bytes"));
	m_pLabTx = new QLabel(QStringLiteral("Tx:0 Bytes"));
	m_pLabPlace = new QLabel;

	
	QStatusBar *statusBar = this->statusBar();

	statusBar->addWidget(m_pLabConnInfo, 1);
	statusBar->addWidget(m_pLabRx, 1);
	statusBar->addWidget(m_pLabTx, 1);
	statusBar->addWidget(m_pLabPlace, 1);

	//文本设置为红色
	this->setLabelTextColor(m_pLabConnInfo, Qt::red);
}

/**
* @brief			初始化一些组合框的值
* @return			void
*/
void ChildWidget::addPortList()
{
	m_pCmbCommWay->addItem("TCP/UDP");

	Q_FOREACH(QSerialPortInfo portInfo, QSerialPortInfo::availablePorts())
	{
		m_pCmbCommWay->addItem(portInfo.portName());
	}

	m_pCmbMode->addItem(QStringLiteral("TCP Client"));
	m_pCmbMode->addItem(QStringLiteral("TCP Server"));

	//初始化串口的相关选项设置
	QStringList baudRateList = { "9600", "19200", "38400", "57600", "115200" };
	m_pCmbBaudRate->addItems(baudRateList);

	QStringList dataBitList = { "5", "6", "7", "8" };
	m_pCmbDataBits->addItems(dataBitList);

	QStringList parityList = { "None", "Even", "Odd",  "Space", "Mark" };
	m_pCmbParity->addItems(parityList);

	QStringList stopBitList = { "1", "1.5", "2" };
	m_pCmbStopBits->addItems(stopBitList);

	QStringList flowCtrlList = { "None", "Hardware", "Software" };
	m_pCmbFlowControl->addItems(flowCtrlList);

	m_pCmbIp->addItem("LocalHost");
	m_pCmbIp->addItem("127.0.0.1");
	m_pCmbIp->setCurrentIndex(0);

	m_pCmbPort->addItem("7000");
	m_pCmbPort->setCurrentIndex(0);

	m_pTreeWgtClient->setHeaderLabel("Connections");
}

/**
* @brief			窗体关闭事件
* @param event		关闭事件类
*
* @return			void
*/
void ChildWidget::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
}

/**
* @brief			连接方式组合框发生改变时
* @param index		组合框的当前index
*
* @return			void
*/
void ChildWidget::commWayChanged(int index)
{
	bool bVisible = (index == TCPUDP); //当前模式是TCP UDP

	m_pLabMode->setVisible(bVisible);
	m_pCmbMode->setVisible(bVisible);
	m_pLabIP->setVisible(bVisible);
	m_pCmbIp->setVisible(bVisible);
	m_pLabPort->setVisible(bVisible);
	m_pCmbPort->setVisible(bVisible);

	m_pLabBaudRate->setVisible(!bVisible);
	m_pLabDataBits->setVisible(!bVisible);
	m_pLabParity->setVisible(!bVisible);
	m_pLabStopBits->setVisible(!bVisible);
	m_pLabFlowControl->setVisible(!bVisible);

	m_pCmbBaudRate->setVisible(!bVisible);
	m_pCmbDataBits->setVisible(!bVisible);
	m_pCmbParity->setVisible(!bVisible);
	m_pCmbStopBits->setVisible(!bVisible);
	m_pCmbFlowControl->setVisible(!bVisible);
}

/**
* @brief			Server/Client方式组合框发生改变时
* @param index		组合框的当前index
*
* @return			void
*/
void ChildWidget::tcpudpModeChanged(int index)
{
	bool bVisible = (index == TCPCLIENT);

	m_pLabIP->setVisible(bVisible);
	m_pCmbIp->setVisible(bVisible);
	m_pTreeWgtClient->setVisible(!bVisible);
}

/**
* @brief			设置对应QLabel的字体颜色
* @param pLabel		label指针
* @param color		颜色
* @return			void
*/
void ChildWidget::setLabelTextColor(QLabel *pLabel, const QColor &color)
{
	QPalette palette;
	palette.setColor(QPalette::WindowText, color);
	pLabel->setPalette(palette);
}

/**
* @brief			发送按钮单击
* @return			void
*/
void ChildWidget::sendBtnClicked()
{
	if (this->m_pEditSend->toPlainText().isEmpty())
	{
		this->m_pEditSend->setFocus();

		return;
	}

	QString strData = this->m_pEditSend->toPlainText();

	bool bHexSend = this->m_pRadbtnSendHex->isChecked();		//发送方式是否为Hex?

																//转为16进制发送
	QByteArray hexData;
	if (bHexSend)
	{
		if (m_bSptNote)
		{
			//支持注释,不发送//以后的内容
			int index = strData.indexOf("//");
			if (index)
			{
				strData = strData.remove(index, strData.length() - index);
			}
		}
		hexData = ToolsHelp::QString2Hex(strData);  //strData.toUtf8().toHex().toUpper();
	}

	if (m_bDisplayColor)
	{
		//发送显示颜色
		bool ok;
		this->m_pEditMsg->setTextColor(QColor(QRgb(m_sendMsgColor.toInt(&ok, 16))));
	}

	if (m_curSendOpt == TcpServerSend)
	{
		//服务器发送
		if (m_pTcpServer && m_pTcpServer->getCurClientCount())
		{
			int length = 0;
			if (bHexSend)
			{
				length = m_pTcpServer->sendMessage(hexData);
			}
			else
			{
				QTextCodec *tc = QTextCodec::codecForName("GBK");	//处理中文乱码问题

																	//QString str  = tc->toUnicode(strData.toUtf8());
				length = m_pTcpServer->sendMessage(tc->fromUnicode(strData));
			}

			if (length > 0)
			{
				updateRxTxInfo(EnmUpdateRxTx::AddTx, length);
				showMessage(bHexSend ? hexData.toHex().toUpper() : strData);
			}
		}
	}
	else if (m_curSendOpt == TcpClientSend)
	{
		//客户端发送
		if (m_pTcpClient)
		{
			int length = 0;
			if (bHexSend)
			{
				length = m_pTcpClient->write(hexData);
			}
			else
			{
				QTextCodec *tc = QTextCodec::codecForName("GBK");	//处理中文乱码问题
				length = m_pTcpClient->write(tc->fromUnicode(strData));
			}

			if (length > 0)
			{
				showMessage(bHexSend ? hexData.toHex().toUpper() : strData);	//显示信息到界面
				updateRxTxInfo(EnmUpdateRxTx::AddTx, strData.toLocal8Bit().length());	//更新状态栏Tx信息

				appendToComboBox(m_pCmbHistory, strData);
			}
		}
	}
	else if (m_curSendOpt == SerialPortSend)
	{
		//串口发送
		if (m_pSerialPort)
		{
			int length = 0;
			if (bHexSend)
			{
				length = m_pSerialPort->write(hexData);
			}
			else
			{
				QTextCodec *tc = QTextCodec::codecForName("GBK");	//处理中文乱码问题
				length = m_pSerialPort->write(tc->fromUnicode(strData));
			}

			if (length > 0)
			{
				showMessage(bHexSend ? hexData.toHex().toUpper() : strData);	//显示信息到界面
				updateRxTxInfo(EnmUpdateRxTx::AddTx, strData.toLocal8Bit().length());	//更新状态栏Tx信息

				appendToComboBox(m_pCmbHistory, strData);
			}
		}
	}
	else
	{
		//当前发送按钮为"打开"  直接调用打开按钮单击事件
		if (!m_pRepeatSendTimer)
		{
			doConnect();
		}
	}
}

/**
* @brief			 选择历史发送消息
* @return			 void
*/
void ChildWidget::sendHistorySelcted()
{
	this->m_pEditSend->setText(m_pCmbHistory->currentText());
}

/**
* @brief			把数据添加到指定组合框
* @param pCmbBox	QComboBox指针
* @param strMsg		数据
* @return			void
*/
void ChildWidget::appendToComboBox(QComboBox *pCmbBox, const QString &strMsg)
{
	bool bExist = false;	//是否已经有该值

	for (int i = 0; i < pCmbBox->count(); ++i)
	{
		if (pCmbBox->itemText(i) == strMsg)
		{
			bExist = true;
			break;
		}
	}

	if (!bExist)
	{
		pCmbBox->addItem(strMsg);
	}
}

/**
* @brief			 选择重复发送
* @param iSend		 是否选中 2:选中
* @return			 void
*/
void ChildWidget::repeatSend(int iSend)
{
	//勾选
	if (iSend == Qt::Checked)
	{
		if (m_pRepeatSendTimer)
		{
			m_pRepeatSendTimer->stop();
			QObject::disconnect(m_pRepeatSendTimer, 0, 0, 0);
			m_pRepeatSendTimer->deleteLater();
			m_pRepeatSendTimer = nullptr;
		}

		//小于50ms 自动设置为50ms
		int interval = this->m_pSpboxTimes->value() < 50 ? 50 : this->m_pSpboxTimes->value();

		m_pRepeatSendTimer = new QTimer(this);
		connect(m_pRepeatSendTimer, &QTimer::timeout, this, &ChildWidget::sendBtnClicked);	//绑定到发送槽
		m_pRepeatSendTimer->setInterval(interval);
		m_pRepeatSendTimer->start();
	}
	else
	{
		if (m_pRepeatSendTimer)
		{
			m_pRepeatSendTimer->stop();
			QObject::disconnect(m_pRepeatSendTimer, 0, 0, 0);
			m_pRepeatSendTimer->deleteLater();
			m_pRepeatSendTimer = nullptr;
		}
	}
}

/**
* @brief			 更新状态栏的RxTx信息
* @param state		 Rx or Tx
* @param count		 更新的字节数
* @return			 void
*/
void ChildWidget::updateRxTxInfo(EnmUpdateRxTx state, int count)
{
	if (state == EnmUpdateRxTx::ClearRxTx)
	{
		//清空计数
		m_pLabRx->setText("Rx:0 Bytes");
		m_pLabTx->setText("Tx:0 Bytes");
		m_iTxCount = 0;
		m_iRxCount = 0;

		return;
	}
	else if (state == EnmUpdateRxTx::AddRx)
	{
		m_iRxCount += count;
		QString strInfo = QString("Rx:%0 Bytes").arg(m_iRxCount);

		m_pLabRx->setText(strInfo);
	}
	else if (state == EnmUpdateRxTx::AddTx)
	{
		m_iTxCount += count;
		QString strInfo = QString("Tx:%0 Bytes").arg(m_iTxCount);

		m_pLabTx->setText(strInfo);
	}

	//else unknown
}

/**
* @brief			 服务器接收到来自客户端的消息，更新界面
* @return			 void
*/
void ChildWidget::readDataFromClient(QByteArray strMsg)
{
	if (m_bDisplayColor)
	{
		//发送显示颜色
		bool ok;
		this->m_pEditMsg->setTextColor(QColor(QRgb(m_receMsgColor.toInt(&ok, 16))));
	}

	if (m_pRadbtnRecHex->isChecked())
	{
		showMessage(strMsg.toHex().toUpper());
	}
	else
	{
		QTextCodec *tc = QTextCodec::codecForName("GBK");//处理中文乱码问题
		showMessage(tc->toUnicode(strMsg));
	}

	updateRxTxInfo(EnmUpdateRxTx::AddRx, strMsg.length());
}

/**
* @brief			新的客户端连接，保存并显示
* @param strAddr	客户端的IP地址
* @param strAddr	客户端的端口
* @return			void
*/
void ChildWidget::newConnectFromServer(QString strAddr, quint16 iport)
{
	strAddr = strAddr + ":" + QString::number(iport);

	//保存到列表中，方便断连时删除
	m_mapClient[iport] = strAddr;

	QStringList addrList;
	addrList.append(strAddr);

	this->m_pTreeWgtClient->addTopLevelItem(new QTreeWidgetItem(addrList));
}

/**
* @brief			 接收到服务器发来的数据
* @return			 void
*/
void ChildWidget::readDataFromServer()
{
	QByteArray strData = m_pTcpClient->readAll();

	if (m_bDisplayColor)
	{
		//发送显示颜色
		bool ok;
		this->m_pEditMsg->setTextColor(QColor(QRgb(m_receMsgColor.toInt(&ok, 16))));
	}

	if (m_pRadbtnRecHex->isChecked())
	{
		showMessage(strData.toHex().toUpper());
	}
	else
	{
		QTextCodec *tc = QTextCodec::codecForName("GBK");//处理中文乱码问题
		showMessage(tc->toUnicode(strData));
	}

	updateRxTxInfo(EnmUpdateRxTx::AddRx, strData.length());
}

/**
* @brief			客户端断开连接，更新界面
* @param strAddr	客户端的IP地址
* @param strAddr	客户端的端口
* @return			void
*/
void ChildWidget::disconnectedFromServer(QString strAddr, int iport)
{
	//异常！没有该客户端存储的信息
	if (!m_mapClient.contains(iport))
	{
		showMessage(QStringLiteral("断开的客户端不存有"), EnmLogOption::Console | EnmLogOption::LogFile);

		return;
	}

	m_mapClient.remove(iport);

	QString strItem = strAddr + ":" + QString::number(iport);	//treeitem中的名字

																//treewidget中移除它
	QTreeWidgetItemIterator itr(m_pTreeWgtClient);
	while (*itr)
	{
		if (strItem == (*itr)->text(0))
		{
			delete (*itr);
		}
		++itr;
	}
}

/**
* @brief			 连接的客户端发生错误
* @param socketError 错误信息
* @return			 void
*/
void ChildWidget::clientSocketError(QAbstractSocket::SocketError socketError)
{
	//这里最好通知下主界面工具栏更新下。
	//...

	setLabelTextColor(m_pLabConnInfo, Qt::red);
	m_pLabConnInfo->setText(m_pTcpClient->errorString());
	
	//closeBtnClicked();	//直接调用断开方法，直接删除socket
}

/**
* @brief			接收到串口发送的消息
* @return			void
*/
void ChildWidget::readDataFromSerialPort()
{
	QByteArray strData = m_pSerialPort->readAll();

	if (m_bDisplayColor)
	{
		//发送显示颜色
		bool ok;
		this->m_pEditMsg->setTextColor(QColor(QRgb(m_receMsgColor.toInt(&ok, 16))));
	}

	if (m_pRadbtnRecHex->isChecked())
	{
		showMessage(strData.toHex().toUpper());
	}
	else
	{
		QTextCodec *tc = QTextCodec::codecForName("GBK");//处理中文乱码问题
		showMessage(tc->toUnicode(strData));
	}

	updateRxTxInfo(EnmUpdateRxTx::AddRx, strData.length());
}

/**
* @brief			串口发生错误 (Qt5.8 以后可用)
* @return			void
*/
void ChildWidget::serialPortError(QSerialPort::SerialPortError error)
{
	setLabelTextColor(m_pLabConnInfo, Qt::red);
	m_pLabConnInfo->setText(m_pSerialPort->errorString());

	//closeBtnClicked();	//直接调用断开方法，直接删除socket
}

/**
* @brief			显示消息到界面/日志/控制台
* @param strMsg		显示的消息
* @param ways		显示方式
* @return			void
*/
void ChildWidget::showMessage(QString strMsg, EnmLogOptions ways)
{
	if (ways == 0)
	{
		return;
	}

	QString strTime;
	if (!m_strTimeFormat.isEmpty())
	{
		strTime = QTime::currentTime().toString(m_strTimeFormat);
	}
	

	if (m_pChkShowTime->isChecked() && m_pChkWordwrap->isChecked())	//显示时间 自动换行
	{
		strMsg = strTime + " " + strMsg;
	}

	if (ways & EnmLogOption::Console)
	{
		qDebug() << strMsg;
	}

	if (ways & EnmLogOption::Interface)
	{
		if (m_curSendOpt == EnmCurSendOption::TcpClientSend)
		{
			//客户端状态下
			if (m_pChkShowSend->isChecked())	//显示发送
			{
				if (m_pChkWordwrap->isChecked())	//自动换行
				{
					this->m_pEditMsg->append(strMsg);
				}
				else
				{
					this->m_pEditMsg->insertPlainText(strMsg);
					this->m_pEditMsg->moveCursor(QTextCursor::End);
				}
			}
		}
		else
		{
			this->m_pEditMsg->append(strMsg);
		}
	}

	if (ways & EnmLogOption::LogFile)
	{
		//打印到日志
		if (m_bSaveLog)
		{
			Logger(strMsg, m_strLogPath);
		}
	}
}

/**
* @brief			根据时间单位来计算重复时间
* @param times		时间
* @param timeSuf	时间单位
* @return			void
*/
void ChildWidget::setRepeatTimes(int times, EnmTimeSuffix timeSuf)
{
	if (timeSuf == EnmTimeSuffix::MilliSecond)
	{
		m_pSpboxTimes->setValue(times);
	}
	else if (timeSuf == EnmTimeSuffix::Second)
	{
		int secTimes = times * 1000;
		m_pSpboxTimes->setValue(secTimes);
	}
	else if (timeSuf == EnmTimeSuffix::Minute)
	{
		int secTimes = times * 60 * 1000;
		m_pSpboxTimes->setValue(secTimes);
	}
	else if (timeSuf == EnmTimeSuffix::Hour)
	{
		int secTimes = times * 60 * 60 * 1000;
		m_pSpboxTimes->setValue(secTimes);
	}
}