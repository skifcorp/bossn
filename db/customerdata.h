#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QVariant>

#include "factory.h"


class CustomerData :public QObject,
                    public BossnFactory<CustomerData, const QVariantMap>
{
    Q_OBJECT
public:
    CustomerData() {}
    virtual ~CustomerData() {}
private:

};
#endif // CUSTOMER_H
