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
* @brief		��ʼ���������
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
* @brief		��ʼ���źŲ�����
* @return		void
*/
void CustomColDlg::initConnect()
{
	connect(m_pBtnOk, &QPushButton::clicked, this, &CustomColDlg::okBtnClicked);
	connect(m_pBtnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

/**
* @brief		OK��ť����
* @return		void
*/
void CustomColDlg::okBtnClicked()
{
	this->accept();
}

/**
* @brief		��ȡ���������
* @return		void
*/
int CustomColDlg::getCols()
{
	return this->m_pSpinboxCols->value();
}