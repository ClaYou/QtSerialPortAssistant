#pragma once
#include <QString>
#include <QMutex>
#include <QFile>
#include <QTextStream>

/**
* @brief			�򵥵���־��ӡ
* @param msg		��ӡ����Ϣ
* @param filePath	�ļ���·��
*
* @return			void
*/
static void Logger(const QString &msg, const QString &filePath)
{
	static QMutex mutex;
	mutex.lock();

	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		mutex.unlock();

		return;
	}
	QTextStream text_stream(&file);
	text_stream << msg << "\r\n";
	file.flush();
	file.close();

	mutex.unlock();
}