#include "SettingDlg.h"
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSerialPortInfo>
#include <QGroupBox>
#include <QFontDialog>
#include <QDebug>
#include <QColorDialog>
#include <QDateTime>
#include <QRgb>

#include "Global.h"

SettingDlg::SettingDlg(QWidget *parent)
	: QDialog(parent)
{
	initViewer();
	initConnect();
	addPortList();
}

SettingDlg::~SettingDlg()
{
}

/**
* @brief		初始化信窗体和一些信息
* @return		void
*/
void SettingDlg::initViewer()
{
	this->setWindowTitle(QStringLiteral("设置"));

	/* 串口选项 */
	m_pWgtConnway = new QWidget();
	QLabel *labCommWay = new QLabel(QStringLiteral("串口:"));
	labCommWay->setMaximumWidth(40);
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

	QVBoxLayout *portExtendLayout = new QVBoxLayout();
	portExtendLayout->addLayout(portSetLayout);
	portExtendLayout->addStretch();

	m_pWgtConnway->setLayout(portExtendLayout);

	/* 接收选项 */
	m_pWgtReceive = new QWidget();
	m_pRadbtnRecASCII = new QRadioButton(QStringLiteral("ASCII"));
	m_pRadbtnRecASCII->setChecked(true);
	m_pRadbtnRecHex = new QRadioButton(QStringLiteral("Hex"));
	m_pChkRepeatSend = new QCheckBox(QStringLiteral("重复发送"));
	m_pTimeUint = new QComboBox;
	m_pSpboxTimes = new QSpinBox;
	m_pSpboxTimes->setMaximum(INT_MAX);
	m_pSpboxTimes->setValue(1000);
	m_pChkNote = new QCheckBox(QStringLiteral("支持注释(Hex模式下不发送 // 之后的内容)"));
	m_pChkSendFormat = new QCheckBox(QStringLiteral("发送格式化 (010203 -> 01 02 03)"));
	QGroupBox *boxLineMode = new QGroupBox("Line Ending Mode");
	m_pRadbtnNoLine = new QRadioButton("No Line ending");
	m_pRadbtnNewLine = new QRadioButton("NewLine");
	m_pRadbtnCarrRet = new QRadioButton("Carriage return");
	m_pRadbtnBothNLCR = new QRadioButton("Both NL & CR");

	//ASCII Hex布局
	QHBoxLayout *receModeLayout = new QHBoxLayout;
	receModeLayout->addWidget(m_pRadbtnRecASCII);
	receModeLayout->addWidget(m_pRadbtnRecHex);
	receModeLayout->addStretch();

	//重复发送的布局
	QHBoxLayout *receRepeatLayout = new QHBoxLayout;
	receRepeatLayout->addWidget(m_pChkRepeatSend);
	receRepeatLayout->addWidget(m_pSpboxTimes);
	receRepeatLayout->addWidget(m_pTimeUint);
	receRepeatLayout->addStretch();

	//QGroupBox的布局
	QHBoxLayout *lineModeLayout = new QHBoxLayout;
	lineModeLayout->addWidget(m_pRadbtnNoLine);
	lineModeLayout->addWidget(m_pRadbtnNewLine);
	lineModeLayout->addStretch();
	lineModeLayout->addWidget(m_pRadbtnCarrRet);
	lineModeLayout->addWidget(m_pRadbtnBothNLCR);
	boxLineMode->setLayout(lineModeLayout);

	//总布局
	QVBoxLayout *receMainLayout = new QVBoxLayout;
	receMainLayout->addLayout(receModeLayout);
	receMainLayout->addLayout(receRepeatLayout);
	receMainLayout->addWidget(m_pChkNote);
	receMainLayout->addWidget(m_pChkSendFormat);
	receMainLayout->addWidget(boxLineMode);
	m_pWgtReceive->setLayout(receMainLayout);
	receMainLayout->setSpacing(10);

	/* 发送选项 */
	m_pWgtSend = new QWidget();
	m_pRadbtnSendASCII = new QRadioButton(QStringLiteral("ASCII"));
	m_pRadbtnSendASCII->setChecked(true);
	m_pRadbtnSendHex = new QRadioButton(QStringLiteral("Hex"));
	m_pChkWordwrap = new QCheckBox(QStringLiteral("自动换行"));
	m_pChkShowSend = new QCheckBox(QStringLiteral("显示发送"));
	m_pChkShowTime = new QCheckBox(QStringLiteral("显示时间"));
	QLabel *labMinInt = new QLabel("Minimal Interval:");
	m_pSpboxMin = new QSpinBox;
	m_pSpboxMin->setMaximum(INT_MAX);
	m_pSpboxMin->setMinimumWidth(100);
	m_pSpboxMin->setValue(500);
	QLabel *labMs = new	QLabel("ms");
	QLabel *labTimeFormat = new QLabel("Time Format");
	m_pLineEditTimeFormat = new QLineEdit("[hh:mm:ss.zzz]");
	m_pLabResTimeFormat = new QLabel;
	m_pLabResTimeFormat->setText(QDateTime::currentDateTime().toString("[hh:mm:ss.zzz]"));

	//最上面一排
	QHBoxLayout *sendCodeLayout = new QHBoxLayout;
	sendCodeLayout->addWidget(m_pRadbtnSendASCII);
	sendCodeLayout->addWidget(m_pRadbtnSendHex);
	sendCodeLayout->setSpacing(20);
	sendCodeLayout->addStretch(1);
	//第二排
	QHBoxLayout *sendSecondLayout = new QHBoxLayout;
	sendSecondLayout->addWidget(m_pChkWordwrap);
	sendSecondLayout->addStretch(1);
	sendSecondLayout->addWidget(labMinInt);
	sendSecondLayout->addWidget(m_pSpboxMin);
	sendSecondLayout->addWidget(labMs);
	sendSecondLayout->addStretch(1);
	//第三排
	QHBoxLayout *sendThirdLayout = new QHBoxLayout;
	sendThirdLayout->addWidget(m_pChkShowSend);
	sendThirdLayout->addStretch(1);
	//第四排
	QHBoxLayout *sendFourthLayout = new QHBoxLayout;
	sendFourthLayout->addWidget(m_pChkShowTime);
	sendFourthLayout->addStretch(1);
	//第五排
	QHBoxLayout *sendFifthLayout = new QHBoxLayout;
	sendFifthLayout->addWidget(labTimeFormat);
	sendFifthLayout->addStretch();
	sendFifthLayout->addWidget(m_pLineEditTimeFormat);
	sendFifthLayout->addWidget(m_pLabResTimeFormat);
	sendFifthLayout->addStretch(1);

	//总布局
	QVBoxLayout *sendMaiLayout = new QVBoxLayout;
	sendMaiLayout->addLayout(sendCodeLayout);
	sendMaiLayout->addLayout(sendSecondLayout);
	sendMaiLayout->addLayout(sendThirdLayout);
	sendMaiLayout->addLayout(sendFourthLayout);
	sendMaiLayout->addLayout(sendFifthLayout);
	sendMaiLayout->addStretch(1);
	sendMaiLayout->setSpacing(10);

	m_pWgtSend->setLayout(sendMaiLayout);

	/* 显示选项 */
	m_pWgtDisplay = new QWidget();
	QLabel *labMaxLines = new QLabel(QStringLiteral("最大消息行数"));
	m_pSpboxMaxLines = new QSpinBox(this);
	m_pSpboxMaxLines->setRange(1, 10000);
	m_pSpboxMaxLines->setValue(500);
	QLabel *labLines = new QLabel(QStringLiteral("行"));

	QLabel *labFont = new QLabel(QStringLiteral("字体"));
	m_pLineEditFont = new QLineEdit(this);
	m_pLineEditFont->setText("Fixedsys,12");
	m_pLineEditFont->setReadOnly(true);
	m_pBtnFont = new QPushButton(QStringLiteral("字体..."));

	m_pChkUseSet = new QCheckBox(QStringLiteral("显示设置"));

	QLabel *labReceColor = new QLabel(QStringLiteral("接收消息颜色"));
	m_pLineEditReceColor = new QLineEdit(this);
	m_pLineEditReceColor->setReadOnly(true);
	m_pLineEditReceColor->setText("000000");
	QLabel *labSendColor = new QLabel(QStringLiteral("发送消息颜色"));
	m_pLineEditSendColor = new QLineEdit(this);
	m_pLineEditSendColor->setReadOnly(true);
	m_pLineEditSendColor->setText("000000");

	m_pBtnReceColor = new QPushButton(QStringLiteral("颜色..."));
	m_pBtnSendColor = new QPushButton(QStringLiteral("颜色..."));

	QGridLayout *displayLayout = new QGridLayout;
	displayLayout->addWidget(labMaxLines, 0, 0, 1, 1);
	displayLayout->addWidget(m_pSpboxMaxLines, 0, 1, 1, 4);
	displayLayout->addWidget(labLines, 0, 6, 1, 1);

	displayLayout->addWidget(labFont, 1, 0, 1, 1, Qt::AlignRight);
	displayLayout->addWidget(m_pLineEditFont, 1, 1, 1, 4);
	displayLayout->addWidget(m_pBtnFont, 1, 6, 1, 1);

	displayLayout->addWidget(m_pChkUseSet, 2, 0, 1, 1);

	displayLayout->addWidget(labReceColor, 3, 0, 1, 2);
	displayLayout->addWidget(m_pLineEditReceColor, 3, 1, 1, 4);
	displayLayout->addWidget(m_pBtnReceColor, 3, 6, 1, 1);

	displayLayout->addWidget(labSendColor, 4, 0, 1, 2);
	displayLayout->addWidget(m_pLineEditSendColor, 4, 1, 1, 4);
	displayLayout->addWidget(m_pBtnSendColor, 4, 6, 1, 1);
	m_pWgtDisplay->setLayout(displayLayout);


	m_pTabWidget = new QTabWidget(this);
	m_pTabWidget->addTab(m_pWgtConnway, QStringLiteral("串口"));
	m_pTabWidget->addTab(m_pWgtSend, QStringLiteral("发送"));
	m_pTabWidget->addTab(m_pWgtReceive, QStringLiteral("接收"));
	m_pTabWidget->addTab(m_pWgtDisplay, QStringLiteral("显示"));

	m_pBtnOK = new QPushButton("OK");
	m_pBtnCancel = new QPushButton("Cancel");
	m_pBtnApply = new QPushButton("Apply");

	QHBoxLayout *btnLayout = new QHBoxLayout();
	btnLayout->addStretch(1);
	btnLayout->addWidget(m_pBtnOK);
	btnLayout->addWidget(m_pBtnCancel);
	btnLayout->addWidget(m_pBtnApply);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addWidget(m_pTabWidget);
	mainLayout->addLayout(btnLayout);
	
	this->setLayout(mainLayout);
	this->resize(400, 260);
	this->setFixedHeight(260);
}

/**
* @brief		初始化组合框的值
* @return		void
*/
void SettingDlg::addPortList()
{
	m_pCmbCommWay->addItem("TCP/UDP");

	Q_FOREACH(QSerialPortInfo portInfo, QSerialPortInfo::availablePorts())
	{
		m_pCmbCommWay->addItem(portInfo.portName());
	}

	m_pCmbMode->addItem(QStringLiteral("TCP Client"));
	m_pCmbMode->addItem(QStringLiteral("TCP Server"));

	m_pCmbIp->addItem("LocalHost");
	m_pCmbIp->addItem("127.0.0.1");
	m_pCmbIp->setCurrentIndex(0);

	m_pCmbPort->addItem("7000");
	m_pCmbPort->setCurrentIndex(0);

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

	//时间单位
	QStringList unitsList = {"ms", "s", "m", "h"};
	m_pTimeUint->addItems(unitsList);
}

/**
* @brief		初始化信号槽的连接
* @return		void
*/
void SettingDlg::initConnect()
{
	connect(m_pBtnOK, &QPushButton::clicked, this, &SettingDlg::okBtnClicked);
	connect(m_pBtnCancel, &QPushButton::clicked, this, &SettingDlg::reject);
	connect(this->m_pCmbCommWay, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingDlg::commWayChanged);//串口/网络切换
	connect(this->m_pCmbMode, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingDlg::tcpudpModeChanged);//client/server切换
	connect(this->m_pBtnFont, &QPushButton::clicked, this, &SettingDlg::choiceFont);
	connect(this->m_pBtnReceColor, &QPushButton::clicked, this, &SettingDlg::choiceReceColor);
	connect(this->m_pBtnSendColor, &QPushButton::clicked, this, &SettingDlg::choiceSendColor);
	connect(this->m_pLineEditTimeFormat, &QLineEdit::textChanged, this, &SettingDlg::timeFormatChanged);
}

/**
* @brief		连接方式发生变化
* @param index  组合框的当前index
*
* @return		void
*/
void SettingDlg::commWayChanged(int index)
{
	QSize size = this->size();
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
* @brief		Server/Client方式组合框发生改变时
* @param index  组合框的当前index
*
* @return		void
*/
void SettingDlg::tcpudpModeChanged(int index)
{
	bool bVisible = (index == TCPCLIENT);

	m_pLabIP->setVisible(bVisible);
	m_pCmbIp->setVisible(bVisible);
}

/**
* @brief		获取设置信息
* @return		设置的信息结构体
*/
SettingCollect SettingDlg::getSettingInfo()
{
	return m_setInfo;
}

/**
* @brief		OK按钮单击
* @return		void
*/
void SettingDlg::okBtnClicked()
{
	//填充设置信息的结构体
	if (m_pCmbCommWay->currentText() == "TCP/UDP")
	{
		if (m_pCmbMode->currentText() == "TCP Client")
		{
			m_setInfo.conOpt = EnmConnOption::TcpClientOpt;
		}
		else
		{
			m_setInfo.conOpt = EnmConnOption::TcpServerOpt;
		}

		m_setInfo.ip = m_pCmbIp->currentText();
		m_setInfo.port = m_pCmbPort->currentText().toInt();
	}
	else
	{
		//串口连接方式
		m_setInfo.conOpt = EnmConnOption::SerialPortOpt;
		m_setInfo.serialPortName = m_pCmbCommWay->currentText();
		m_setInfo.baudRate = m_pCmbBaudRate->currentText().toInt();
		m_setInfo.dataBits = m_pCmbDataBits->currentText().toInt();
		m_setInfo.parity = m_pCmbParity->currentText();
		m_setInfo.stopBits = m_pCmbStopBits->currentText().toFloat();
		m_setInfo.flowControl = m_pCmbFlowControl->currentText();
	}

	m_setInfo.receOpt = m_pRadbtnRecASCII->isChecked()? EnmEncodeOpt::ASCII : EnmEncodeOpt::Hex;
	m_setInfo.wordwrap = m_pChkWordwrap->isChecked();
	m_setInfo.showSend = m_pChkShowSend->isChecked();
	m_setInfo.showTime = m_pChkShowTime->isChecked();
	m_setInfo.minInterval = m_pSpboxMin->value();
	m_setInfo.timeFormat = m_pLineEditTimeFormat->text();

	m_setInfo.sendOpt = m_pRadbtnSendASCII->isChecked() ? EnmEncodeOpt::ASCII : EnmEncodeOpt::Hex;
	m_setInfo.sendRepeat = m_pChkRepeatSend->isChecked();
	m_setInfo.repeatTimes = m_pSpboxTimes->value();
	m_setInfo.timeSuffix = (EnmTimeSuffix)m_pTimeUint->currentIndex();
	m_setInfo.sptNote = m_pChkNote->isChecked();
	m_setInfo.sendFormat = m_pChkSendFormat->isChecked();

	m_setInfo.maxLines = m_pSpboxMaxLines->value();
	m_setInfo.font = m_pLineEditFont->text();
	m_setInfo.displayFormatColor = m_pChkUseSet->isChecked();
	m_setInfo.receMsgColor = m_pLineEditReceColor->text();
	m_setInfo.sendMsgColor = m_pLineEditSendColor->text();

	this->accept();
}

/**
* @brief		字体选择
* @return		void
*/
void SettingDlg::choiceFont()
{
	QStringList fontstyle =  m_pLineEditFont->text().split(',');

	QFont defaultFont;

	if (fontstyle.size() == 2)
	{
		defaultFont.setFamily(fontstyle[0]);
		defaultFont.setPointSize(fontstyle[1].toInt());
	}

	bool bok = false;
	QFont font = QFontDialog::getFont(&bok, defaultFont, this);
	if (bok)
	{
		QString strFont = font.family() + "," + QString::number(font.pointSize());
		this->m_pLineEditFont->setText(strFont);
	}
}

/**
* @brief		选择接收的颜色
* @return		void
*/
void SettingDlg::choiceReceColor()
{
	bool bok = false;
	QRgb rgb = QColorDialog::getRgba(this->m_pLineEditReceColor->text().toInt(&bok, 16), &bok);
	if (bok)
	{
		this->m_pLineEditReceColor->setText(QString::number(rgb, 16));
	}
}

/**
* @brief		选择发送的颜色
* @return		void
*/
void SettingDlg::choiceSendColor()
{
	bool bok = false;
	QRgb rgb = QColorDialog::getRgba(this->m_pLineEditSendColor->text().toInt(&bok, 16), &bok);
	if (bok)
	{
		this->m_pLineEditSendColor->setText(QString::number(rgb, 16));
	}
}

/**
* @brief		timeformat的值发生改变
* @param str	改变的值
*
* @return		void
*/
void SettingDlg::timeFormatChanged(const QString &str)
{
	QString strDate = QDateTime::currentDateTime().toString(str);
	this->m_pLabResTimeFormat->setText(strDate);
}