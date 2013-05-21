#ifndef FUNC_H
#define FUNC_H

#include <QString>
#include <QDebug>
#include <QTextCodec>
#include <QCoreApplication>
#include <QTime>
#include <QTextStream>
#include <QApplication>

#include <iostream>

using std::cout;

#include <ctime>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <sstream>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include <boost/rdb/core/datetime.hpp>

const QString rdb_date_time_format = "yyyy-MM-dd hh:mm:ss";

inline void printOnDisplay(const QString& str)
{
    //QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    //qDebug( codec->fromUnicode(str) );

    //qDebug () << str;
    //QTextStream s(stdout);
    //s.setCodec("IBM866");
    //s<<str;
    //qDebug() << str;

    QTextStream out(stdout);

    out.setCodec( QTextCodec::codecForName("IBM 866") );

    out <<  str << "\n";

}

inline void printByteArray(const QByteArray& ba)
{
    cout << std::hex;

    for (int i = 0; i< ba.count(); ++i) {
        cout << (uint)(uchar)ba[i]<<" ";
    }
    cout << "\n"<<std::dec;
}


inline QString byteArrayToString(const QByteArray& ba, int base = 16, const QString& sep = " ")
{
    QString ret;
    for (int i = 0; i< ba.count(); ++i) {
        ret += QString::number(static_cast<uint>(static_cast<uchar>(ba[i])), base) + sep;
    }
    return ret;
}


inline QDateTime timeShitToDateTime(ulong timeInMinutes)
{
    QDate d(2000, 01, 01);
    QDateTime dt(d);

    ulong temp=timeInMinutes/60/24;
    int   ost=timeInMinutes-temp*60*24;

    dt=dt.addDays(temp);

    int hour=ost/60;
    int minute=ost-hour*60;

    QTime time(hour, minute);
    dt.setTime(time);
    return dt;
}

inline ulong dateTimeToTimeShit(const QDateTime& dt)
{
    QDateTime startDT(QDate (2000, 01, 01));

    if (dt<startDT) {
//qDebug () << "smaller!";
        return 0;
    }
    unsigned long temp = startDT.daysTo(dt)*24*60 + dt.time().hour()*60+dt.time().minute();
    return temp;
}

inline std::time_t toTime_t( const boost::posix_time::ptime & t )
{
    static const boost::posix_time::ptime epoch( boost::gregorian::date(1970,1,1) );

    const boost::posix_time::time_duration::sec_type x((t - epoch).total_seconds() );

    return x;
}

//boost::posix_time::from_time_t( QDateTime::currentDateTime().toTime_t() );

inline boost::posix_time::ptime qt_to_ptime( const QDateTime & dt )
{
    return boost::posix_time::from_time_t( dt.toTime_t() );
}


inline QDateTime ptime_to_qt( const boost::posix_time::ptime & t )
{
    return QDateTime::fromTime_t( toTime_t( t ) );
}

inline std::string time_duration_to_string( const boost::posix_time::time_duration& td )
{
    std::stringstream ss;
    ss<<td;
    return ss.str();
}

template <class V>
inline QVariantMap to_variant_map(const QMap<QString, V>& m)
{
    QVariantMap ret;
    for ( auto iter = m.begin(); iter != m.end(); ++iter ) {
        ret[iter.key()] = QVariant::fromValue(*iter);
    }

    return std::move(ret);
}

#endif // FUNC_H
