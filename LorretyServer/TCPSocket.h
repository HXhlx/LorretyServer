#pragma once

#include <QHostInfo>
#include <QTcpSocket>
#include "SQL.h"

class TCPSocket : public QTcpSocket
{
	Q_OBJECT
public:
	quint64 ID;
	QString auth;
	SQL* sql;
	static QMap<quint64,QString>users;
	TCPSocket(QObject* parent);
	~TCPSocket();
public slots:
	void onClientConnected();
	void onClientDisconnected();
	void onSocketReadyRead();
signals:
	void clientDisconnected(qintptr);
};
