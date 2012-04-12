#ifndef FUNC_H
#define FUNC_H

#include <QString>
#include <QDebug>
#include <QTextCodec>
#include <QCoreApplication>
#include <QTime>

#include <iostream>

using std::cout;

inline void printOnDisplay(const QString& str)
{
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    qDebug( codec->fromUnicode(str) );
    //qDebug () << str;
}

inline void printByteArray(const QByteArray& ba)
{
    cout << std::hex;

    for (int i = 0; i< ba.count(); ++i) {
        cout << (uint)(uchar)ba[i]<<" ";
    }
    cout << "\n"<<std::dec;
}

inline void sleepnb(int msec)
{
    QTime tm;
    tm.start();

    while (tm.elapsed() < msec) qApp->processEvents();
}

#endif // FUNC_H
