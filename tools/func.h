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

inline void printOnDisplay(const QString& str)
{
    //QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    //qDebug( codec->fromUnicode(str) );

    ///qDebug () << str;
    QTextStream s(stdout);
    s.setCodec("IBM866");
    s<<str;
}

inline void printByteArray(const QByteArray& ba)
{
    cout << std::hex;

    for (int i = 0; i< ba.count(); ++i) {
        cout << (uint)(uchar)ba[i]<<" ";
    }
    cout << "\n"<<std::dec;
}


inline QString byteArrayToString(const QByteArray& ba)
{
    QString ret;
    for (int i = 0; i< ba.count(); ++i) {
        ret += QString::number(static_cast<uint>(static_cast<uchar>(ba[i])), 16);
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


#endif // FUNC_H
