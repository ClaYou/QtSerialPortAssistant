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
	void setActionChecked(EnmCurRunState state);	//根据状态改变动作checked

	Q_SLOT void newUtility();			//新建
	Q_SLOT void openUtility();			//打开
	Q_SLOT void saveUtility();			//保存
	Q_SLOT void startBtnClicked();		//开始连接按钮
	Q_SLOT void pauseBtnClicked();		//暂停?
	Q_SLOT void closeBtnClicked();		//关闭连接按钮
	Q_SLOT void addChannelBtnClicked();	//添加新的通道
	Q_SLOT void delChannelBtnClicked();	//删除通道
	Q_SLOT void changeAligment(QAction *pAction);
	Q_SLOT void openSettingDlg();		//打开设置对话框
	Q_SLOT void openASCIIDlg();			//打开ASCII 对话框
	Q_SLOT void clearAllMsg();
	Q_SLOT void setWinForwoardOrNot();		//是否设置为置顶
	Q_SLOT void setLogRecordState();		//设置是否记录日志
	Q_SLOT void openLogFolder();			//打开日志文件夹
	Q_SLOT void viewCurDataLog();			//查看当前日志TXT
	Q_SLOT void quickSetting();	//快速设置
	Q_SLOT void about();		//关于对话框

protected:
	void closeEvent(QCloseEvent *event) override;
private:

#pragma region 菜单相关
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

	EnmLastLayout m_lastLayout;		//记录上一次的布局
	int m_iCustomLayoutCols;		//自定义布局的列数 如果有
	QString m_strLogPath;			//记录下日志路径
};
