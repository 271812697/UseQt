#include "mainwindow.h"
#include<log.h>
#include <QApplication>
#include<iostream>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Log::Init();
	CORE_INFO("start the log");
	MainWindow w;
	w.show();
	int res = a.exec();
	Log::Shutdown();
	return res;
}
