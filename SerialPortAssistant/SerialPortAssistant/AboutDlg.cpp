#include "AboutDlg.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

AboutDlg::AboutDlg(QWidget *parent)
	: QDialog(parent)
{
	initViewer();
}

AboutDlg::~AboutDlg()
{
}

/**
* @brief		初始化界面参数
* @return		void
*/
void AboutDlg::initViewer()
{
	this->setWindowTitle(QStringLiteral("关于"));

	QString strInfo;
	strInfo.append(tr("Version: 1.0.0\n"));
	strInfo.append("Copyright @ 2019\n");
	strInfo.append("Email: xb_hu0926@163.com\n");
	strInfo.append("QQ: 383865602\n\n");

	QFont font(QStringLiteral("Microsoft YaHei"), 12, 75);
	m_pLabTitle = new QLabel(QStringLiteral("<b>串口网络调试工具</b>"));
	m_pLabTitle->setFont(font);

	QFont font2(QStringLiteral("新宋体"), 10);
	m_pLabInfo = new QLabel;
	m_pLabInfo->setFont(font2);
	m_pLabInfo->setWordWrap(true);
	//m_labInfo->setMidLineWidth(500);
	m_pLabInfo->setText(strInfo);

	m_pLabIcon = new QLabel;
	QPixmap pixmap(":/images/Icon.png");
	pixmap.scaled(QSize(60,60), Qt::KeepAspectRatio);
	m_pLabIcon->setPixmap(pixmap);

	m_pBtnOk = new QPushButton("OK");
	connect(m_pBtnOk, &QPushButton::clicked, this, &AboutDlg::close);

	QVBoxLayout *iconLayout = new QVBoxLayout;
	iconLayout->addWidget(m_pLabIcon);
	iconLayout->addStretch(1);

	QVBoxLayout *infoLayout = new QVBoxLayout;
	infoLayout->addWidget(m_pLabTitle);
	infoLayout->addWidget(m_pLabInfo);
	infoLayout->addStretch(1);

	QHBoxLayout *btnLayout = new QHBoxLayout;
	btnLayout->addStretch(1);
	btnLayout->addWidget(m_pBtnOk);

	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->setSpacing(20);
	topLayout->addLayout(iconLayout);
	topLayout->addStretch(1);
	topLayout->addLayout(infoLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addStretch(1);
	mainLayout->addLayout(btnLayout);

	this->setLayout(mainLayout);

	//this->resize(400, 300);
}
