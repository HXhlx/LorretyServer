#pragma once
#pragma execution_character_set("utf-8")
#include <QtSql>

class SQL : public QObject
{
	Q_OBJECT
private:
	QSqlDatabase DB;
public:
	SQL();
	SQL(QObject* parent);
	~SQL();
	long Operate(QString sqlstr);
	QString Select(QString sqlstr);
	void open();
	void close();
};
