#include "TCPSocket.h"

QMap<quint64,QString> TCPSocket::users;

TCPSocket::TCPSocket(QObject* parent)
	: QTcpSocket(parent)
{
	connect(this, SIGNAL(connected()), this, SLOT(onClientConnected()));
	connect(this, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
	connect(this, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

TCPSocket::~TCPSocket()
{}

void TCPSocket::onClientConnected()
{
	qDebug() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << "客户端已连接. IP地址:" << peerAddress() << ",端口号:" << peerPort();
}

void TCPSocket::onClientDisconnected()
{
	users.remove(ID);
	qDebug() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << "客户端已断开. IP地址:" << peerAddress() << ",端口号:" << peerPort();
	emit clientDisconnected(socketDescriptor());
}

void TCPSocket::onSocketReadyRead()
{
	auto out = [this] {
		QString str = readLine();
		qDebug() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << str;
		if (str.contains("insert", Qt::CaseInsensitive) || str.contains("update", Qt::CaseInsensitive) || str.contains("delete", Qt::CaseInsensitive))write(QByteArray::number(sql->Operate(str)).append('\n'));
		else if (str.contains("select", Qt::CaseInsensitive))
		{
			auto res = sql->Select(str);
			if (!res.isEmpty() && res[0] == 'u')
			{
				ID = res.mid(2, res.indexOf('|') - 2).toULongLong();
				auth = res.left(res.lastIndexOf('|') - 1);
				if (users.contains(ID))write(QString("repeat").toUtf8());
				else
				{
					users[ID] = auth;
					write(res.toUtf8());
				}
			}
			else write(res.toUtf8());
		}
	};
	if (canReadLine())while (canReadLine())out();
	else out();
}