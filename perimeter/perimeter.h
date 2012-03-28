#ifndef PERIMETER_H
#define PERIMETER_H

#include <QDebug>
#include <QMap>
#include <QSharedPointer>

#include "basetask.h"
#include "tags.h"
#include "factory.h"
#include "perimeteralhos.h"


class PerimeterTask : public BaseTask
{
    Q_OBJECT
public:
    typedef QSharedPointer<PerimeterTask> Pointer;

    PerimeterTask(Tags & t );
    ~PerimeterTask() {}
    virtual void exec();
    void setSettings ( const QMap<QString, QVariant>& );
signals:
    void appeared();
    void disappeared();
private:
    Tags & tags;
    PerimeterControl::Pointer perim;
    //QString type;


};

#endif
