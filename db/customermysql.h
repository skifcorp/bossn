#ifndef CUSTOMERMYSQL_H
#define CUSTOMERMYSQL_H

#include <QSqlDatabase>

#include "customerdata.h"
#include "factory.h"

class CustomerMysql : public CustomerData
{
    Q_OBJECT
public:
    ~CustomerMysql(){}

    static CustomerData * create(const QVariantMap& m)
    {
        return new CustomerMysql(m);
    }

    //Q_INVOKABLE QVariant open();
    Q_INVOKABLE QVariant isCarBlocked(const QVariant& );
protected:
    CustomerMysql(const QVariantMap& );
private:
    static BossnFactoryRegistrator<CustomerMysql> registrator;

    QSqlDatabase database;
};

#endif // CUSTOMERMYSQL_H
