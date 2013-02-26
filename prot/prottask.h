#ifndef PROTTASK_H
#define PROTTASK_H

#include <QVariant>
#include <QSqlDatabase>
#include <QTimer>

#include <vector>

using std::vector;

#include "basetask.h"
#include "tags.h"
#include "async_func.h"
#include "protdb.h"
#include "coroutine2.h"

#include <QAtomicInt>

class ProtTask : public BaseTask
{
    Q_OBJECT
public:
    ProtTask(Tags & t):BaseTask("ProtTask"),tags(t)
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
    static BossnFactoryRegistrator<ProtTask> registrator;
    struct TagProtConf
    {
        enum class DzType {DzNone, DzPerc, DzAbs};
        QString tag_name;
        QString func_name;
        QVariant dz;
        QVariant min;
        QVariant max;

        DzType dz_type = DzType::DzNone;
    };
    using TagProtConfs = vector<TagProtConf> ;

    using prot_value_type = typename boost::rdb::mysql::table_result_set<prot_values_table>::type::value_type;
    using TagValues  = vector<prot_value_type>;
    using TagsValues = vector<TagValues>;

    using message_log_type = typename boost::rdb::mysql::table_result_set<message_log_table>::type::value_type;
private:
    void exec();

    void initProtDataTables() ;
    void initializeProtWork();
    void initializeMessageLogs();

    void initTagsValues();
    void clearDataInTagsValues();

    bool is_busy = false;

    boost::rdb::mysql::mysql_database database;

    Tags & tags;

    TagProtConfs tag_prot_confs;
    TagsValues tags_values;
    TagValues  last_values;

    vector<message_log_table> message_logs;

    QTimer save_timer;
    QAtomicInt saving_now{0};
private:
    prot_work_table  prot_work {"prot_work"};
    prot_values_table prot_values;
    message_log_table message_log{"message_log"};
};

#endif // PROTTASK_H
