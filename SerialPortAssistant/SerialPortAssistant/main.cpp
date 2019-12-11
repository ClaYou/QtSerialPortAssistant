#include "SerialPortAssistant.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shell32.lib")

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	SerialPortAssistant w;
	w.show();
	return a.exec();
}
