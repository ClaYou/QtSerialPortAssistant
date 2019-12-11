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
* @brief		��ʼ���Ŵ����һЩ��Ϣ
* @return		void
*/
void SettingDlg::initViewer()
{
	this->setWindowTitle(QStringLiteral("����"));

	/* ����ѡ�� */
	m_pWgtConnway = new QWidget();
	QLabel *labCommWay = new QLabel(QStringLiteral("����:"));
	labCommWay->setMaximumWidth(40);
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

	QVBoxLayout *portExtendLayout = new QVBoxLayout();
	portExtendLayout->addLayout(portSetLayout);
	portExtendLayout->addStretch();

	m_pWgtConnway->setLayout(portExtendLayout);

	/* ����ѡ�� */
	m_pWgtReceive = new QWidget();
	m_pRadbtnRecASCII = new QRadioButton(QStringLiteral("ASCII"));
	m_pRadbtnRecASCII->setChecked(true);
	m_pRadbtnRecHex = new QRadioButton(QStringLiteral("Hex"));
	m_pChkRepeatSend = new QCheckBox(QStringLiteral("�ظ�����"));
	m_pTimeUint = new QComboBox;
	m_pSpboxTimes = new QSpinBox;
	m_pSpboxTimes->setMaximum(INT_MAX);
	m_pSpboxTimes->setValue(1000);
	m_pChkNote = new QCheckBox(QStringLiteral("֧��ע��(Hexģʽ�²����� // ֮�������)"));
	m_pChkSendFormat = new QCheckBox(QStringLiteral("���͸�ʽ�� (010203 -> 01 02 03)"));
	QGroupBox *boxLineMode = new QGroupBox("Line Ending Mode");
	m_pRadbtnNoLine = new QRadioButton("No Line ending");
	m_pRadbtnNewLine = new QRadioButton("NewLine");
	m_pRadbtnCarrRet = new QRadioButton("Carriage return");
	m_pRadbtnBothNLCR = new QRadioButton("Both NL & CR");

	//ASCII Hex����
	QHBoxLayout *receModeLayout = new QHBoxLayout;
	receModeLayout->addWidget(m_pRadbtnRecASCII);
	receModeLayout->addWidget(m_pRadbtnRecHex);
	receModeLayout->addStretch();

	//�ظ����͵Ĳ���
	QHBoxLayout *receRepeatLayout = new QHBoxLayout;
	receRepeatLayout->addWidget(m_pChkRepeatSend);
	receRepeatLayout->addWidget(m_pSpboxTimes);
	receRepeatLayout->addWidget(m_pTimeUint);
	receRepeatLayout->addStretch();

	//QGroupBox�Ĳ���
	QHBoxLayout *lineModeLayout = new QHBoxLayout;
	lineModeLayout->addWidget(m_pRadbtnNoLine);
	lineModeLayout->addWidget(m_pRadbtnNewLine);
	lineModeLayout->addStretch();
	lineModeLayout->addWidget(m_pRadbtnCarrRet);
	lineModeLayout->addWidget(m_pRadbtnBothNLCR);
	boxLineMode->setLayout(lineModeLayout);

	//�ܲ���
	QVBoxLayout *receMainLayout = new QVBoxLayout;
	receMainLayout->addLayout(receModeLayout);
	receMainLayout->addLayout(receRepeatLayout);
	receMainLayout->addWidget(m_pChkNote);
	receMainLayout->addWidget(m_pChkSendFormat);
	receMainLayout->addWidget(boxLineMode);
	m_pWgtReceive->setLayout(receMainLayout);
	receMainLayout->setSpacing(10);

	/* ����ѡ�� */
	m_pWgtSend = new QWidget();
	m_pRadbtnSendASCII = new QRadioButton(QStringLiteral("ASCII"));
	m_pRadbtnSendASCII->setChecked(true);
	m_pRadbtnSendHex = new QRadioButton(QStringLiteral("Hex"));
	m_pChkWordwrap = new QCheckBox(QStringLiteral("�Զ�����"));
	m_pChkShowSend = new QCheckBox(QStringLiteral("��ʾ����"));
	m_pChkShowTime = new QCheckBox(QStringLiteral("��ʾʱ��"));
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

	//������һ��
	QHBoxLayout *sendCodeLayout = new QHBoxLayout;
	sendCodeLayout->addWidget(m_pRadbtnSendASCII);
	sendCodeLayout->addWidget(m_pRadbtnSendHex);
	sendCodeLayout->setSpacing(20);
	sendCodeLayout->addStretch(1);
	//�ڶ���
	QHBoxLayout *sendSecondLayout = new QHBoxLayout;
	sendSecondLayout->addWidget(m_pChkWordwrap);
	sendSecondLayout->addStretch(1);
	sendSecondLayout->addWidget(labMinInt);
	sendSecondLayout->addWidget(m_pSpboxMin);
	sendSecondLayout->addWidget(labMs);
	sendSecondLayout->addStretch(1);
	//������
	QHBoxLayout *sendThirdLayout = new QHBoxLayout;
	sendThirdLayout->addWidget(m_pChkShowSend);
	sendThirdLayout->addStretch(1);
	//������
	QHBoxLayout *sendFourthLayout = new QHBoxLayout;
	sendFourthLayout->addWidget(m_pChkShowTime);
	sendFourthLayout->addStretch(1);
	//������
	QHBoxLayout *sendFifthLayout = new QHBoxLayout;
	sendFifthLayout->addWidget(labTimeFormat);
	sendFifthLayout->addStretch();
	sendFifthLayout->addWidget(m_pLineEditTimeFormat);
	sendFifthLayout->addWidget(m_pLabResTimeFormat);
	sendFifthLayout->addStretch(1);

	//�ܲ���
	QVBoxLayout *sendMaiLayout = new QVBoxLayout;
	sendMaiLayout->addLayout(sendCodeLayout);
	sendMaiLayout->addLayout(sendSecondLayout);
	sendMaiLayout->addLayout(sendThirdLayout);
	sendMaiLayout->addLayout(sendFourthLayout);
	sendMaiLayout->addLayout(sendFifthLayout);
	sendMaiLayout->addStretch(1);
	sendMaiLayout->setSpacing(10);

	m_pWgtSend->setLayout(sendMaiLayout);

	/* ��ʾѡ�� */
	m_pWgtDisplay = new QWidget();
	QLabel *labMaxLines = new QLabel(QStringLiteral("�����Ϣ����"));
	m_pSpboxMaxLines = new QSpinBox(this);
	m_pSpboxMaxLines->setRange(1, 10000);
	m_pSpboxMaxLines->setValue(500);
	QLabel *labLines = new QLabel(QStringLiteral("��"));

	QLabel *labFont = new QLabel(QStringLiteral("����"));
	m_pLineEditFont = new QLineEdit(this);
	m_pLineEditFont->setText("Fixedsys,12");
	m_pLineEditFont->setReadOnly(true);
	m_pBtnFont = new QPushButton(QStringLiteral("����..."));

	m_pChkUseSet = new QCheckBox(QStringLiteral("��ʾ����"));

	QLabel *labReceColor = new QLabel(QStringLiteral("������Ϣ��ɫ"));
	m_pLineEditReceColor = new QLineEdit(this);
	m_pLineEditReceColor->setReadOnly(true);
	m_pLineEditReceColor->setText("000000");
	QLabel *labSendColor = new QLabel(QStringLiteral("������Ϣ��ɫ"));
	m_pLineEditSendColor = new QLineEdit(this);
	m_pLineEditSendColor->setReadOnly(true);
	m_pLineEditSendColor->setText("000000");

	m_pBtnReceColor = new QPushButton(QStringLiteral("��ɫ..."));
	m_pBtnSendColor = new QPushButton(QStringLiteral("��ɫ..."));

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
	m_pTabWidget->addTab(m_pWgtConnway, QStringLiteral("����"));
	m_pTabWidget->addTab(m_pWgtSend, QStringLiteral("����"));
	m_pTabWidget->addTab(m_pWgtReceive, QStringLiteral("����"));
	m_pTabWidget->addTab(m_pWgtDisplay, QStringLiteral("��ʾ"));

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
* @brief		��ʼ����Ͽ��ֵ
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

	//ʱ�䵥λ
	QStringList unitsList = {"ms", "s", "m", "h"};
	m_pTimeUint->addItems(unitsList);
}

/**
* @brief		��ʼ���źŲ۵�����
* @return		void
*/
void SettingDlg::initConnect()
{
	connect(m_pBtnOK, &QPushButton::clicked, this, &SettingDlg::okBtnClicked);
	connect(m_pBtnCancel, &QPushButton::clicked, this, &SettingDlg::reject);
	connect(this->m_pCmbCommWay, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingDlg::commWayChanged);//����/�����л�
	connect(this->m_pCmbMode, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingDlg::tcpudpModeChanged);//client/server�л�
	connect(this->m_pBtnFont, &QPushButton::clicked, this, &SettingDlg::choiceFont);
	connect(this->m_pBtnReceColor, &QPushButton::clicked, this, &SettingDlg::choiceReceColor);
	connect(this->m_pBtnSendColor, &QPushButton::clicked, this, &SettingDlg::choiceSendColor);
	connect(this->m_pLineEditTimeFormat, &QLineEdit::textChanged, this, &SettingDlg::timeFormatChanged);
}

/**
* @brief		���ӷ�ʽ�����仯
* @param index  ��Ͽ�ĵ�ǰindex
*
* @return		void
*/
void SettingDlg::commWayChanged(int index)
{
	QSize size = this->size();
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
* @brief		Server/Client��ʽ��Ͽ����ı�ʱ
* @param index  ��Ͽ�ĵ�ǰindex
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
* @brief		��ȡ������Ϣ
* @return		���õ���Ϣ�ṹ��
*/
SettingCollect SettingDlg::getSettingInfo()
{
	return m_setInfo;
}

/**
* @brief		OK��ť����
* @return		void
*/
void SettingDlg::okBtnClicked()
{
	//���������Ϣ�Ľṹ��
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
		//�������ӷ�ʽ
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
* @brief		����ѡ��
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
* @brief		ѡ����յ���ɫ
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
* @brief		ѡ���͵���ɫ
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
* @brief		timeformat��ֵ�����ı�
* @param str	�ı��ֵ
*
* @return		void
*/
void SettingDlg::timeFormatChanged(const QString &str)
{
	QString strDate = QDateTime::currentDateTime().toString(str);
	this->m_pLabResTimeFormat->setText(strDate);
}