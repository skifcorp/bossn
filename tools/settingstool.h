#ifndef SETTINGSTOOL_H
#define SETTINGSTOOL_H

#include <QtGlobal>
//#include <Maybe.h>


template <class T>
T get_setting(const QString& n, const QMap<QString, QVariant>& s )
{
    auto iter = s.find(n);
    if (iter == s.end()) {
        qFatal( qPrintable ("get_setting: cant find " + n) );
    }

    return iter->value<T>();
}

template <class T>
T get_setting(const QString& n, const QMap<QString, QVariant>& s, const T& def, bool msg = false )
{
    auto iter = s.find(n);
    if (iter == s.end()) {
        if ( msg ) {
            qWarning() << "get_setting: cant find " << n;
        }
        return def;
    }

    return iter->value<T>();
}

#endif // SETTINGS_H
