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
* @brief ������Ҫ˵��-���Ժ���
* @param index    ����1
* @param t        ����2 @see CTest
*
* @return ����˵��
*     -<em>false</em> fail
*     -<em>true</em> succeed
*/

SerialPortAssistant::SerialPortAssistant(QWidget *parent)
	: QMainWindow(parent)
{
	initViewer();
}

/**
* @brief		��ʼ����
* @return		void
*/
void SerialPortAssistant::initViewer()
{
	createMenu();
	createToolBtns();
	createCenWgt();
	initConnect();
	this->resize(700, 550);
	this->setWindowTitle(QStringLiteral("����������Թ���"));
	//this->setWindowIcon(QIcon(":/images/Icon.png"));

	m_pStopAction->setChecked(true);	//Ĭ�Ϲر�״̬
	m_lastLayout = EnmLastLayout::HBoxLayout;
	m_iCustomLayoutCols = 2;

	//��¼��־�ļ�
	QString strDate = QDateTime::currentDateTime().toString("yyyyMMdd");
	m_strLogPath =  QCoreApplication::applicationDirPath() +  "/log_" + strDate + ".txt";
}

/**
* @brief		��ʼ���˵�
* @return		void
*/
void SerialPortAssistant::createMenu()
{
	m_pFileMenu = new QMenu(QStringLiteral("�ļ�(&F)"));
	m_pEditMenu = new QMenu(QStringLiteral("�༭(&E)"));
	m_pViewMenu = new QMenu(QStringLiteral("��ͼ(&V)"));
	m_pToolMenu = new QMenu(QStringLiteral("����(&T)"));
	m_pHelpMenu = new QMenu(QStringLiteral("����(&H)"));

	m_pNewAction = new QAction(QStringLiteral("�½�..."));
	m_pNewAction->setIcon(QIcon(":/images/New.png"));
	m_pOpenAction = new QAction(QStringLiteral("��..."));
	m_pOpenAction->setIcon(QIcon(":/images/Open.png"));
	m_pSaveAction = new QAction(QStringLiteral("����..."));
	m_pSaveAction->setIcon(QIcon(":/images/Save.png"));
	m_pRecordLogAction = new QAction(QStringLiteral("Record Data Log"));
	m_pRecordLogAction->setIcon(QIcon(":/images/Log.png"));
	m_pRecordLogAction->setCheckable(true);
	m_pViewCurLogActin = new QAction(QStringLiteral("View Current Data Log"));
	m_pOpenLogAction = new QAction(QStringLiteral("Open Log Folder"));
	m_pExitAction = new QAction(QStringLiteral("�˳�(&X)"));

	m_pAddPortAction = new QAction(QStringLiteral("���Ӷ˿�..."));
	m_pAddPortAction->setIcon(QIcon(":/images/Add.png"));
	m_pDelPortActin = new QAction(QStringLiteral("ɾ���˿�..."));
	m_pDelPortActin->setEnabled(false);
	m_pDelPortActin->setIcon(QIcon(":/images/Delete.png"));
	m_pStartAction = new QAction(QStringLiteral("��ʼ"));
	m_pStartAction->setIcon(QIcon(":/images/Start.png"));
	m_pStartAction->setCheckable(true);
	m_pPauseAction = new QAction(QStringLiteral("��ͣ"));
	m_pPauseAction->setIcon(QIcon(":/images/Pause.png"));
	m_pPauseAction->setCheckable(true);
	m_pStopAction = new QAction(QStringLiteral("ֹͣ"));
	m_pStopAction->setIcon(QIcon(":/images/Stop.png"));
	m_pStopAction->setCheckable(true);
	m_pClearAction = new QAction(QStringLiteral("���"));
	m_pClearAction->setIcon(QIcon(":/images/Clear.png"));

	m_pHorAction = new QAction(QStringLiteral("ˮƽ"));
	m_pHorAction->setCheckable(true);
	m_pHorAction->setChecked(true);
	m_pVertAction = new QAction(QStringLiteral("��ֱ"));
	m_pVertAction->setCheckable(true);
	m_pGridAction = new QAction(QStringLiteral("����"));
	m_pGridAction->setCheckable(true);
	m_pCustomAction = new QAction(QStringLiteral("�Զ���"));
	m_pCustomAction->setCheckable(true);

	m_pAlignmentGroup = new QActionGroup(this);
	m_pAlignmentGroup->addAction(m_pHorAction);
	m_pAlignmentGroup->addAction(m_pVertAction);
	m_pAlignmentGroup->addAction(m_pGridAction);
	m_pAlignmentGroup->addAction(m_pCustomAction);

	m_pQuickSetAction = new QAction(QStringLiteral("��������"));
	m_pQuickSetAction->setIcon(QIcon(":/images/QuickSet.png"));
	m_pQuickSetAction->setCheckable(true);
	m_pQuickSetAction->setChecked(true);
	m_pForwardAction = new QAction(QStringLiteral("ǰ����ʾ"));
	m_pForwardAction->setCheckable(true);

	m_pASCIICodeAction = new QAction(QStringLiteral("ASCII Code..."));
	m_pSetAction = new QAction(QStringLiteral("����"));
	m_pSetAction->setIcon(QIcon(":/images/Set.png"));

	m_pAboutAction = new QAction(QStringLiteral("����(&A)"));
	m_pAboutQtAction = new QAction(QStringLiteral("����Qt(&Q)"));

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
* @brief		��ʼ��������
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
* @brief		��ʼ���ؼ�
* @return		void
*/
void SerialPortAssistant::createCenWgt()
{
	m_pCenWidget = new QWidget;

	ChildWidget *childWgt = new ChildWidget(0);

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->setSpacing(0);
	gridLayout->addWidget(childWgt);
	gridLayout->setMargin(0);			//���ø��������ļ��Ϊ0 
	m_pCenWidget->setLayout(gridLayout);

	this->setCentralWidget(m_pCenWidget);

	m_listChildWidget.push_back(childWgt);
}

/**
* @brief		��ʼ���źŲ۵�������Ϣ
* @return		void
*/
void SerialPortAssistant::initConnect()
{
	connect(this->m_pStartAction, &QAction::triggered, this, &SerialPortAssistant::startBtnClicked);			//���Ӱ�ť
	connect(this->m_pPauseAction, &QAction::triggered, this, &SerialPortAssistant::pauseBtnClicked);			//��ͣ��
	connect(this->m_pStopAction, &QAction::triggered, this, &SerialPortAssistant::closeBtnClicked);				//�Ͽ����Ӱ�ť
	connect(this->m_pSetAction, &QAction::triggered, this, &SerialPortAssistant::openSettingDlg);				//���öԻ���
	connect(this->m_pNewAction, &QAction::triggered, this, &SerialPortAssistant::newUtility);					//�½�...
	connect(this->m_pOpenAction, &QAction::triggered, this, &SerialPortAssistant::openUtility);					//��...
	connect(this->m_pSaveAction, &QAction::triggered, this, &SerialPortAssistant::saveUtility);					//����...
	connect(this->m_pASCIICodeAction, &QAction::triggered, this, &SerialPortAssistant::openASCIIDlg);			//��ASCII�Ի���
	connect(this->m_pForwardAction, &QAction::triggered, this, &SerialPortAssistant::setWinForwoardOrNot);		//�Ƿ��ö�
	connect(this->m_pQuickSetAction, &QAction::triggered, this, &SerialPortAssistant::quickSetting);			//��������
	connect(this->m_pAddPortAction, &QAction::triggered, this, &SerialPortAssistant::addChannelBtnClicked);		//����µĶ˿�
	connect(this->m_pDelPortActin, &QAction::triggered, this, &SerialPortAssistant::delChannelBtnClicked);		//ɾ���˿�
	connect(this->m_pClearAction, &QAction::triggered, this, &SerialPortAssistant::clearAllMsg);				//�����ť
	connect(this->m_pAlignmentGroup, &QActionGroup::triggered, this, &SerialPortAssistant::changeAligment);		//���²���)
	connect(this->m_pRecordLogAction, &QAction::triggered, this, &SerialPortAssistant::setLogRecordState);		//�����Ƿ����־��¼
	connect(this->m_pOpenLogAction, &QAction::triggered, this, &SerialPortAssistant::openLogFolder);			//����־�ļ���
	connect(this->m_pViewCurLogActin, &QAction::triggered, this, &SerialPortAssistant::viewCurDataLog);			//�鿴��ǰ��־
	connect(this->m_pExitAction, &QAction::triggered, this, &SerialPortAssistant::close);						//�˳�
	connect(this->m_pAboutAction, &QAction::triggered, this, &SerialPortAssistant::about);						//����
	connect(this->m_pAboutQtAction, &QAction::triggered, this, &QApplication::aboutQt);							//����Qt
}

/**
* @brief		����ر��¼�
* @param event  �ر��¼���
*
* @return		void
*/
void SerialPortAssistant::closeEvent(QCloseEvent *event)
{

}

/**
* @brief		�½�
* @return		void
*/
void SerialPortAssistant::newUtility()
{
	//�����д���ɾ�����ָ���ԭʼ״̬����Ϊ�½�
	QLayout *layout = m_pCenWidget->layout();

	for (int i = 1; i < m_listChildWidget.size(); i++)
	{
		m_listChildWidget[i]->doDisconnect();
		m_listChildWidget[i]->setParent(nullptr);			//����һ��Ҫ����Ϊnull
		layout->removeWidget(m_listChildWidget[i]);			//�Ӳ������Ƴ�
		m_listChildWidget[i]->deleteLater();				//��������

		m_listChildWidget.removeAt(i);
	}

	m_pQuickSetAction->setChecked(true);

	m_listChildWidget[0]->doDisconnect();
	m_listChildWidget[0]->restoreDefault();
	m_listChildWidget[0]->doQuckSetting(true);

	this->m_pRecordLogAction->setChecked(false);
	setLogRecordState();

	setActionChecked(EnmCurRunState::StateStop);	//����״̬Ϊֹͣ
	//this->setMinimumHeight(500);
	this->resize(QSize(700, 550));
}

/**
* @brief		����
* @return		void
*/
void SerialPortAssistant::saveUtility()
{
	//�洢��ǰ����Ϣ�� ���Խ��д򿪣� ���������á���
	QString filePath = QFileDialog::getSaveFileName(this, QStringLiteral("��������"), QCoreApplication::applicationDirPath(), "SerialPortAssistant(*.spa)");

	if (filePath.isEmpty())
	{
		return;
	}

	//�����ļ��ĸ�ʽ...
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, QStringLiteral("����"), QStringLiteral("�ļ�����ʧ��, �ļ���ʧ��"), QMessageBox::Ok);
		return;
	}

	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_5_8);

	//��ȡ��һ��ͨ��������
	SettingCollect collect = m_listChildWidget[0]->getSetting();

	//���浱ǰ��һ��ͨ����������Ϣ...���򵥵Ĵ�������Ӧ�ð�����ͨ�������ö�����һ��)
	out << MagicNumber;	//д��ħ����
	out << collect;

	if (file.flush())
	{
		QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("�����ļ��ɹ���"), QMessageBox::Ok);
	}
	else
	{
		QMessageBox::warning(this, QStringLiteral("����"), QStringLiteral("�ļ�����ʧ��, д��ʧ��"), QMessageBox::Ok);
	}
}

/**
* @brief		��
* @return		void
*/
void SerialPortAssistant::openUtility()
{
	QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("������"), QCoreApplication::applicationDirPath(), "SerialPortAssistant(*.spa)");

	if (filePath.isEmpty())
	{
		return;
	}

	//��ȡ�ļ�
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, QStringLiteral("����"), QStringLiteral("�ļ���ʧ��"), QMessageBox::Ok);
		return;
	}

	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_5_8);
	quint32 magic;
	in >> magic;

	if (magic != MagicNumber)
	{
		QMessageBox::warning(this, QStringLiteral("����"), QStringLiteral("�ļ���ʽ�����޷�����"), QMessageBox::Ok);
		return;
	}

	SettingCollect collect;
	in >> collect;
	
	newUtility();		//�������ļ��������ͨ���ر�
	Q_ASSERT(m_listChildWidget.size() != 0);

	m_listChildWidget[0]->initSetting(collect);		//���������ļ���ʼ����ǰ�Ĵ���
}

/**
* @brief		��ʼ��ť����
* @return		void
*/
void SerialPortAssistant::startBtnClicked()
{
	Q_ASSERT(m_listChildWidget.size() > 0);		//m_listChildWidget ����Ӧ����һ��

	if (m_listChildWidget.size() == 1)
	{
		//��ǰֻ��һ�����壬����û��ʹ�öര�ڻ��ƣ���Ҫ���°�ť״̬
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
		//��������£� �������д�������
		for (auto iter = m_listChildWidget.begin(); iter != m_listChildWidget.end(); iter++)
		{
			(*iter)->doConnect();
		}
		//�ѵ�ǰ��ť��״̬��Ϊ����״̬
		setActionChecked(EnmCurRunState::StateStart);
	}	
}

/**
* @brief		��ͣ��ť����
* @return		void
*/
void SerialPortAssistant::pauseBtnClicked()
{
	//��ͣ��д�ˡ�����������״̬��
	setActionChecked(EnmCurRunState::StatePause);
}

/**
* @brief		�Ͽ����Ӱ�ť����
* @return		void
*/
void SerialPortAssistant::closeBtnClicked()
{
	for (auto iter = m_listChildWidget.begin(); iter != m_listChildWidget.end(); iter++)
	{
		(*iter)->doDisconnect();
	}

	setActionChecked(EnmCurRunState::StateStop);//�ı䰴ť״̬
}

/**
* @brief		����µ�ͨ��
* @return		void
*/
void SerialPortAssistant::addChannelBtnClicked()
{
#if 0
	//��һ�ַ�ʽ������֮ǰ�Ĳ��֣�����һ���µĲ��֣����ǻ������������� ���Һ����е�bug����
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
	//�ڶ��ַ�ʽ������QGridLayout�����֣���Ϊ������ӦQHBoxLayout �� QVBoxLayout
	SettingDlg setDlg(this);
	if (setDlg.exec() == QDialog::Rejected)
	{
		return;
	}
	//��ȡ���õ���Ϣ
	SettingCollect setInfo = setDlg.getSettingInfo();

	ChildWidget *childWidget = new ChildWidget(this);
	childWidget->initSetting(setInfo);
	if (m_pRecordLogAction->isChecked())
	{
		//����־��¼
		childWidget->setLogPath(m_strLogPath);
		childWidget->setLogState(true);
	}

	m_listChildWidget.push_back(childWidget);

	QGridLayout *gridLayout = qobject_cast<QGridLayout *>(m_pCenWidget->layout());

	if (m_pHorAction->isChecked())
	{
		//ˮƽ����
		gridLayout->addWidget(childWidget, 0, m_listChildWidget.size() - 1);
	}
	else if (m_pVertAction->isChecked())
	{
		//��ֱ����
		gridLayout->addWidget(childWidget);
	}
	else if (m_pGridAction->isChecked())
	{
		int rows = (m_listChildWidget.size() - 1) / 2;	//�����Ҫ������
		int cols = (m_listChildWidget.size() - 1) % 2;	//��
		//���񲼾� Ĭ������
		gridLayout->addWidget(childWidget, rows, cols);
	}
	else if (m_pCustomAction->isChecked())
	{
		int rows = (m_listChildWidget.size() - 1) / m_iCustomLayoutCols;	//�����Ҫ������
		int cols = (m_listChildWidget.size() - 1) % m_iCustomLayoutCols;	//��
		//�Զ������񲼾�
		gridLayout->addWidget(childWidget, rows, cols);
	}

	//�����ö�ͨ��ʱ�� ȡ���������
	m_pQuickSetAction->setChecked(false);
	quickSetting();

	//ɾ��ͨ����ť����Ϊ����
	m_pDelPortActin->setEnabled(true);
#endif
}

/**
* @brief		ɾ��ͨ��
* @return		void
*/
void SerialPortAssistant::delChannelBtnClicked()
{
	QLayout *layout = m_pCenWidget->layout();

	if (m_listChildWidget.size() == 1)
		return;

	//ɾ�����һ��ͨ��
	int delIndex = m_listChildWidget.size() - 1;

	ChildWidget *cellWidget = m_listChildWidget.at(delIndex);

	cellWidget->setParent(nullptr);			//����һ��Ҫ����Ϊnull
	layout->removeWidget(cellWidget);		//�Ӳ������Ƴ�
	cellWidget->deleteLater();				//��������

	m_listChildWidget.removeAt(delIndex);

	m_pDelPortActin->setEnabled(m_listChildWidget.size() > 1);
}

/**
* @brief		���ַ�ʽ��ť
* @return		void
*/
void SerialPortAssistant::changeAligment(QAction *pAction)
{
	//������Զ���Ļ��� ��ȷ���Ƿ���������
	if (pAction->text() == QStringLiteral("�Զ���"))
	{
		CustomColDlg dlg(this);
		if (dlg.exec() == QDialog::Rejected)
		{
			//�Ȼص�֮ǰ��״̬
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

		m_iCustomLayoutCols = dlg.getCols();	//��ȡ���������
	}

	//���ֻ��һ������ �������
	if (m_listChildWidget.size() == 1)
	{
		return;
	}

	QLayout *lastLayout = qobject_cast<QLayout *>(m_pCenWidget->layout());

	//ɾ��֮ǰ�����в���
	for (int i = 0; i < m_listChildWidget.size(); i++)
	{
		lastLayout->removeWidget(m_listChildWidget[i]);
		m_listChildWidget[i]->setParent(nullptr);
	}

	delete lastLayout;

	if (pAction->text() == QStringLiteral("ˮƽ"))
	{
		QHBoxLayout *hLayout = new QHBoxLayout;

		for (int i = 0; i < m_listChildWidget.size(); i++)
		{
			hLayout->addWidget(m_listChildWidget[i]);
		}

		m_pCenWidget->setLayout(hLayout);
		m_lastLayout = EnmLastLayout::HBoxLayout;
	}
	else if (pAction->text() == QStringLiteral("��ֱ"))
	{
		QVBoxLayout *vLayout = new QVBoxLayout;
		for (int i = 0; i < m_listChildWidget.size(); i++)
		{
			vLayout->addWidget(m_listChildWidget[i]);
		}

		m_pCenWidget->setLayout(vLayout);
		m_lastLayout = EnmLastLayout::VBoxLayout;
	}
	else if (pAction->text() == QStringLiteral("����"))
	{
		QGridLayout *grLayout = new QGridLayout;
		for (int i = 0; i < m_listChildWidget.size(); i++)
		{
			grLayout->addWidget(m_listChildWidget[i], int(i / 2), i % 2);
		}

		m_pCenWidget->setLayout(grLayout);
		m_lastLayout = EnmLastLayout::GridLayout;
	}
	else if (pAction->text() == QStringLiteral("�Զ���"))
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
* @brief		������д��ڵ���Ϣ
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
* @brief		���öԻ���
* @return		void
*/
void SerialPortAssistant::openSettingDlg()
{
	SettingDlg dlg(this);
	if (dlg.exec())
	{
		Q_ASSERT(m_listChildWidget.size() > 0);
		//ֻ�򵥵����õ�һ��ͨ������
		if (m_listChildWidget.size() == 1)
		{
			//ֻ��һ��ͨ���Ļ��� ��״̬����Ϊֹͣ
			setActionChecked(EnmCurRunState::StateStop);
		}
		m_listChildWidget[0]->doDisconnect();	//�ȶϿ����ӡ�
		m_listChildWidget[0]->initSetting(dlg.getSettingInfo());//�����ò���ֵ
	}
}

/**
* @brief		ASCII�Ի���
* @return		void
*/
void SerialPortAssistant::openASCIIDlg()
{
	ASCIIDlg *dlg = new ASCIIDlg(this);
	dlg->show();
}

/**
* @brief			����״̬�ı䶯��checked
* @param state		��ǰ��״̬
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
* @brief			 �Ƿ��ö�
* @return			 void
*/
void SerialPortAssistant::setWinForwoardOrNot()
{
	if (m_pForwardAction->isChecked())
	{
		//�ö�
		::SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		/*�ڶ��ַ�ʽ
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
* @brief			 �Ƿ����־��¼
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
* @brief			 ����־�ļ���
* @return			 void
*/
void SerialPortAssistant::openLogFolder()
{
	auto dir = QCoreApplication::applicationDirPath();
	QDesktopServices::openUrl(QUrl(dir));
}

/**
* @brief			 ����־�ı�
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
* @brief			 �Ƿ�򿪿�������
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
* @brief			 ���ڶԻ���
* @return			 void
*/
void SerialPortAssistant::about()
{
	AboutDlg dlg(this);
	dlg.exec();
}
