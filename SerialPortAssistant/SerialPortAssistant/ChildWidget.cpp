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

	//!DEBUG ������ѡ��ѡ��
	m_pChkShowSend->setChecked(true);
	m_pChkShowTime->setChecked(true);
	m_pChkWordwrap->setChecked(true);
}

ChildWidget::~ChildWidget()
{
	qDebug() << QStringLiteral("ͨ��ɾ��");
}


/**
* @brief		��ʼ��������Ϣ
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
	this->m_msgFont = "Fixedsys,12";		//�������ø�Ĭ��ֵ
	this->m_receMsgColor = "000000";
	this->m_sendMsgColor = "000000";
}

/**
* @brief		�����浥�����Ӱ�ť����
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
			QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("��д��ȷ�Ķ˿�"));
			this->m_pCmbPort->setFocus();
			return false;
		}
		//����˼���
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
				QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("TCP Server��ʧ��\n") + m_pTcpServer->errorString());
				m_pTcpServer->close();
				delete m_pTcpServer;
				m_pTcpServer = nullptr;

				return false;
			}

			connect(m_pTcpServer, &TcpServer::updateMessage, this, &ChildWidget::readDataFromClient);	//�ӵ�client�����������ݣ����½���
			connect(m_pTcpServer, &TcpServer::newConnect, this, &ChildWidget::newConnectFromServer);
			connect(m_pTcpServer, &TcpServer::clientDisconnected, this, &ChildWidget::disconnectedFromServer);

			m_curSendOpt = TcpServerSend;

			QString strConn = "Waiting for Connection on " + QString("%0").arg(iPort) + "...";

			this->setLabelTextColor(m_pLabConnInfo, Qt::darkGreen);
			this->m_pLabConnInfo->setText(strConn);
		}
		else
		{
			//�ͻ���
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
				QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("��д��ȷ��IP��ַ"));
				this->m_pCmbIp->setFocus();

				return false;
			}

			quint16 iport = 0;

			iport = m_pCmbPort->currentText().toInt(&bok);
			if (!bok)
			{
				QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("��д��ȷ�Ķ˿�"), QMessageBox::Ok);
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
					//���ӷ�����ʧ��
					m_pLabConnInfo->setText(m_pTcpClient->errorString());
					delete m_pTcpClient;
					m_pTcpClient = nullptr;

					return false;
				}
				m_curSendOpt = TcpClientSend;		//��ǰ״̬���Ϊclient

				appendToComboBox(m_pCmbIp, strIp);

				connect(m_pTcpClient, &QTcpSocket::readyRead, this, &ChildWidget::readDataFromServer);
				connect(m_pTcpClient, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &ChildWidget::clientSocketError);	//����������
				this->setLabelTextColor(m_pLabConnInfo, Qt::darkGreen);
				m_pLabConnInfo->setText("Connection is established");
			}
		}
	}
	else
	{
		//����ģʽ
		if (m_pSerialPort)
		{
			disconnect(m_pSerialPort, 0, 0, 0);
			m_pSerialPort->close();
			delete m_pSerialPort;
			m_pSerialPort = nullptr;
		}
		m_pSerialPort = new QSerialPort(this);

		m_pSerialPort->setPortName(m_pCmbCommWay->currentText());									//���ô��ں�
		m_pSerialPort->setBaudRate(m_pCmbBaudRate->currentText().toInt());							//���ò�����
		m_pSerialPort->setDataBits((QSerialPort::DataBits)m_pCmbDataBits->currentText().toInt());	//����λ ֱ��ת(���Ͻ�)
		m_pSerialPort->setParity((QSerialPort::Parity)m_pCmbParity->currentIndex());				//У��λ ֱ��ת 
		m_pSerialPort->setStopBits((QSerialPort::StopBits)m_pCmbStopBits->currentIndex());			//ֹͣλ
		m_pSerialPort->setFlowControl((QSerialPort::FlowControl)m_pCmbFlowControl->currentIndex()); //������ 

		if (!m_pSerialPort->open(QIODevice::ReadWrite))
		{
			qDebug() << m_pSerialPort->errorString();
			QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("���ڴ�ʧ��\n") + m_pSerialPort->errorString());

			delete m_pSerialPort;
			m_pSerialPort = nullptr;

			setLabelTextColor(m_pLabConnInfo, Qt::red);
			m_pLabConnInfo->setText(QString("%1 CLOSED").arg(m_pCmbCommWay->currentText()));

			return false;
		}

		connect(m_pSerialPort, &QSerialPort::readyRead, this, &ChildWidget::readDataFromSerialPort);
		connect(m_pSerialPort, &QSerialPort::errorOccurred, this, &ChildWidget::serialPortError);

		//�ı�״̬����ʾ��Ϣ
		m_curSendOpt = SerialPortSend;

		QString strConn = QString("%1 OPENED, %2, %3, %4, %5, %6").arg(m_pCmbPort->currentText()).arg(m_pCmbBaudRate->currentText())
			.arg(m_pCmbDataBits->currentText()).arg(m_pCmbParity->currentText())
			.arg(m_pCmbStopBits->currentText()).arg(m_pCmbFlowControl->currentText());

		this->setLabelTextColor(m_pLabConnInfo, Qt::darkGreen);
		this->m_pLabConnInfo->setText(strConn);
	}

	m_pBtnSend->setText(QStringLiteral("����"));
	return true;
}

/**
* @brief		�����浥���Ͽ���ť����
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
		//������״̬��
		QObject::disconnect(m_pTcpServer, 0, 0, 0);
		m_pTcpServer->clearClient();	//����Ѿ����ӵĿͻ���
		m_pTcpServer->close();
		m_pTcpServer->deleteLater();
		m_pTcpServer = nullptr;

		//��ձ������Ϣ
		m_mapClient.clear();
		//��ս�������������Ϣ
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

	m_curSendOpt = EnmCurSendOption::NoneSend;	//�Ͽ����Ӻ�ǵðѵ�ǰ״̬�ı�

	m_pBtnSend->setText(QStringLiteral("��")); //���Ͱ�ť��Ϊ��
}

/**
* @brief		���յ�������������ť
* @return		void
*/
void ChildWidget::doClearEdit()
{
	m_pEditMsg->clear();
}

/**
* @brief			 ����״̬����RxTx��Ϣ
* @param bOn		 �Ƿ��
* @return			 void
*/
void ChildWidget::doQuckSetting(bool bOn)
{
	//���ػ���ʾ��ߴ��弴��
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
* @brief			 ������־·��
* @param logPath	 ��־·��
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
* @brief			 �Ƿ����־
* @param bOn		 �Ƿ��
* @return			 void
*/
void ChildWidget::setLogState(bool bOn)
{
	m_bSaveLog = bOn;
}

/**
* @brief			�ָ���ʼ״̬
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
* @brief			��ʼ�����õ���Ϣ public
* @param collect	���õ���Ϣ
*
* @return			void
*/
void ChildWidget::initSetting(const SettingCollect &setInfo)
{
	if (setInfo.conOpt == EnmConnOption::SerialPortOpt)
	{
		//����������Ϣ����
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
	//���ý��յ����塣��
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
* @brief			��ʼ�����õ���Ϣ public
* @return			���õ���Ϣ
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
* @brief			��ȡ���ӵķ�ʽ ����/TCPServer/TCPClient
* @return			���ӵķ�ʽ
*/
EnmConnOption ChildWidget::getConnWay()
{
	if (m_pCmbCommWay->currentIndex() == 0)
	{
		if (m_pCmbMode->currentIndex() == 0)
		{
			//�ͻ���
			return EnmConnOption::TcpClientOpt;
		}
		else
		{
			//������
			return EnmConnOption::TcpServerOpt;
		}
	}
	else
	{
		return EnmConnOption::SerialPortOpt;
	}
}

/**
* @brief			���ô��ڵ���Ϣ
* @param collect	���õ���Ϣ
*
* @return			void
*/
void ChildWidget::setSerialPortInfo(const SettingCollect &setInfo)
{
	//���ö˿���
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
* @brief			���ô���
* @return			void
*/
void ChildWidget::createCenWgt()
{
	m_pCenWidget = new QWidget;

	QGroupBox *gboxPortSet = new QGroupBox(QStringLiteral("��������"));
	QGroupBox *gboxReceiveSet = new QGroupBox(QStringLiteral("��������"));
	QGroupBox *gboxSendSet = new QGroupBox(QStringLiteral("��������"));

	//�������ÿ�
	QLabel *labCommWay = new QLabel(QStringLiteral("����:"));
	m_pLabMode = new QLabel(QStringLiteral("Mode:"));
	m_pLabIP = new QLabel(QStringLiteral("IP:"));
	m_pLabPort = new QLabel(QStringLiteral("Port:"));
	m_pLabBaudRate = new QLabel(QStringLiteral("������:"));
	m_pLabDataBits = new QLabel(QStringLiteral("����λ:"));
	m_pLabParity = new QLabel(QStringLiteral("У��λ:"));
	m_pLabStopBits = new QLabel(QStringLiteral("ֹͣλ:"));
	m_pLabFlowControl = new QLabel(QStringLiteral("��  ��:"));
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

	//�������ÿ�
	m_pRadbtnRecASCII = new QRadioButton(QStringLiteral("ASCII"));
	m_pRadbtnRecASCII->setChecked(true);
	m_pRadbtnRecHex = new QRadioButton(QStringLiteral("Hex"));
	m_pChkWordwrap = new QCheckBox(QStringLiteral("�Զ�����"));
	m_pChkShowSend = new QCheckBox(QStringLiteral("��ʾ����"));
	m_pChkShowTime = new QCheckBox(QStringLiteral("��ʾʱ��"));
	QGridLayout *recSetLayout = new QGridLayout;
	recSetLayout->addWidget(m_pRadbtnRecASCII, 0, 0);
	recSetLayout->addWidget(m_pRadbtnRecHex, 0, 1);
	recSetLayout->addWidget(m_pChkWordwrap, 1, 0);
	recSetLayout->addWidget(m_pChkShowSend, 2, 0);
	recSetLayout->addWidget(m_pChkShowTime, 3, 0);
	gboxReceiveSet->setLayout(recSetLayout);

	//��������
	m_pRadbtnSendASCII = new QRadioButton(QStringLiteral("ASCII"));
	m_pRadbtnSendASCII->setChecked(true);
	m_pRadbtnSendHex = new QRadioButton(QStringLiteral("Hex"));
	m_pChkRepeatSend = new QCheckBox(QStringLiteral("�ظ�����"));
	m_pSpboxTimes = new QSpinBox;
	m_pSpboxTimes->setMaximum(INT_MAX);
	m_pSpboxTimes->setValue(1000);
	QGridLayout *sendSetLayout = new QGridLayout;
	sendSetLayout->addWidget(m_pRadbtnSendASCII, 0, 0);
	sendSetLayout->addWidget(m_pRadbtnSendHex, 0, 1);
	sendSetLayout->addWidget(m_pChkRepeatSend, 1, 0);
	sendSetLayout->addWidget(m_pSpboxTimes, 1, 1);
	gboxSendSet->setLayout(sendSetLayout);

	//��ߵĲ���
	QVBoxLayout *leftLayout = new QVBoxLayout();
	leftLayout->addWidget(gboxPortSet);
	leftLayout->addWidget(m_pTreeWgtClient);
	leftLayout->addWidget(gboxReceiveSet);
	leftLayout->addWidget(gboxSendSet);
	leftLayout->addStretch(1);

	//��һ��Widget ������ȥ���������������ʾ
	m_pLeftSetWgt = new QWidget(this);
	m_pLeftSetWgt->setLayout(leftLayout);
	m_pLeftSetWgt->setMaximumWidth(220);

	m_pEditMsg = new QTextEdit;
	m_pBtnSend = new QPushButton(QStringLiteral("����"));
	m_pEditSend = new QTextEdit;
	m_pCmbHistory = new QComboBox;

	//�ұߵĲ���
	QGridLayout *rightLayout = new QGridLayout();
	rightLayout->addWidget(m_pEditMsg, 0, 0, 1, 2);
	rightLayout->addWidget(m_pEditSend, 1, 0);
	rightLayout->addWidget(m_pBtnSend, 1, 1);
	rightLayout->addWidget(m_pCmbHistory, 2, 0, 1, 2);

	//size����
	m_pCmbCommWay->setMinimumWidth(130);
	m_pEditSend->setMaximumHeight(70);

	//�ܲ���
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addWidget(m_pLeftSetWgt);
	mainLayout->addLayout(rightLayout);
	m_pCenWidget->setLayout(mainLayout);
	this->setCentralWidget(m_pCenWidget);
}

/**
* @brief			��ʼ���źŲ۵�����
* @return			void
*/
void ChildWidget::initConnect()
{
	connect(this->m_pCmbCommWay, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ChildWidget::commWayChanged);//����/�����л�
	connect(this->m_pCmbMode, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ChildWidget::tcpudpModeChanged);//client/server�л�
	connect(this->m_pBtnSend, &QPushButton::clicked, this, &ChildWidget::sendBtnClicked);		//���Ͱ�ť
	connect(this->m_pCmbHistory, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ChildWidget::sendHistorySelcted);//ѡ����ʷ������Ϣ
	connect(this->m_pChkRepeatSend, &QCheckBox::stateChanged, this, &ChildWidget::repeatSend);	//�ظ����͸�ѡ��
}

/**
* @brief			��ʼ��״̬��
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

	//�ı�����Ϊ��ɫ
	this->setLabelTextColor(m_pLabConnInfo, Qt::red);
}

/**
* @brief			��ʼ��һЩ��Ͽ��ֵ
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

	//��ʼ�����ڵ����ѡ������
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
* @brief			����ر��¼�
* @param event		�ر��¼���
*
* @return			void
*/
void ChildWidget::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
}

/**
* @brief			���ӷ�ʽ��Ͽ����ı�ʱ
* @param index		��Ͽ�ĵ�ǰindex
*
* @return			void
*/
void ChildWidget::commWayChanged(int index)
{
	bool bVisible = (index == TCPUDP); //��ǰģʽ��TCP UDP

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
* @brief			Server/Client��ʽ��Ͽ����ı�ʱ
* @param index		��Ͽ�ĵ�ǰindex
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
* @brief			���ö�ӦQLabel��������ɫ
* @param pLabel		labelָ��
* @param color		��ɫ
* @return			void
*/
void ChildWidget::setLabelTextColor(QLabel *pLabel, const QColor &color)
{
	QPalette palette;
	palette.setColor(QPalette::WindowText, color);
	pLabel->setPalette(palette);
}

/**
* @brief			���Ͱ�ť����
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

	bool bHexSend = this->m_pRadbtnSendHex->isChecked();		//���ͷ�ʽ�Ƿ�ΪHex?

																//תΪ16���Ʒ���
	QByteArray hexData;
	if (bHexSend)
	{
		if (m_bSptNote)
		{
			//֧��ע��,������//�Ժ������
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
		//������ʾ��ɫ
		bool ok;
		this->m_pEditMsg->setTextColor(QColor(QRgb(m_sendMsgColor.toInt(&ok, 16))));
	}

	if (m_curSendOpt == TcpServerSend)
	{
		//����������
		if (m_pTcpServer && m_pTcpServer->getCurClientCount())
		{
			int length = 0;
			if (bHexSend)
			{
				length = m_pTcpServer->sendMessage(hexData);
			}
			else
			{
				QTextCodec *tc = QTextCodec::codecForName("GBK");	//����������������

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
		//�ͻ��˷���
		if (m_pTcpClient)
		{
			int length = 0;
			if (bHexSend)
			{
				length = m_pTcpClient->write(hexData);
			}
			else
			{
				QTextCodec *tc = QTextCodec::codecForName("GBK");	//����������������
				length = m_pTcpClient->write(tc->fromUnicode(strData));
			}

			if (length > 0)
			{
				showMessage(bHexSend ? hexData.toHex().toUpper() : strData);	//��ʾ��Ϣ������
				updateRxTxInfo(EnmUpdateRxTx::AddTx, strData.toLocal8Bit().length());	//����״̬��Tx��Ϣ

				appendToComboBox(m_pCmbHistory, strData);
			}
		}
	}
	else if (m_curSendOpt == SerialPortSend)
	{
		//���ڷ���
		if (m_pSerialPort)
		{
			int length = 0;
			if (bHexSend)
			{
				length = m_pSerialPort->write(hexData);
			}
			else
			{
				QTextCodec *tc = QTextCodec::codecForName("GBK");	//����������������
				length = m_pSerialPort->write(tc->fromUnicode(strData));
			}

			if (length > 0)
			{
				showMessage(bHexSend ? hexData.toHex().toUpper() : strData);	//��ʾ��Ϣ������
				updateRxTxInfo(EnmUpdateRxTx::AddTx, strData.toLocal8Bit().length());	//����״̬��Tx��Ϣ

				appendToComboBox(m_pCmbHistory, strData);
			}
		}
	}
	else
	{
		//��ǰ���Ͱ�ťΪ"��"  ֱ�ӵ��ô򿪰�ť�����¼�
		if (!m_pRepeatSendTimer)
		{
			doConnect();
		}
	}
}

/**
* @brief			 ѡ����ʷ������Ϣ
* @return			 void
*/
void ChildWidget::sendHistorySelcted()
{
	this->m_pEditSend->setText(m_pCmbHistory->currentText());
}

/**
* @brief			��������ӵ�ָ����Ͽ�
* @param pCmbBox	QComboBoxָ��
* @param strMsg		����
* @return			void
*/
void ChildWidget::appendToComboBox(QComboBox *pCmbBox, const QString &strMsg)
{
	bool bExist = false;	//�Ƿ��Ѿ��и�ֵ

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
* @brief			 ѡ���ظ�����
* @param iSend		 �Ƿ�ѡ�� 2:ѡ��
* @return			 void
*/
void ChildWidget::repeatSend(int iSend)
{
	//��ѡ
	if (iSend == Qt::Checked)
	{
		if (m_pRepeatSendTimer)
		{
			m_pRepeatSendTimer->stop();
			QObject::disconnect(m_pRepeatSendTimer, 0, 0, 0);
			m_pRepeatSendTimer->deleteLater();
			m_pRepeatSendTimer = nullptr;
		}

		//С��50ms �Զ�����Ϊ50ms
		int interval = this->m_pSpboxTimes->value() < 50 ? 50 : this->m_pSpboxTimes->value();

		m_pRepeatSendTimer = new QTimer(this);
		connect(m_pRepeatSendTimer, &QTimer::timeout, this, &ChildWidget::sendBtnClicked);	//�󶨵����Ͳ�
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
* @brief			 ����״̬����RxTx��Ϣ
* @param state		 Rx or Tx
* @param count		 ���µ��ֽ���
* @return			 void
*/
void ChildWidget::updateRxTxInfo(EnmUpdateRxTx state, int count)
{
	if (state == EnmUpdateRxTx::ClearRxTx)
	{
		//��ռ���
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
* @brief			 ���������յ����Կͻ��˵���Ϣ�����½���
* @return			 void
*/
void ChildWidget::readDataFromClient(QByteArray strMsg)
{
	if (m_bDisplayColor)
	{
		//������ʾ��ɫ
		bool ok;
		this->m_pEditMsg->setTextColor(QColor(QRgb(m_receMsgColor.toInt(&ok, 16))));
	}

	if (m_pRadbtnRecHex->isChecked())
	{
		showMessage(strMsg.toHex().toUpper());
	}
	else
	{
		QTextCodec *tc = QTextCodec::codecForName("GBK");//����������������
		showMessage(tc->toUnicode(strMsg));
	}

	updateRxTxInfo(EnmUpdateRxTx::AddRx, strMsg.length());
}

/**
* @brief			�µĿͻ������ӣ����沢��ʾ
* @param strAddr	�ͻ��˵�IP��ַ
* @param strAddr	�ͻ��˵Ķ˿�
* @return			void
*/
void ChildWidget::newConnectFromServer(QString strAddr, quint16 iport)
{
	strAddr = strAddr + ":" + QString::number(iport);

	//���浽�б��У��������ʱɾ��
	m_mapClient[iport] = strAddr;

	QStringList addrList;
	addrList.append(strAddr);

	this->m_pTreeWgtClient->addTopLevelItem(new QTreeWidgetItem(addrList));
}

/**
* @brief			 ���յ�����������������
* @return			 void
*/
void ChildWidget::readDataFromServer()
{
	QByteArray strData = m_pTcpClient->readAll();

	if (m_bDisplayColor)
	{
		//������ʾ��ɫ
		bool ok;
		this->m_pEditMsg->setTextColor(QColor(QRgb(m_receMsgColor.toInt(&ok, 16))));
	}

	if (m_pRadbtnRecHex->isChecked())
	{
		showMessage(strData.toHex().toUpper());
	}
	else
	{
		QTextCodec *tc = QTextCodec::codecForName("GBK");//����������������
		showMessage(tc->toUnicode(strData));
	}

	updateRxTxInfo(EnmUpdateRxTx::AddRx, strData.length());
}

/**
* @brief			�ͻ��˶Ͽ����ӣ����½���
* @param strAddr	�ͻ��˵�IP��ַ
* @param strAddr	�ͻ��˵Ķ˿�
* @return			void
*/
void ChildWidget::disconnectedFromServer(QString strAddr, int iport)
{
	//�쳣��û�иÿͻ��˴洢����Ϣ
	if (!m_mapClient.contains(iport))
	{
		showMessage(QStringLiteral("�Ͽ��Ŀͻ��˲�����"), EnmLogOption::Console | EnmLogOption::LogFile);

		return;
	}

	m_mapClient.remove(iport);

	QString strItem = strAddr + ":" + QString::number(iport);	//treeitem�е�����

																//treewidget���Ƴ���
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
* @brief			 ���ӵĿͻ��˷�������
* @param socketError ������Ϣ
* @return			 void
*/
void ChildWidget::clientSocketError(QAbstractSocket::SocketError socketError)
{
	//�������֪ͨ�������湤���������¡�
	//...

	setLabelTextColor(m_pLabConnInfo, Qt::red);
	m_pLabConnInfo->setText(m_pTcpClient->errorString());
	
	//closeBtnClicked();	//ֱ�ӵ��öϿ�������ֱ��ɾ��socket
}

/**
* @brief			���յ����ڷ��͵���Ϣ
* @return			void
*/
void ChildWidget::readDataFromSerialPort()
{
	QByteArray strData = m_pSerialPort->readAll();

	if (m_bDisplayColor)
	{
		//������ʾ��ɫ
		bool ok;
		this->m_pEditMsg->setTextColor(QColor(QRgb(m_receMsgColor.toInt(&ok, 16))));
	}

	if (m_pRadbtnRecHex->isChecked())
	{
		showMessage(strData.toHex().toUpper());
	}
	else
	{
		QTextCodec *tc = QTextCodec::codecForName("GBK");//����������������
		showMessage(tc->toUnicode(strData));
	}

	updateRxTxInfo(EnmUpdateRxTx::AddRx, strData.length());
}

/**
* @brief			���ڷ������� (Qt5.8 �Ժ����)
* @return			void
*/
void ChildWidget::serialPortError(QSerialPort::SerialPortError error)
{
	setLabelTextColor(m_pLabConnInfo, Qt::red);
	m_pLabConnInfo->setText(m_pSerialPort->errorString());

	//closeBtnClicked();	//ֱ�ӵ��öϿ�������ֱ��ɾ��socket
}

/**
* @brief			��ʾ��Ϣ������/��־/����̨
* @param strMsg		��ʾ����Ϣ
* @param ways		��ʾ��ʽ
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
	

	if (m_pChkShowTime->isChecked() && m_pChkWordwrap->isChecked())	//��ʾʱ�� �Զ�����
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
			//�ͻ���״̬��
			if (m_pChkShowSend->isChecked())	//��ʾ����
			{
				if (m_pChkWordwrap->isChecked())	//�Զ�����
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
		//��ӡ����־
		if (m_bSaveLog)
		{
			Logger(strMsg, m_strLogPath);
		}
	}
}

/**
* @brief			����ʱ�䵥λ�������ظ�ʱ��
* @param times		ʱ��
* @param timeSuf	ʱ�䵥λ
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