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
	void doQuckSetting(bool bOn);					//�Ƿ�򿪿������ô���
	void setLogState(bool bOn);						//�Ƿ����־
	void setLogPath(const QString &logPath);		//������־·��
	void restoreDefault();							//�ָ�Ĭ��

private:
#pragma region �������
	QWidget *m_pCenWidget;
	QWidget *m_pLeftSetWgt;		//��ߵĲ��ִ���
	QTextEdit *m_pEditMsg;		//��ʾ��Ϣ�б�
	QTextEdit *m_pEditSend;		//Ҫ���͵���Ϣ
	QPushButton *m_pBtnSend;	//����
	QComboBox *m_pCmbHistory;	//��¼�������͵ļ�¼

	QComboBox *m_pCmbCommWay;	//ͨ�ŵķ�ʽ
	QComboBox *m_pCmbMode;
	QComboBox *m_pCmbIp;
	QComboBox *m_pCmbPort;
	QLabel *m_pLabMode;
	QLabel *m_pLabIP;
	QLabel *m_pLabPort;

	//����ͨ��
	QLabel *m_pLabBaudRate;
	QLabel *m_pLabDataBits;
	QLabel *m_pLabParity;
	QLabel *m_pLabStopBits;
	QLabel *m_pLabFlowControl;
	QComboBox *m_pCmbBaudRate;
	QComboBox *m_pCmbDataBits;		//����λ
	QComboBox *m_pCmbParity;		//У��λ
	QComboBox *m_pCmbStopBits;		//ֹͣλ
	QComboBox *m_pCmbFlowControl;	//������

									//�����б� ���������Ч
	QTreeWidget *m_pTreeWgtClient;

	//����
	QRadioButton *m_pRadbtnRecASCII;
	QRadioButton *m_pRadbtnRecHex;
	QCheckBox *m_pChkWordwrap;
	QCheckBox *m_pChkShowSend;
	QCheckBox *m_pChkShowTime;

	//����
	QRadioButton *m_pRadbtnSendASCII;
	QRadioButton *m_pRadbtnSendHex;
	QCheckBox *m_pChkRepeatSend;
	QSpinBox *m_pSpboxTimes;

	//״̬��
	QLabel *m_pLabConnInfo;
	QLabel *m_pLabTx;
	QLabel *m_pLabRx;
	QLabel *m_pLabPlace;
#pragma  endregion

private:
	TcpServer *m_pTcpServer;		//����������
	QTcpSocket *m_pTcpClient;		//�ͻ�������socket
	QSerialPort *m_pSerialPort;		//����ͨ��

	QMap<quint16, QString> m_mapClient;	//���浱ǰ�ͻ����б�

	QTimer *m_pRepeatSendTimer;			//�ظ����Ͷ�ʱ��
	EnmCurSendOption m_curSendOpt;		//��ǰ���ӵ�״̬

	int m_iRxCount;		//Rx ����
	int m_iTxCount;		//Tx ����

	QString m_strTimeFormat;	//ʱ���ʽ
	bool m_bSptNote;			//֧��ע��
	bool m_bSendFormat;			//�����Ƿ��ʽ��
	bool m_bSaveLog;			//�Ƿ�д��־
	QString m_strLogPath;		//��־�ļ�·��
	int m_iMaxLines;			//��Ϣ�������
	QString m_msgFont;			//��ʾ��Ϣ������
	bool m_bDisplayColor;		//�Ƿ���ʾ��ɫ
	QString m_receMsgColor;		//���յ�������ɫ
	QString m_sendMsgColor;		//���͵�������ɫ

protected:
	void closeEvent(QCloseEvent *event);

private:
	void initViewer();
	void initConnect();
	void initStatusBar();
	void addPortList();
	void createCenWgt();
	EnmConnOption getConnWay();		//��ȡһ�µ�ǰ�����ӷ�ʽ
	void setLabelTextColor(QLabel *pLabel, const QColor &color);	//����label��ɫ
	void updateRxTxInfo(EnmUpdateRxTx state, int count = 0);		//����״̬����RxTx��Ϣ
	void showMessage(QString strMsg, EnmLogOptions ways = EnmLogOption::Console | EnmLogOption::Interface | EnmLogOption::LogFile);
	void setRepeatTimes(int times, EnmTimeSuffix timeSuf);
	void setSerialPortInfo(const SettingCollect &collect);
	void appendToComboBox(QComboBox *pCmbBox, const QString &strMsg);

	Q_SLOT void commWayChanged(int index);
	Q_SLOT void tcpudpModeChanged(int index);
	Q_SLOT void sendBtnClicked();			//���Ͱ�ť
	Q_SLOT void sendHistorySelcted();		//ѡ�����½���ʷ����
	Q_SLOT void repeatSend(int iSend);		//ѡ���ظ�����
	Q_SLOT void readDataFromClient(QByteArray strMsg);
	Q_SLOT void newConnectFromServer(QString strAddr, quint16 iport);
	Q_SLOT void readDataFromServer();
	Q_SLOT void disconnectedFromServer(QString strAddr, int iport);
	Q_SLOT void clientSocketError(QAbstractSocket::SocketError socketError);
	Q_SLOT void readDataFromSerialPort();
	Q_SLOT void serialPortError(QSerialPort::SerialPortError error);
};
