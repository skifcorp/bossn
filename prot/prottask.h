#ifndef PROTTASK_H
#define PROTTASK_H

#include <QVariant>
#include <QSqlDatabase>
#include <QTimer>


#include "basetask.h"
#include "tags.h"
#include "async_func.h"
#include "protdb.h"
#include "coroutine.h"

#include <QAtomicInt>

class ProtTask : public BaseTask
{
    Q_OBJECT
public:
    ProtTask(Tags & t):BaseTask("ProtTask"),tags(t),
                       async_func_(database, *this),
                       config_async_func_( config_database, *this ),
                       viewer_prot_initialized(false),
                       prot_conf_initialized(false),
                        prot_work_initialized(false),
                        message_logs_initialized(false),
                        cur_prot_work (new prot_work), init(true)
    {
        //async_func_.setShowDebugInfo(true);
    }

    ~ProtTask()  {}

    virtual void setSettings(const QVariantMap &);

    static ProtTask * create (Tags& t, const QVariantMap& app_settings)
    {
        Q_UNUSED(app_settings);

        return new ProtTask(t);
    }



    Q_INVOKABLE QVariant addLogMessage(const QString&, AlhoSequence*, QGenericArgument sender_id, QGenericArgument type, QGenericArgument text );
    //Q_INVOKABLE void addLogMessage(const QString&, const QVariant& sender_id, const QVariant& type, const QVariant& text );
    void addLogMessageP( int sender_id, int type, const QString& text );

    virtual bool busy() const ;
protected:
    virtual void run();
private slots:
    void onSaveTimer();
private:
    Tags & tags;
    async_func async_func_;
    async_func config_async_func_;

    static BossnFactoryRegistrator<ProtTask> registrator;

    struct TagProtConf : public prot_conf
    {
        enum DzType {DzNone = 0, DzPerc = 1, DzAbs = 2};

        QString func_name;
        QVariant dz;
        QVariant min;
        QVariant max;
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
    TagValues  last_values;

    QSqlDatabase database;
    QSqlDatabase config_database;

    void initConfigForProtViewer(const QString&, const QString& ) throw(MainSequenceException);
    void insertProtConf() throw (MainSequenceException);
    void insertDbNames(const QString&, const QString& ) throw (MainSequenceException);
    void initProtDataTables()  throw (MainSequenceException);

    void tryInitializeProtViewerConf(const QString&, const QString&);
    std::function <void ()> tryInitializeProtViewerConf_;
    void tryInitializeProtDataTables();
    void initializeProtWork();
    void initializeMessageLogs();

    QTimer save_timer;

    void initTagsValues();
    void clearDataInTagsValues();

    QAtomicInt saving_now;

    bool viewer_prot_initialized;
    bool prot_conf_initialized;
    bool prot_work_initialized;
    bool message_logs_initialized;

    qx::dao::ptr<prot_work> cur_prot_work;

    QList<message_log> message_logs;
    bool init;

    void exec();
    bool is_busy;
};

#endif // PROTTASK_H
