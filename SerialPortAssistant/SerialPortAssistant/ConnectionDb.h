#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

bool CreateConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("Serial.db");
	db.setUserName(QString("hxb"));
	db.setPassword(QString("123456"));
	if (!db.open())
	{
		QMessageBox::critical(0, QString("严重错误")
			, QString("无法创建数据库\n原因：%1").arg(db.lastError().text()),
			QMessageBox::Ok);
		return false;
	}
	return true;
}

void CreateTable()
{
	QSqlQuery query(QString("CREATE TABLE tb_InputHistory"));
}