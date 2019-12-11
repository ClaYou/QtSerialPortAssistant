#include "CustomColDlg.h"
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

CustomColDlg::CustomColDlg(QWidget *parent)
	: QDialog(parent)
{
	initViewer();
	initConnect();
}

CustomColDlg::~CustomColDlg()
{
	
}

/**
* @brief		初始化界面参数
* @return		void
*/
void CustomColDlg::initViewer()
{
	QLabel *labInfo = new QLabel("Port Number Per Column:");
	m_pSpinboxCols = new QSpinBox;
	m_pSpinboxCols->setRange(1, 16);
	m_pSpinboxCols->setValue(2);
	m_pSpinboxCols->selectAll();

	m_pBtnOk = new QPushButton("OK");
	m_pBtnCancel = new QPushButton("Cancel");
	QHBoxLayout *btnLayout = new QHBoxLayout;
	btnLayout->addStretch();
	btnLayout->addWidget(m_pBtnOk);
	btnLayout->addWidget(m_pBtnCancel);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(labInfo);
	mainLayout->addWidget(m_pSpinboxCols);
	mainLayout->addLayout(btnLayout);

	this->setLayout(mainLayout);
}

/**
* @brief		初始化信号槽连接
* @return		void
*/
void CustomColDlg::initConnect()
{
	connect(m_pBtnOk, &QPushButton::clicked, this, &CustomColDlg::okBtnClicked);
	connect(m_pBtnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

/**
* @brief		OK按钮单击
* @return		void
*/
void CustomColDlg::okBtnClicked()
{
	this->accept();
}

/**
* @brief		获取输入的列数
* @return		void
*/
int CustomColDlg::getCols()
{
	return this->m_pSpinboxCols->value();
}