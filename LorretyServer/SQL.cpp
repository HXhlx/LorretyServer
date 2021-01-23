#include "SQL.h"

SQL::SQL()
{
	DB = QSqlDatabase::addDatabase("QMYSQL");
	DB.setDatabaseName("lorrety");
	DB.setHostName("cdb-geebna1a.cd.tencentcdb.com");
	DB.setPort(10053);
	DB.setUserName("HXhlx");
	DB.setPassword("HXhlx19990627");
	open();
}

SQL::SQL(QObject* parent)
	: QObject(parent)
{
	new(this)SQL;
}

SQL::~SQL()
{}

long SQL::Operate(QString sqlstr)
{
	QSqlQuery query;
	query.exec(sqlstr);
	qDebug() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << query.numRowsAffected();
	return query.numRowsAffected();
}

QString SQL::Select(QString sqlstr)
{
	QSqlQuery query;
	query.exec(sqlstr);
	qDebug() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << query.size();
	if (query.size() == 0)return "";
	QString results = "";
	while (query.next())
	{
		if (sqlstr.contains("Lorretys", Qt::CaseInsensitive))results += "l&";
		else if (sqlstr.contains("DoubleChromosphere", Qt::CaseInsensitive))results += "d&";
		else if (sqlstr.contains("users", Qt::CaseInsensitive))results += "u&";
		QSqlRecord rec = query.record();
		for (size_t i = 0; i < rec.count(); ++i)results += rec.value(i).toString() + "|";
		results += "\n";
	}
	return results;
}

void SQL::open()
{
	if (!DB.open())
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-M-d h:m:s ddd") << DB.lastError().text();
		DB.close();
		throw QException();
	}
}

void SQL::close()
{
	DB.close();
}
