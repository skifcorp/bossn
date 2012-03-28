#ifndef SETTINGSTOOL_H
#define SETTINGSTOOL_H

template <class T>
bool get_setting(const QString& n, const QMap<QString, QVariant>& s, T& val, bool msg = true)
{
    auto iter = s.find(n);
    if (iter == s.end()) {
        if (msg)
            qWarning()<<"get_setting: cant find "<<n;
        return false;
    }
    val = iter->value<T>();
    return true;
}

#endif // SETTINGS_H
