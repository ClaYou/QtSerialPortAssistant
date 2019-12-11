#include "ASCIIDlg.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QDebug>

ASCIIDlg::ASCIIDlg(QWidget *parent)
	: QDialog(parent)
{
	initViewer();
	initTable();

	this->setAttribute(Qt::WA_DeleteOnClose);
}

ASCIIDlg::~ASCIIDlg()
{
	qDebug() << "delete";
}

/**
* @brief		初始化界面参数
* @return		void
*/
void ASCIIDlg::initViewer()
{
	m_pTableWidget = new QTableWidget(this);
	m_pTableWidget->setColumnCount(5);
	m_pTableWidget->setRowCount(128);
	m_pTableWidget->setCurrentCell(0, 0);
	m_pTableWidget->horizontalHeader()->setStretchLastSection(true);		//最后一列自适应

	QStringList horHeaders = {"Binary", "Decimal", "Hex", "Abbr/Glyph", "Discription" };
	m_pTableWidget->setHorizontalHeaderLabels(horHeaders);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(m_pTableWidget);
	this->setLayout(mainLayout);

	this->resize(600, 400);
	this->setWindowTitle("ASCII Code Chart");
}

/**
* @brief		初始化表格
* @return		void
*/
void ASCIIDlg::initTable()
{
	
	for (int i = 0; i < 128; i++)
	{
		m_pTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i, 2)));		//二进制
		m_pTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(i)));		//十进制
		m_pTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(i, 16)));	//十六进制
		m_pTableWidget->setItem(i, 3, new QTableWidgetItem(QChar(i)));					//字符
	}

	QVector<QString> abrVec;
	abrVec.push_back("NUL");
	abrVec.push_back("SOH");
	abrVec.push_back("STX");
	abrVec.push_back("ETX");
	abrVec.push_back("EOT");
	abrVec.push_back("ENQ");
	abrVec.push_back("ACK");
	abrVec.push_back("BEL");
	abrVec.push_back("BS");
	abrVec.push_back("HT");
	abrVec.push_back("LF");
	abrVec.push_back("VT");
	abrVec.push_back("FF");
	abrVec.push_back("CR");
	abrVec.push_back("SO");
	abrVec.push_back("SI");
	abrVec.push_back("DLE");
	abrVec.push_back("DC1");
	abrVec.push_back("DC2");
	abrVec.push_back("DC3");
	abrVec.push_back("DC4");
	abrVec.push_back("NAK");
	abrVec.push_back("SYN");
	abrVec.push_back("ETB");
	abrVec.push_back("CAN");
	abrVec.push_back("EM");
	abrVec.push_back("SUB");
	abrVec.push_back("ESC");
	abrVec.push_back("FS");
	abrVec.push_back("GS");
	abrVec.push_back("RS");
	abrVec.push_back("US");
	abrVec.push_back(" ");

	QVector<QString> disVec;
	disVec.push_back("\\0 Null character");
	disVec.push_back("Start Of Header");
	disVec.push_back("Start of Text");
	disVec.push_back("End of Text");
	disVec.push_back("End of Transmission");
	disVec.push_back("Enquiry");
	disVec.push_back("Acknowledgment");
	disVec.push_back("\\a Bell");
	disVec.push_back("\\b Backspace");
	disVec.push_back("\\t Horizontal Tab");
	disVec.push_back("\\n Line feed");
	disVec.push_back("\\v Vertical Tab");
	disVec.push_back("\\f Form feed");
	disVec.push_back("\\r Carriage return");
	disVec.push_back("Shift Out");
	disVec.push_back("Shift In");
	disVec.push_back("Data Link Escape");
	disVec.push_back("Device Control 1 (oft. XON)");
	disVec.push_back("Device Control 2");
	disVec.push_back("Device Control 3 (oft. XOFF)");
	disVec.push_back("Device Control 4");
	disVec.push_back("Negative Acknowledgement");
	disVec.push_back("Synchronous idle");
	disVec.push_back("End of Transmission Block");
	disVec.push_back("Cancel");
	disVec.push_back("End of Medium");
	disVec.push_back("Substitute");
	disVec.push_back("\\e Escape");
	disVec.push_back("File Separator");
	disVec.push_back("Group Separator");
	disVec.push_back("Record Separator");
	disVec.push_back("Unit Separator");
	disVec.push_back("Space");

	Q_ASSERT(abrVec.size() == disVec.size());

	for (int i = 0; i < abrVec.size(); ++i)
	{
		m_pTableWidget->item(i, 3)->setText(abrVec[i]);
		m_pTableWidget->setItem(i, 4, new QTableWidgetItem(disVec[i]));
	}
}
