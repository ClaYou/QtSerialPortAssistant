#pragma once
#include <QString>

class ToolsHelp
{
public:
	static QByteArray  QString2Hex(QString str);
	static int ConvertHexChar(char ch);

private:
	ToolsHelp() {}
	~ToolsHelp() {}
};

