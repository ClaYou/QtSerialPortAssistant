#pragma once

#include <QDialog>

class QTableWidget;

class ASCIIDlg : public QDialog
{
	Q_OBJECT

public:
	ASCIIDlg(QWidget *parent = nullptr);
	~ASCIIDlg();

private:
	void initViewer();
	void initTable();

	QTableWidget *m_pTableWidget;
};
