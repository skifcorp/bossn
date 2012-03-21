#ifndef TOOLS_H
#define TOOLS_H

void makeDataBaseC();
void makeDataBaseS();
void formatCard();


#include <QObject>
#include <functional>

using std::function;

/*class connect_function_helper : public QObject
{
public:
    connect_function_helper(QObject * parent, const function<void ()>& f):QObject(parent), func(f)
    {

    }
public slots:
    void
private:
    function<void ()> func;
};*/



#endif // TOOLS_H
