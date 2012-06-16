#ifndef PROTTASK_H
#define PROTTASK_H

#include <QVariant>
#include <QSqlDatabase>
#include <QTimer>


#include "basetask.h"
#include "tags.h"
#include "async_func.h"
#include "protdb.h"

class ProtTask : public BaseTask
{
    Q_OBJECT
public:
    ProtTask(Tags & t):tags(t), async_func_(database)
    {
        //qDebug( ) << "prot!!!! ";
    }
    ~ProtTask()  {}

    virtual void setSettings(const QVariantMap &);

    static ProtTask * create (Tags& t, const QVariantMap& app_settings)
    {
        Q_UNUSED(app_settings);

        return new ProtTask(t);
    }
    virtual void exec();
private slots:
    void onSaveTimer();
private:
    Tags & tags;
    async_func async_func_;

    static BossnFactoryRegistrator<ProtTask> registrator;

    struct TagProtConf : public prot_conf
    {
        enum DzType {DzNone = 0, DzPerc = 1, DzAbs = 2};

        QString func_name;
        QVariant dz;
        int dz_type;
        TagProtConf(  ) : prot_conf(), dz_type(DzNone)
        {
        }
    };

    typedef QList<TagProtConf> TagProtConfs;
    TagProtConfs tag_prot_confs;

    typedef QList<prot_values>  TagValues;
    typedef QList<TagValues>    TagsValues;

    TagsValues tags_values;

    QSqlDatabase database;

    void initConfigForProtViewer(const QVariantMap& );
    void insertProtConf() throw (MainSequenceException);
    void insertDbNames(const QString&, const QString& ) throw (MainSequenceException);

    void initProtDataTables();

    QTimer save_timer;

    void initTagsValues();
    void clearDataInTagsValues();
};

#endif // PROTTASK_H
