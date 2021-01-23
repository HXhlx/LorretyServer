#pragma once

#include <Python.h>
#include <QTCPServer>
#include "TCPSocket.h"

class TCPServer : public QTcpServer
{
	Q_OBJECT
private:
	SQL* sql = new SQL;
	PyObject* pModule, * pFun;
	QHash<qintptr, TCPSocket*>tcpSockets;
	QTimer* timer = new QTimer(this);
public:
	TCPServer();
	TCPServer(QObject* parent);
	~TCPServer();
protected:
	void incomingConnection(qintptr handle);
public slots:
	void on_timer_timeout();
	void onClientDisconnected(qintptr handle);
	void onAcceptError(QAbstractSocket::SocketError error);
};
