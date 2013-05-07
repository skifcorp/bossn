#ifndef PROTTASK_H
#define PROTTASK_H

#include <QVariant>
#include <QSqlDatabase>
#include <QTimer>

#include <vector>

using std::vector;

#include <boost/date_time/posix_time/ptime.hpp>

#include "basetask.h"
#include "tags.h"
#include "async_func.h"

#include "coroutine2.h"
#include "func.h"

#include <QAtomicInt>

class ProtTaskImpl;

class ProtTask : public BaseTask
{
    Q_OBJECT
public:
    ProtTask(Tags & t);

    ~ProtTask();

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
private:

    QSharedPointer<ProtTaskImpl> impl_;

    void exec();

    void initProtDataTables() ;
    void initializeProtWork();
    void initializeMessageLogs();

    void initTagsValues();
    void clearDataInTagsValues();

    bool is_busy = false;


    QTimer save_timer;
    QAtomicInt saving_now{0};
    boost::posix_time::ptime when_started = boost::posix_time::second_clock::universal_time();
private:
};

#endif // PROTTASK_H
