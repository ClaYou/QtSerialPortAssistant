#pragma once

#include <QDialog>
#include "Global.h"

class QPushButton;
class QComboBox;
class QRadioButton;
class QLineEdit;
class QCheckBox;
class QTabWidget;
class QLabel;
class QSpinBox;

class SettingDlg : public QDialog
{
	Q_OBJECT

public:
	SettingDlg(QWidget *parent);
	~SettingDlg();
	SettingCollect getSettingInfo();

private:
	void initViewer();
	void initConnect();
	void addPortList();

	Q_SLOT void commWayChanged(int index);
	Q_SLOT void tcpudpModeChanged(int index);
	Q_SLOT void okBtnClicked();
	Q_SLOT void choiceFont();
	Q_SLOT void choiceReceColor();
	Q_SLOT void choiceSendColor();
	Q_SLOT void timeFormatChanged(const QString &);

	SettingCollect m_setInfo;

#pragma region 界面相关
	QTabWidget *m_pTabWidget;	//主窗口

	/*通信方式*/
	QWidget *m_pWgtConnway;
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

	/*接收设置*/
	QWidget *m_pWgtReceive;
	QRadioButton *m_pRadbtnRecASCII;
	QRadioButton *m_pRadbtnRecHex;
	QComboBox *m_pTimeUint;
	QCheckBox *m_pChkWordwrap;
	QCheckBox *m_pChkShowSend;
	QCheckBox *m_pChkShowTime;
	QSpinBox *m_pSpboxMin;
	QLineEdit *m_pLineEditTimeFormat;	//时间格式
	QLabel *m_pLabResTimeFormat;		//时间格式的结果

	/*发送设置*/
	QWidget *m_pWgtSend;
	QRadioButton *m_pRadbtnSendASCII;
	QRadioButton *m_pRadbtnSendHex;
	QCheckBox *m_pChkRepeatSend;
	QSpinBox *m_pSpboxTimes;
	QCheckBox *m_pChkNote;
	QCheckBox *m_pChkSendFormat;
	QRadioButton *m_pRadbtnNoLine;
	QRadioButton *m_pRadbtnNewLine;
	QRadioButton *m_pRadbtnCarrRet;
	QRadioButton *m_pRadbtnBothNLCR;

	/*显示*/
	QWidget *m_pWgtDisplay;
	QSpinBox *m_pSpboxMaxLines;
	QLineEdit *m_pLineEditFont;
	QPushButton *m_pBtnFont;
	QCheckBox *m_pChkUseSet;
	QLineEdit *m_pLineEditReceColor;
	QPushButton *m_pBtnReceColor;
	QLineEdit *m_pLineEditSendColor;
	QPushButton *m_pBtnSendColor;

	QPushButton *m_pBtnOK;
	QPushButton *m_pBtnCancel;
	QPushButton *m_pBtnApply;
#pragma endregion
};
