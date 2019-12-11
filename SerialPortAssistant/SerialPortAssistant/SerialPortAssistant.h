#pragma once

#include <QtWidgets/QMainWindow>
#include <QAbstractSocket>
#include <QSerialPort>
#include "ChildWidget.h"
#include "Global.h"

class QMenu;
class QAction;
class QActionGroup;
class QTextEdit;
class QPushButton;
class QRadioButton;
class QCheckBox;
class QSpinBox;
class QLabel;
class QListWidget;
class QTreeWidget;
class TcpServer;
class QTcpSocket;
class QTimer;
class QComboBox;
class QToolBar;


class SerialPortAssistant : public QMainWindow
{
	Q_OBJECT

public:
	SerialPortAssistant(QWidget *parent = Q_NULLPTR);

private:
	void initViewer();
	void createMenu();
	void createToolBtns();
	void initConnect();
	void createCenWgt();
	void setActionChecked(EnmCurRunState state);	//����״̬�ı䶯��checked

	Q_SLOT void newUtility();			//�½�
	Q_SLOT void openUtility();			//��
	Q_SLOT void saveUtility();			//����
	Q_SLOT void startBtnClicked();		//��ʼ���Ӱ�ť
	Q_SLOT void pauseBtnClicked();		//��ͣ?
	Q_SLOT void closeBtnClicked();		//�ر����Ӱ�ť
	Q_SLOT void addChannelBtnClicked();	//����µ�ͨ��
	Q_SLOT void delChannelBtnClicked();	//ɾ��ͨ��
	Q_SLOT void changeAligment(QAction *pAction);
	Q_SLOT void openSettingDlg();		//�����öԻ���
	Q_SLOT void openASCIIDlg();			//��ASCII �Ի���
	Q_SLOT void clearAllMsg();
	Q_SLOT void setWinForwoardOrNot();		//�Ƿ�����Ϊ�ö�
	Q_SLOT void setLogRecordState();		//�����Ƿ��¼��־
	Q_SLOT void openLogFolder();			//����־�ļ���
	Q_SLOT void viewCurDataLog();			//�鿴��ǰ��־TXT
	Q_SLOT void quickSetting();	//��������
	Q_SLOT void about();		//���ڶԻ���

protected:
	void closeEvent(QCloseEvent *event) override;
private:

#pragma region �˵����
	QMenu *m_pFileMenu;
	QMenu *m_pEditMenu;
	QMenu *m_pViewMenu;
	QMenu *m_pToolMenu;
	QMenu *m_pHelpMenu;

	QAction *m_pNewAction;
	QAction *m_pOpenAction;
	QAction *m_pSaveAction;
	QAction *m_pRecordLogAction;
	QAction *m_pViewCurLogActin;
	QAction *m_pOpenLogAction;
	QAction *m_pExitAction;

	QAction *m_pAddPortAction;
	QAction *m_pDelPortActin;
	QAction *m_pStartAction;
	QAction *m_pPauseAction;
	QAction *m_pStopAction;
	QAction *m_pClearAction;

	QActionGroup *m_pAlignmentGroup;
	QAction *m_pHorAction;
	QAction *m_pVertAction;
	QAction *m_pGridAction;
	QAction *m_pCustomAction;
	QAction *m_pQuickSetAction;
	QAction *m_pForwardAction;

	QAction *m_pASCIICodeAction;
	QAction *m_pSetAction;

	QAction *m_pAboutAction;
	QAction *m_pAboutQtAction;
#pragma endregion

	QToolBar *m_pToolsBar;
	QMenuBar *m_pMenuBar;
	QWidget *m_pCenWidget;
	QList<ChildWidget *> m_listChildWidget;

	EnmLastLayout m_lastLayout;		//��¼��һ�εĲ���
	int m_iCustomLayoutCols;		//�Զ��岼�ֵ����� �����
	QString m_strLogPath;			//��¼����־·��
};
