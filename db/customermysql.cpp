#include "customermysql.h"
#include "settingstool.h"
#include "QxOrm.h"

BossnFactoryRegistrator<CustomerMysql> CustomerMysql::registrator("CustomerMysql");

CustomerMysql::CustomerMysql(const QVariantMap & m)
{
/*    QString host_name       = get_setting<QString>("hostName", m);
    QString database_name   = get_setting<QString>("databaseName", m);
    QString user_name       = get_setting<QString>("userName", m);
    QString password        = get_setting<QString>("password", m);
    QString connect_options = get_setting<QString>("connectionOptions", m);

    database = QSqlDatabase::addDatabase("QMYSQL");
    if (!database.isValid()) {
        qWarning()<<" cant load QMYSQL driver!!!"; return;
    }

    database.setHostName(host_name);
    database.setDatabaseName(database_name);
    database.setUserName(user_name);
    database.setPassword(password);
    database.setConnectOptions(connect_options);

    if (!database.open()) {
        qWarning( ) << "cant open MySql database: "<<
                       "host_name:" << host_name <<
                       "database_name:"<< database_name <<
                       "user_name:" << user_name <<
                       "password:" << password <<
                       "connect_options:" << connect_options;
    }*/

    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLMYSQL");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName("test");
    qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
    qx::QxSqlDatabase::getSingleton()->setUserName("root");
    qx::QxSqlDatabase::getSingleton()->setPassword("parabelum");
}


QVariant CustomerMysql::isCarBlocked(const QVariant & car_var)
{
    ushort car = static_cast<ushort>(car_var.toUInt());

    return QVariant(false);
}

