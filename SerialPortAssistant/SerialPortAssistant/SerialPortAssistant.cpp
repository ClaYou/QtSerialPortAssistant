#include "SerialPortAssistant.h"
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QUrl>
#include <QToolBar>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QTextCodec>
#include <QDesktopServices>
#include <QProcess>
#include <QFileDialog>
#include "windows.h"

#include "TcpServer.h"
#include "TcpClient.h"
#include "AboutDlg.h"
#include "SettingDlg.h"
#include "ToolsHelp.h"
#include "CustomColDlg.h"
#include "ASCIIDlg.h"


/**
* @brief 函数简要说明-测试函数
* @param index    参数1
* @param t        参数2 @see CTest
*
* @return 返回说明
*     -<em>false</em> fail
*     -<em>true</em> succeed
*/

SerialPortAssistant::SerialPortAssistant(QWidget *parent)
	: QMainWindow(parent)
{
	initViewer();
}

/**
* @brief		初始化类
* @return		void
*/
void SerialPortAssistant::initViewer()
{
	createMenu();
	createToolBtns();
	createCenWgt();
	initConnect();
	this->resize(700, 550);
	this->setWindowTitle(QStringLiteral("串口网络调试工具"));
	//this->setWindowIcon(QIcon(":/images/Icon.png"));

	m_pStopAction->setChecked(true);	//默认关闭状态
	m_lastLayout = EnmLastLayout::HBoxLayout;
	m_iCustomLayoutCols = 2;

	//记录日志文件
	QString strDate = QDateTime::currentDateTime().toString("yyyyMMdd");
	m_strLogPath =  QCoreApplication::applicationDirPath() +  "/log_" + strDate + ".txt";
}

/**
* @brief		初始化菜单
* @return		void
*/
void SerialPortAssistant::createMenu()
{
	m_pFileMenu = new QMenu(QStringLiteral("文件(&F)"));
	m_pEditMenu = new QMenu(QStringLiteral("编辑(&E)"));
	m_pViewMenu = new QMenu(QStringLiteral("视图(&V)"));
	m_pToolMenu = new QMenu(QStringLiteral("工具(&T)"));
	m_pHelpMenu = new QMenu(QStringLiteral("帮助(&H)"));

	m_pNewAction = new QAction(QStringLiteral("新建..."));
	m_pNewAction->setIcon(QIcon(":/images/New.png"));
	m_pOpenAction = new QAction(QStringLiteral("打开..."));
	m_pOpenAction->setIcon(QIcon(":/images/Open.png"));
	m_pSaveAction = new QAction(QStringLiteral("保存..."));
	m_pSaveAction->setIcon(QIcon(":/images/Save.png"));
	m_pRecordLogAction = new QAction(QStringLiteral("Record Data Log"));
	m_pRecordLogAction->setIcon(QIcon(":/images/Log.png"));
	m_pRecordLogAction->setCheckable(true);
	m_pViewCurLogActin = new QAction(QStringLiteral("View Current Data Log"));
	m_pOpenLogAction = new QAction(QStringLiteral("Open Log Folder"));
	m_pExitAction = new QAction(QStringLiteral("退出(&X)"));

	m_pAddPortAction = new QAction(QStringLiteral("增加端口..."));
	m_pAddPortAction->setIcon(QIcon(":/images/Add.png"));
	m_pDelPortActin = new QAction(QStringLiteral("删除端口..."));
	m_pDelPortActin->setEnabled(false);
	m_pDelPortActin->setIcon(QIcon(":/images/Delete.png"));
	m_pStartAction = new QAction(QStringLiteral("开始"));
	m_pStartAction->setIcon(QIcon(":/images/Start.png"));
	m_pStartAction->setCheckable(true);
	m_pPauseAction = new QAction(QStringLiteral("暂停"));
	m_pPauseAction->setIcon(QIcon(":/images/Pause.png"));
	m_pPauseAction->setCheckable(true);
	m_pStopAction = new QAction(QStringLiteral("停止"));
	m_pStopAction->setIcon(QIcon(":/images/Stop.png"));
	m_pStopAction->setCheckable(true);
	m_pClearAction = new QAction(QStringLiteral("清除"));
	m_pClearAction->setIcon(QIcon(":/images/Clear.png"));

	m_pHorAction = new QAction(QStringLiteral("水平"));
	m_pHorAction->setCheckable(true);
	m_pHorAction->setChecked(true);
	m_pVertAction = new QAction(QStringLiteral("垂直"));
	m_pVertAction->setCheckable(true);
	m_pGridAction = new QAction(QStringLiteral("网格"));
	m_pGridAction->setCheckable(true);
	m_pCustomAction = new QAction(QStringLiteral("自定义"));
	m_pCustomAction->setCheckable(true);

	m_pAlignmentGroup = new QActionGroup(this);
	m_pAlignmentGroup->addAction(m_pHorAction);
	m_pAlignmentGroup->addAction(m_pVertAction);
	m_pAlignmentGroup->addAction(m_pGridAction);
	m_pAlignmentGroup->addAction(m_pCustomAction);

	m_pQuickSetAction = new QAction(QStringLiteral("快速设置"));
	m_pQuickSetAction->setIcon(QIcon(":/images/QuickSet.png"));
	m_pQuickSetAction->setCheckable(true);
	m_pQuickSetAction->setChecked(true);
	m_pForwardAction = new QAction(QStringLiteral("前端显示"));
	m_pForwardAction->setCheckable(true);

	m_pASCIICodeAction = new QAction(QStringLiteral("ASCII Code..."));
	m_pSetAction = new QAction(QStringLiteral("设置"));
	m_pSetAction->setIcon(QIcon(":/images/Set.png"));

	m_pAboutAction = new QAction(QStringLiteral("关于(&A)"));
	m_pAboutQtAction = new QAction(QStringLiteral("关于Qt(&Q)"));

	m_pFileMenu->addAction(m_pNewAction);
	m_pFileMenu->addAction(m_pOpenAction);
	m_pFileMenu->addAction(m_pSaveAction);
	m_pFileMenu->addSeparator();
	m_pFileMenu->addAction(m_pRecordLogAction);
	m_pFileMenu->addAction(m_pViewCurLogActin);
	m_pFileMenu->addAction(m_pOpenLogAction);
	m_pFileMenu->addSeparator();
	m_pFileMenu->addAction(m_pExitAction);

	m_pEditMenu->addAction(m_pAddPortAction);
	m_pEditMenu->addAction(m_pDelPortActin);
	m_pEditMenu->addSeparator();
	m_pEditMenu->addAction(m_pStartAction);
	m_pEditMenu->addAction(m_pPauseAction);
	m_pEditMenu->addAction(m_pStopAction);
	m_pEditMenu->addSeparator();
	m_pEditMenu->addAction(m_pClearAction);
	m_pEditMenu->addSeparator();

	m_pViewMenu->addAction(m_pHorAction);
	m_pViewMenu->addAction(m_pVertAction);
	m_pViewMenu->addAction(m_pGridAction);
	m_pViewMenu->addAction(m_pCustomAction);
	m_pViewMenu->addSeparator();
	m_pViewMenu->addAction(m_pQuickSetAction);
	m_pViewMenu->addSeparator();
	m_pViewMenu->addAction(m_pForwardAction);

	m_pToolMenu->addAction(m_pASCIICodeAction);
	m_pToolMenu->addSeparator();
	m_pToolMenu->addAction(m_pSetAction);

	m_pHelpMenu->addAction(m_pAboutAction);
	m_pHelpMenu->addAction(m_pAboutQtAction);

	m_pMenuBar = this->menuBar();
	m_pMenuBar->addMenu(m_pFileMenu);
	m_pMenuBar->addMenu(m_pEditMenu);
	m_pMenuBar->addMenu(m_pViewMenu);
	m_pMenuBar->addMenu(m_pToolMenu);
	m_pMenuBar->addMenu(m_pHelpMenu);
}

/**
* @brief		初始化工具栏
* @return		void
*/
void SerialPortAssistant::createToolBtns()
{
	m_pToolsBar = this->addToolBar("Default");
	m_pToolsBar->setIconSize(QSize(28, 28));

	m_pToolsBar->addAction(m_pNewAction);
	m_pToolsBar->addAction(m_pOpenAction);
	m_pToolsBar->addAction(m_pSaveAction);
	m_pToolsBar->addAction(m_pRecordLogAction);
	m_pToolsBar->addSeparator();
	m_pToolsBar->addAction(m_pAddPortAction);
	m_pToolsBar->addAction(m_pDelPortActin);
	m_pToolsBar->addSeparator();
	m_pToolsBar->addAction(m_pStartAction);
	m_pToolsBar->addAction(m_pPauseAction);
	m_pToolsBar->addAction(m_pStopAction);
	m_pToolsBar->addAction(m_pClearAction);
	m_pToolsBar->addSeparator();
	m_pToolsBar->addAction(m_pQuickSetAction);
	m_pToolsBar->addAction(m_pSetAction);
	m_pToolsBar->addSeparator();
}

/**
* @brief		初始化控件
* @return		void
*/
void SerialPortAssistant::createCenWgt()
{
	m_pCenWidget = new QWidget;

	ChildWidget *childWgt = new ChildWidget(0);

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->setSpacing(0);
	gridLayout->addWidget(childWgt);
	gridLayout->setMargin(0);			//设置各个窗体间的间隔为0 
	m_pCenWidget->setLayout(gridLayout);

	this->setCentralWidget(m_pCenWidget);

	m_listChildWidget.push_back(childWgt);
}

/**
* @brief		初始化信号槽的连接信息
* @return		void
*/
void SerialPortAssistant::initConnect()
{
	connect(this->m_pStartAction, &QAction::triggered, this, &SerialPortAssistant::startBtnClicked);			//连接按钮
	connect(this->m_pPauseAction, &QAction::triggered, this, &SerialPortAssistant::pauseBtnClicked);			//暂停？
	connect(this->m_pStopAction, &QAction::triggered, this, &SerialPortAssistant::closeBtnClicked);				//断开连接按钮
	connect(this->m_pSetAction, &QAction::triggered, this, &SerialPortAssistant::openSettingDlg);				//设置对话框
	connect(this->m_pNewAction, &QAction::triggered, this, &SerialPortAssistant::newUtility);					//新建...
	connect(this->m_pOpenAction, &QAction::triggered, this, &SerialPortAssistant::openUtility);					//打开...
	connect(this->m_pSaveAction, &QAction::triggered, this, &SerialPortAssistant::saveUtility);					//保存...
	connect(this->m_pASCIICodeAction, &QAction::triggered, this, &SerialPortAssistant::openASCIIDlg);			//打开ASCII对话框
	connect(this->m_pForwardAction, &QAction::triggered, this, &SerialPortAssistant::setWinForwoardOrNot);		//是否置顶
	connect(this->m_pQuickSetAction, &QAction::triggered, this, &SerialPortAssistant::quickSetting);			//快速设置
	connect(this->m_pAddPortAction, &QAction::triggered, this, &SerialPortAssistant::addChannelBtnClicked);		//添加新的端口
	connect(this->m_pDelPortActin, &QAction::triggered, this, &SerialPortAssistant::delChannelBtnClicked);		//删除端口
	connect(this->m_pClearAction, &QAction::triggered, this, &SerialPortAssistant::clearAllMsg);				//清除按钮
	connect(this->m_pAlignmentGroup, &QActionGroup::triggered, this, &SerialPortAssistant::changeAligment);		//重新布局)
	connect(this->m_pRecordLogAction, &QAction::triggered, this, &SerialPortAssistant::setLogRecordState);		//设置是否打开日志记录
	connect(this->m_pOpenLogAction, &QAction::triggered, this, &SerialPortAssistant::openLogFolder);			//打开日志文件夹
	connect(this->m_pViewCurLogActin, &QAction::triggered, this, &SerialPortAssistant::viewCurDataLog);			//查看当前日志
	connect(this->m_pExitAction, &QAction::triggered, this, &SerialPortAssistant::close);						//退出
	connect(this->m_pAboutAction, &QAction::triggered, this, &SerialPortAssistant::about);						//关于
	connect(this->m_pAboutQtAction, &QAction::triggered, this, &QApplication::aboutQt);							//关于Qt
}

/**
* @brief		窗体关闭事件
* @param event  关闭事件类
*
* @return		void
*/
void SerialPortAssistant::closeEvent(QCloseEvent *event)
{

}

/**
* @brief		新建
* @return		void
*/
void SerialPortAssistant::newUtility()
{
	//把所有窗体删除，恢复到原始状态，即为新建
	QLayout *layout = m_pCenWidget->layout();

	for (int i = 1; i < m_listChildWidget.size(); i++)
	{
		m_listChildWidget[i]->doDisconnect();
		m_listChildWidget[i]->setParent(nullptr);			//这里一定要设置为null
		layout->removeWidget(m_listChildWidget[i]);			//从布局里移除
		m_listChildWidget[i]->deleteLater();				//调用析构

		m_listChildWidget.removeAt(i);
	}

	m_pQuickSetAction->setChecked(true);

	m_listChildWidget[0]->doDisconnect();
	m_listChildWidget[0]->restoreDefault();
	m_listChildWidget[0]->doQuckSetting(true);

	this->m_pRecordLogAction->setChecked(false);
	setLogRecordState();

	setActionChecked(EnmCurRunState::StateStop);	//设置状态为停止
	//this->setMinimumHeight(500);
	this->resize(QSize(700, 550));
}

/**
* @brief		保存
* @return		void
*/
void SerialPortAssistant::saveUtility()
{
	//存储当前的信息， 可以进行打开， 来快速配置。。
	QString filePath = QFileDialog::getSaveFileName(this, QStringLiteral("保存配置"), QCoreApplication::applicationDirPath(), "SerialPortAssistant(*.spa)");

	if (filePath.isEmpty())
	{
		return;
	}

	//保存文件的格式...
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, QStringLiteral("保存"), QStringLiteral("文件保存失败, 文件打开失败"), QMessageBox::Ok);
		return;
	}

	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_5_8);

	//获取第一个通道的配置
	SettingCollect collect = m_listChildWidget[0]->getSetting();

	//保存当前第一个通道的配置信息...（简单的处理，本来应该把所有通道的配置都保存一下)
	out << MagicNumber;	//写入魔幻数
	out << collect;

	if (file.flush())
	{
		QMessageBox::information(this, QStringLiteral("保存"), QStringLiteral("保存文件成功！"), QMessageBox::Ok);
	}
	else
	{
		QMessageBox::warning(this, QStringLiteral("保存"), QStringLiteral("文件保存失败, 写入失败"), QMessageBox::Ok);
	}
}

/**
* @brief		打开
* @return		void
*/
void SerialPortAssistant::openUtility()
{
	QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("打开配置"), QCoreApplication::applicationDirPath(), "SerialPortAssistant(*.spa)");

	if (filePath.isEmpty())
	{
		return;
	}

	//读取文件
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("文件打开失败"), QMessageBox::Ok);
		return;
	}

	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_5_8);
	quint32 magic;
	in >> magic;

	if (magic != MagicNumber)
	{
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("文件格式错误，无法解析"), QMessageBox::Ok);
		return;
	}

	SettingCollect collect;
	in >> collect;
	
	newUtility();		//打开配置文件后把其它通道关闭
	Q_ASSERT(m_listChildWidget.size() != 0);

	m_listChildWidget[0]->initSetting(collect);		//利用配置文件初始化当前的窗口
}

/**
* @brief		开始按钮单击
* @return		void
*/
void SerialPortAssistant::startBtnClicked()
{
	Q_ASSERT(m_listChildWidget.size() > 0);		//m_listChildWidget 至少应该有一个

	if (m_listChildWidget.size() == 1)
	{
		//当前只有一个窗体，所以没有使用多窗口机制，需要更新按钮状态
		if (m_listChildWidget[0]->doConnect())
		{
			setActionChecked(EnmCurRunState::StateStart);
		}
		else
		{
			setActionChecked(EnmCurRunState::StateStop);
		}
	}
	else
	{
		//多个窗体下， 遍历所有窗体连接
		for (auto iter = m_listChildWidget.begin(); iter != m_listChildWidget.end(); iter++)
		{
			(*iter)->doConnect();
		}
		//把当前按钮的状态改为运行状态
		setActionChecked(EnmCurRunState::StateStart);
	}	
}

/**
* @brief		暂停按钮单击
* @return		void
*/
void SerialPortAssistant::pauseBtnClicked()
{
	//暂停不写了。。就设置下状态吧
	setActionChecked(EnmCurRunState::StatePause);
}

/**
* @brief		断开连接按钮单击
* @return		void
*/
void SerialPortAssistant::closeBtnClicked()
{
	for (auto iter = m_listChildWidget.begin(); iter != m_listChildWidget.end(); iter++)
	{
		(*iter)->doDisconnect();
	}

	setActionChecked(EnmCurRunState::StateStop);//改变按钮状态
}

/**
* @brief		添加新的通道
* @return		void
*/
void SerialPortAssistant::addChannelBtnClicked()
{
#if 0
	//第一种方式，打破之前的布局，创建一个新的布局，但是会有闪屏的现象， 并且好像有点bug。。
	ChildWidget *childWidget = new ChildWidget(this);

	m_listChildWidget.push_back(childWidget);
	
	int itemCount = m_pCenWidget->layout()->count();

	for (int i = (itemCount - 1); i >= 0; --i)
	{
		QLayoutItem *item = m_pCenWidget->layout()->takeAt(i);
		if (item != 0)
		{
			m_pCenWidget->layout()->removeWidget(item->widget());
			item->widget()->setParent(nullptr);
		}
	}

	delete m_pCenWidget->layout();

	QVBoxLayout *layout = new QVBoxLayout;
	for (int i = 0; i < m_listChildWidget.size(); i++)
	{
		layout->addWidget(m_listChildWidget.at(i));
	}

	m_pCenWidget->setLayout(layout);
#else
	//第二种方式，采用QGridLayout来布局，因为可以适应QHBoxLayout 和 QVBoxLayout
	SettingDlg setDlg(this);
	if (setDlg.exec() == QDialog::Rejected)
	{
		return;
	}
	//获取设置的信息
	SettingCollect setInfo = setDlg.getSettingInfo();

	ChildWidget *childWidget = new ChildWidget(this);
	childWidget->initSetting(setInfo);
	if (m_pRecordLogAction->isChecked())
	{
		//打开日志记录
		childWidget->setLogPath(m_strLogPath);
		childWidget->setLogState(true);
	}

	m_listChildWidget.push_back(childWidget);

	QGridLayout *gridLayout = qobject_cast<QGridLayout *>(m_pCenWidget->layout());

	if (m_pHorAction->isChecked())
	{
		//水平布局
		gridLayout->addWidget(childWidget, 0, m_listChildWidget.size() - 1);
	}
	else if (m_pVertAction->isChecked())
	{
		//垂直布局
		gridLayout->addWidget(childWidget);
	}
	else if (m_pGridAction->isChecked())
	{
		int rows = (m_listChildWidget.size() - 1) / 2;	//求出需要多少行
		int cols = (m_listChildWidget.size() - 1) % 2;	//列
		//网格布局 默认两列
		gridLayout->addWidget(childWidget, rows, cols);
	}
	else if (m_pCustomAction->isChecked())
	{
		int rows = (m_listChildWidget.size() - 1) / m_iCustomLayoutCols;	//求出需要多少行
		int cols = (m_listChildWidget.size() - 1) % m_iCustomLayoutCols;	//列
		//自定义网格布局
		gridLayout->addWidget(childWidget, rows, cols);
	}

	//当采用多通道时， 取消快捷设置
	m_pQuickSetAction->setChecked(false);
	quickSetting();

	//删除通道按钮设置为可用
	m_pDelPortActin->setEnabled(true);
#endif
}

/**
* @brief		删除通道
* @return		void
*/
void SerialPortAssistant::delChannelBtnClicked()
{
	QLayout *layout = m_pCenWidget->layout();

	if (m_listChildWidget.size() == 1)
		return;

	//删除最后一个通道
	int delIndex = m_listChildWidget.size() - 1;

	ChildWidget *cellWidget = m_listChildWidget.at(delIndex);

	cellWidget->setParent(nullptr);			//这里一定要设置为null
	layout->removeWidget(cellWidget);		//从布局里移除
	cellWidget->deleteLater();				//调用析构

	m_listChildWidget.removeAt(delIndex);

	m_pDelPortActin->setEnabled(m_listChildWidget.size() > 1);
}

/**
* @brief		布局方式按钮
* @return		void
*/
void SerialPortAssistant::changeAligment(QAction *pAction)
{
	//如果是自定义的话， 先确定是否输入列数
	if (pAction->text() == QStringLiteral("自定义"))
	{
		CustomColDlg dlg(this);
		if (dlg.exec() == QDialog::Rejected)
		{
			//先回到之前的状态
			if (m_lastLayout == EnmLastLayout::HBoxLayout)
			{
				m_pHorAction->setChecked(true);
			}
			else if (m_lastLayout == EnmLastLayout::VBoxLayout)
			{
				m_pVertAction->setChecked(true);
			}
			else if (m_lastLayout == EnmLastLayout::GridLayout)
			{
				m_pGridAction->setChecked(true);
			}

			return;
		}

		m_iCustomLayoutCols = dlg.getCols();	//获取输入的列数
	}

	//如果只有一个窗体 不需理会
	if (m_listChildWidget.size() == 1)
	{
		return;
	}

	QLayout *lastLayout = qobject_cast<QLayout *>(m_pCenWidget->layout());

	//删除之前的所有布局
	for (int i = 0; i < m_listChildWidget.size(); i++)
	{
		lastLayout->removeWidget(m_listChildWidget[i]);
		m_listChildWidget[i]->setParent(nullptr);
	}

	delete lastLayout;

	if (pAction->text() == QStringLiteral("水平"))
	{
		QHBoxLayout *hLayout = new QHBoxLayout;

		for (int i = 0; i < m_listChildWidget.size(); i++)
		{
			hLayout->addWidget(m_listChildWidget[i]);
		}

		m_pCenWidget->setLayout(hLayout);
		m_lastLayout = EnmLastLayout::HBoxLayout;
	}
	else if (pAction->text() == QStringLiteral("垂直"))
	{
		QVBoxLayout *vLayout = new QVBoxLayout;
		for (int i = 0; i < m_listChildWidget.size(); i++)
		{
			vLayout->addWidget(m_listChildWidget[i]);
		}

		m_pCenWidget->setLayout(vLayout);
		m_lastLayout = EnmLastLayout::VBoxLayout;
	}
	else if (pAction->text() == QStringLiteral("网格"))
	{
		QGridLayout *grLayout = new QGridLayout;
		for (int i = 0; i < m_listChildWidget.size(); i++)
		{
			grLayout->addWidget(m_listChildWidget[i], int(i / 2), i % 2);
		}

		m_pCenWidget->setLayout(grLayout);
		m_lastLayout = EnmLastLayout::GridLayout;
	}
	else if (pAction->text() == QStringLiteral("自定义"))
	{
		QGridLayout *cuLayout = new QGridLayout;
		for (int i = 0; i < m_listChildWidget.size(); i++)
		{
			cuLayout->addWidget(m_listChildWidget[i], int(i / m_iCustomLayoutCols), i % m_iCustomLayoutCols);
		}

		m_pCenWidget->setLayout(cuLayout);
		m_lastLayout = EnmLastLayout::CustLayout;
	}
}

/**
* @brief		清除所有窗口的信息
* @return		void
*/
void SerialPortAssistant::clearAllMsg()
{
	for (auto iter = m_listChildWidget.begin(); iter != m_listChildWidget.end(); iter++)
	{
		(*iter)->doClearEdit();
	}
}

/**
* @brief		设置对话框
* @return		void
*/
void SerialPortAssistant::openSettingDlg()
{
	SettingDlg dlg(this);
	if (dlg.exec())
	{
		Q_ASSERT(m_listChildWidget.size() > 0);
		//只简单的设置第一个通道。。
		if (m_listChildWidget.size() == 1)
		{
			//只有一个通道的话， 把状态设置为停止
			setActionChecked(EnmCurRunState::StateStop);
		}
		m_listChildWidget[0]->doDisconnect();	//先断开连接。
		m_listChildWidget[0]->initSetting(dlg.getSettingInfo());//再设置参数值
	}
}

/**
* @brief		ASCII对话框
* @return		void
*/
void SerialPortAssistant::openASCIIDlg()
{
	ASCIIDlg *dlg = new ASCIIDlg(this);
	dlg->show();
}

/**
* @brief			根据状态改变动作checked
* @param state		当前的状态
* @return			void
*/
void SerialPortAssistant::setActionChecked(EnmCurRunState state)
{
	if (state == EnmCurRunState::StateStart)
	{
		m_pStartAction->setChecked(true);
		m_pPauseAction->setChecked(false);
		m_pStopAction->setChecked(false);
	}
	else if (state == EnmCurRunState::StatePause)
	{
		m_pStartAction->setChecked(false);
		m_pPauseAction->setChecked(true);
		m_pStopAction->setChecked(false);
	}
	else if (state == EnmCurRunState::StateStop)
	{
		m_pStartAction->setChecked(false);
		m_pPauseAction->setChecked(false);
		m_pStopAction->setChecked(true);
	}

	//else unknown
}

/**
* @brief			 是否置顶
* @return			 void
*/
void SerialPortAssistant::setWinForwoardOrNot()
{
	if (m_pForwardAction->isChecked())
	{
		//置顶
		::SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		/*第二种方式
		this->hide();
		this->setWindowFlags(Qt::WindowStaysOnTopHint);
		this->show();*/
	}
	else
	{
		::SetWindowPos((HWND)this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		/*this->hide();
		this->setWindowFlags(Qt::Widget);
		this->show();*/
	}
}

/**
* @brief			 是否打开日志记录
* @return			 void
*/
void SerialPortAssistant::setLogRecordState()
{
	if (m_pRecordLogAction->isChecked())
	{
		for (int i = 0; i < m_listChildWidget.size(); ++i)
		{
			m_listChildWidget[i]->setLogPath(m_strLogPath);
			m_listChildWidget[i]->setLogState(true);
		}
	}
	else
	{
		for (int i = 0; i < m_listChildWidget.size(); ++i)
		{
			m_listChildWidget[i]->setLogState(false);
		}
	}
	
}

/**
* @brief			 打开日志文件夹
* @return			 void
*/
void SerialPortAssistant::openLogFolder()
{
	auto dir = QCoreApplication::applicationDirPath();
	QDesktopServices::openUrl(QUrl(dir));
}

/**
* @brief			 打开日志文本
* @return			 void
*/
void SerialPortAssistant::viewCurDataLog()
{
	QProcess *process = new QProcess;
	QStringList list;
	list << m_strLogPath;
	process->start("Notepad.exe", list);
}

/**
* @brief			 是否打开快速设置
* @return			 void
*/
void SerialPortAssistant::quickSetting()
{
	for (auto iter = m_listChildWidget.begin(); iter != m_listChildWidget.end(); iter++)
	{
		(*iter)->doQuckSetting(m_pQuickSetAction->isChecked());
	}
}

/**
* @brief			 关于对话框
* @return			 void
*/
void SerialPortAssistant::about()
{
	AboutDlg dlg(this);
	dlg.exec();
}
