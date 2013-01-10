#ifndef DELETERLATER_H
#define DELETERLATER_H

#include <QObject>

template <class T>
class DeleterLater : public QObject
{
public:
    DeleterLater (T * o)   : obj_(o)
    {}

    ~DeleterLater()
    {
        delete obj_;
    }

private:
    T * obj_;
};

#endif // DELETERLATER_H
