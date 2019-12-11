#pragma once

#include <QDialog>

class QSpinBox;
class QPushButton;

class CustomColDlg : public QDialog
{
	Q_OBJECT

public:
	CustomColDlg(QWidget *parent);
	~CustomColDlg();
	int getCols();

private:
	QSpinBox *m_pSpinboxCols;
	QPushButton *m_pBtnOk;
	QPushButton *m_pBtnCancel;

	void initViewer();
	void initConnect();

	Q_SLOT void okBtnClicked();
};
