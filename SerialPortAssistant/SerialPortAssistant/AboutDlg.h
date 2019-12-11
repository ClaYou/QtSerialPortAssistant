#pragma once

#include <QDialog>

class QLabel;
class QPushButton;

class AboutDlg : public QDialog
{
	Q_OBJECT

public:
	AboutDlg(QWidget *parent);
	~AboutDlg();

private:
	QLabel *m_pLabTitle;
	QLabel *m_pLabInfo;
	QLabel *m_pLabIcon;
	QPushButton *m_pBtnOk;

	void initViewer();
};
