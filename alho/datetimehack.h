#ifndef DATETIMEHACK_H
#define DATETIMEHACK_H

#include <QDateTime>

inline const QDateTime& zero_date_time()
{
    static QDateTime d( QDate (2000, 01, 01) );
    return d;
}

#endif // DATETIMEHACK_H
