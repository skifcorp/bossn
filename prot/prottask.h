#ifndef PROTTASK_H
#define PROTTASK_H

#include <QVariant>
#include <QSqlDatabase>

#include "basetask.h"
#include "tags.h"


class ProtTask : public BaseTask
{
public:
    ProtTask(Tags & t):tags(t)
    {
        qDebug( ) << "prot!!!! ";
    }
    ~ProtTask()  {}

    virtual void setSettings(const QVariantMap &);

    static ProtTask * create (Tags& t, const QVariantMap& app_settings)
    {
        Q_UNUSED(app_settings);

        return new ProtTask(t);
    }
    virtual void exec();
private:
    Tags & tags;

    static BossnFactoryRegistrator<ProtTask> registrator;

    struct TagProtConf
    {
        enum DzType {DzNone = 0, DzPerc = 1, DzAbs = 2};

        QString tag_name;
        QString func_name;
        QVariant dz;
        int dz_type;
        TagProtConf(  ) : dz_type(DzNone)
        {
        }
    };

    typedef QList<TagProtConf> TagProtConfs;
    TagProtConfs tag_prot_confs;

    typedef QList<QVariant>  TagValues;
    typedef QList<TagValues> TagsValues;

    TagsValues tags_values;

    QSqlDatabase database;
};

#endif // PROTTASK_H
