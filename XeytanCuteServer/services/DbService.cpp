#include "DbService.h"



DbService::DbService()
{
}


DbService::~DbService()
{
}

void DbService::init()
{
	/*
	if (!QSqlDatabase::drivers().contains("QSQLITE"))
		exit(0);

	QSqlDatabase db;
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("D:/workspace/Qt/BackGui/clients.db");

	if (db.open())
	{
		QSqlQuery query;
		string address = c->address;
		QString queryString = QString("select * from users");/* WHERE pc_name = '");
		queryString += c->getPcName()->c_str() + QString("' AND ip='") + address.c_str() + "'";*/
	/*
		while (true)
		{
	
	//		Take the country from sqlite DB but , if not set yet by ClientThread::CreateWindow , we sleep 

			bool executed = query.exec(queryString);
			if (!executed)
				cout << query.lastError().text().toStdString() << endl;
			if (query.exec())
			{
				QSqlRecord record = query.record();
				int count;
				if (db.driver()->hasFeature(QSqlDriver::QuerySize))
					count = query.size();
				else
					count = 1;
				query.next();
				QVariant temp = query.value(record.indexOf("country"));
				if (temp.isValid())
				{
					c->country = query.value(record.indexOf("country")).toString();
					break;
				}
			}
			boost::this_thread::sleep(boost::posix_time::milliseconds(500));
		}
	}
	else
	{
		qDebug(db.lastError().text().toLatin1());
	}
	db.close();
	*/
}
