#include <QtCore/QCoreApplication>
#include "TCPServer.h"

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	TCPServer server;
	return a.exec();
}
