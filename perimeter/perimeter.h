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
    //virtual void exec();
    virtual void setSettings ( const QVariantMap& );
    static BaseTask * create(Tags& t, const QVariantMap& )
    {
        return new PerimeterTask(t);
    }
    virtual bool busy() const;
protected:
    virtual void run();
private:
    Tags & tags;
    PerimeterControl::Pointer perim;
    //QString type;
    QString appeared_tag_name;
    QString appeared_tag_func;
    QString disappeared_tag_name;
    QString disappeared_tag_func;

    bool is_busy;

    static BossnFactoryRegistrator<PerimeterTask> registrator;
};

#endif
