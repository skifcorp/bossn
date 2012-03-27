#ifndef PERIMETER_H
#define PERIMETER_H

#include <QDebug>

#include "task.h"
#include "tags.h"

class PerimeterTask : public Task
{
public:
    PerimeterTask(Tags & t):tags(t)
    {

    }
    virtual void exec()
    {
        //qDebug() << "getting value!!!!"<<tags["di"]->value(Q_ARG(int, 0));
        qDebug()<<"getting weight!!!"<<tags["weight1_1"]->value("weight1_1");
    }

private:
    Tags & tags;
};

#endif
