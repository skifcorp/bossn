#ifndef FUNC_H
#define FUNC_H

#include <QString>
#include <QDebug>
#include <QTextCodec>

inline void printOnDisplay(const QString& str)
{
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    qDebug( codec->fromUnicode(str) );
    //qDebug () << str;
}

#endif // FUNC_H
