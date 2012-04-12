#include "customermysql.h"
#include "settingstool.h"

BossnFactoryRegistrator<CustomerMysql> CustomerMysql::registrator("CustomerMysql");

CustomerMysql::CustomerMysql(const QVariantMap & m)
{
    QString host_name;
    get_setting("hostName", m, host_name);

    QString database_name;
    get_setting("databaseName", m, database_name);

    QString user_name;
    get_setting("userName", m, user_name);

    QString password;
    get_setting("password", m, password);

    QString connect_options;
    get_setting("connectionOptions", m, connect_options);

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
    }
}


QVariant CustomerMysql::isCarBlocked(const QVariant & car_var)
{
    ushort car = static_cast<ushort>(car_var.toUInt());

    return QVariant(false);
}

