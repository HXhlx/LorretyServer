#include "TCPServer.h"

TCPServer::TCPServer()
{
	Py_Initialize();
	if (!Py_IsInitialized())
	{
		qCritical() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << "Python��ʼ��ʧ��!";
		throw QException();
	}
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	pModule = PyImport_ImportModule("crawler");
	if (!pModule)
	{
		qCritical() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << "δ�ҵ�Python�ű�";
		throw QException();
	}
	pFun = PyObject_GetAttrString(pModule, "test");
	PyEval_CallObject(pFun, nullptr);
	QString hostName = QHostInfo::localHostName();
	QHostInfo hostInfo = QHostInfo::fromName(hostName);
	QString localIP = "";
	foreach(QHostAddress host, hostInfo.addresses())
		if (QAbstractSocket::IPv4Protocol == host.protocol())
		{
			localIP = host.toString();
			break;
		}
	listen(QHostAddress(localIP), 1234);
	qDebug() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << "��ʼ���� " << serverAddress() << serverPort();
	connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
	connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(onAcceptError(QAbstractSocket::SocketError)));
	timer->start(60000);
}

TCPServer::TCPServer(QObject* parent)
	: QTcpServer(parent)
{
	new(this)TCPServer();
}

TCPServer::~TCPServer()
{
	Py_Finalize();
	close();
}

void TCPServer::incomingConnection(qintptr handle)
{
	TCPSocket* tcpSocket = new TCPSocket(this);
	tcpSocket->setSocketDescriptor(handle);
	tcpSocket->sql = sql;
	tcpSockets[handle] = tcpSocket;
	tcpSocket->onClientConnected();
	connect(tcpSocket, SIGNAL(clientDisconnected(qintptr)), this, SLOT(onClientDisconnected(qintptr)));
}

void TCPServer::onClientDisconnected(qintptr handle)
{
	tcpSockets.remove(handle);
}

void TCPServer::onAcceptError(QAbstractSocket::SocketError error)
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << "���ӳ���. ����ԭ��: " << error;
}

void TCPServer::on_timer_timeout()
{
	QString date = QDateTime::currentDateTime().toString("hh:mm ddd");
	if (date == "22:30 �ܶ�" || date == "22:30 ����" || date == "22:30 ����")
	{
		pFun = PyObject_GetAttrString(pModule, "crawler");
		PyEval_CallObject(pFun, nullptr);
	}
}
