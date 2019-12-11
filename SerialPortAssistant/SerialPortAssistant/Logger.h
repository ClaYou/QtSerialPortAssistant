#pragma once
#include <QString>
#include <QMutex>
#include <QFile>
#include <QTextStream>

/**
* @brief			简单的日志打印
* @param msg		打印的信息
* @param filePath	文件的路径
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