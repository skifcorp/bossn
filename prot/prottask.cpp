//#include <winsock.h>
//#include "mysql.h"

#include "prottask.h"
#include "settingstool.h"
#include "protdb.h"
#include "async_func.h"
#include "func.h"

#include <cmath>
#include <limits>

namespace mysql = boost::rdb::mysql;

BossnFactoryRegistrator<ProtTask> ProtTask::registrator("ProtTask");

bool ProtTask::busy() const
{
    return is_busy;
}

void ProtTask::setSettings(const QVariantMap & s)
{    
    const QVariantList& tgs = get_setting<QVariantList>("tags", s);

    for( QVariant tg_ : tgs) {
        QVariantMap tg = tg_.toMap();
        TagProtConf tag_prot_conf;

        tag_prot_conf.tag_name   = get_setting<QString>("tag", tg);
        //tag_prot_conf.Names     = get_setting<QString>("tag_ru", tg);
       // tag_prot_conf.NameDB    = get_setting<QString>("database_name", s);
        tag_prot_conf.func_name = get_setting<QString>("func", tg);
        tag_prot_conf.dz        = get_setting<QVariant>("dz", tg, QVariant(0));
        tag_prot_conf.min       = get_setting<QVariant>("min", tg, QVariant(0));
        tag_prot_conf.max       = get_setting<QVariant>("max", tg, QVariant(0));

        QString dz_type = get_setting<QString>("dz_type", tg, QString());
        if (dz_type == "absolute")
            tag_prot_conf.dz_type = TagProtConf::DzType::DzAbs;
        else if ( dz_type == "perc" ) {
            tag_prot_conf.dz_type = TagProtConf::DzType::DzPerc;
        }

        tag_prot_confs.push_back(tag_prot_conf);
    }

    initTagsValues();

    database_name = get_setting<QString>("database_name", s);

    database.setHost(get_setting<QString>("database_host", s).toStdString() );
    database.setUser(get_setting<QString>("database_user", s).toStdString());
    database.setPassword(get_setting<QString>("database_password", s).toStdString());

    // database.setConnectOptions( get_setting<QString>("connection_options", s, QString() ) );

    save_timer.setInterval( get_setting<int>("save_interval", s) );
    connect(&save_timer, SIGNAL(timeout()), this, SLOT(onSaveTimer()));
    save_timer.start();

}





void ProtTask::run()
{  
    exec();
}

//boost::posix_time::from_time_t( QDateTime::currentDateTime().toTime_t() );

void ProtTask::exec()
{    
    is_busy = true;

    qDebug()  << "prot_exec!";

    auto iter            = tags_values.begin();
    auto last_value_iter = last_values.begin();

    for (const TagProtConf & tpc : tag_prot_confs) {
        QVariant val = tags[ tpc.tag_name ]->func( tpc.func_name, this );

        if ( !val.isValid() ) {
#if 0
            if ( last_value_iter->value == last_value_iter->value /*not nan*/ ) {
                *last_value_iter = prot_values{ QDateTime::currentDateTime().toUTC(), std::numeric_limits<float>::quiet_NaN() };
                iter->push_back( *last_value_iter );
            }
#endif
            if (  (*last_value_iter)[prot_values.value] == (*last_value_iter)[prot_values.value] /*not nan*/  ) {
                (*last_value_iter)[prot_values.time] = qt_to_ptime(QDateTime::currentDateTime().toUTC());
                (*last_value_iter)[prot_values.value] = std::numeric_limits<double>::quiet_NaN();
            }
        }
        else {
            auto fval = val.toDouble();

            if ( /* true || */
                 ( (*last_value_iter)[prot_values.value] != (*last_value_iter)[prot_values.value] /*isnan*/) ||
                 ( tpc.dz_type == TagProtConf::DzType::DzNone && !qFuzzyCompare(fval, (*last_value_iter)[prot_values.value]) ) ||
                 ( tpc.dz_type == TagProtConf::DzType::DzAbs && qAbs(fval - (*last_value_iter)[prot_values.value]) > tpc.dz.toDouble() ) ) {

                (*last_value_iter)[prot_values.time] = qt_to_ptime(QDateTime::currentDateTime().toUTC());
                (*last_value_iter)[prot_values.value] = val.toDouble();

                (*iter).push_back( *last_value_iter );
            }
        }

        ++iter; ++last_value_iter;
    }

    is_busy = false;
}

void ProtTask::initTagsValues()
{
    for (decltype(tag_prot_confs)::size_type i = 0; i<tag_prot_confs.size(); ++i) {
        tags_values.emplace_back();
        last_values.emplace_back( qt_to_ptime( QDateTime::currentDateTimeUtc() ), std::numeric_limits<double>::quiet_NaN());
    }

}

void ProtTask::clearDataInTagsValues()
{
    for ( TagValues & tv : tags_values ) {
        tv.clear();
    }
}


void ProtTask::onSaveTimer()
{   
    qDebug () << "want save!!!";

#if 0
    if (!prot_conf_initialized) {
        qWarning() << "SAVING OPERATION NOT STARTED BECAUSE prot_conf not initialied!. Will try next time.... point count: " << tags_values.first().count();
        return;
    }
#endif


    if ( static_cast<int>(saving_now) > 0 ) {
        qWarning() << "SAVING OPERATION NOT BECAUSE PREVIOUS NOT FINISHED. Will try next time.... point count: " << tags_values.front().size();
        return;
    }

    vector<QString> names;
    for (const TagProtConf & tpc : tag_prot_confs) {
        names.push_back(tpc.tag_name);
    }



    QtConcurrent::run( [this,  names]() mutable {
        saving_now.ref();
        try {
            if (database.isClosed())
                database.open();

            static bool exec_once = [&, this] {
                database.execute( mysql::create_database(database_name.toStdString()) );

                database.setDatabase( database_name.toStdString() );

                database.execute( mysql::create_table(prot_work) );

                database.execute( mysql::insert_into(prot_work)
                                  (prot_work.start_from, prot_work.work_till)
                                  .values( when_started, boost::posix_time::second_clock::universal_time() ) );

                for ( const TagProtConf & c : tag_prot_confs ) {
                    database.execute(  mysql::create_table( prot_values_table{c.tag_name.toStdString()} ) ) ;
                }

                return true;
            }(); Q_UNUSED(exec_once);

        }
        catch ( ::sql::SQLException & ex ) {
            qWarning() << "Sql exception on saving: " << ex.what() << " state: " << ex.getSQLState().c_str();
        }
        catch( std::exception& ex ) {
            qWarning() << "Exception on saving: " << ex.what();
        }
        catch( ... ) {
            qWarning() << "Exception on saving: ";
        }

        /*cur_prot_work->work_till = QDateTime::currentDateTime().toUTC();
        QSqlError err2 = qx::dao::update_optimized(cur_prot_work, &database, "prot_work");
        if ( err2.isValid() ) {
            qWarning() << "prot_error while updating work_till! "<< err2.databaseText() << " " << err2.driverText();
        }


        QList<QString>::const_iterator iter = names.begin();

        for (TagsValues::iterator values_iter = tags_values.begin(); values_iter != tags_values.end(); ++values_iter  ) {
            if (!values_iter->isEmpty()) {
                QSqlError err = qx::dao::insert(*values_iter, &database, *iter, true);
                if ( err.isValid() ) {
                    qWarning() << "prot_error while saving data! "<< err.databaseText() << " " << err.driverText();
                }
            }

            ++iter;
        }


        QSqlError err3 = qx::dao::insert(message_logs, &database);
        if ( err3.isValid() ) {
            qWarning() << "prot_error while inserting message_logs! "<< err3.databaseText() << " " << err3.driverText();
        }*/



        saving_now.deref();
    } );


    //tags_values.clear();
    clearDataInTagsValues();
    message_logs.clear();
}


QVariant ProtTask::addLogMessage(const QString&, AlhoSequence*, QGenericArgument sender_id, QGenericArgument type, QGenericArgument text )
//void ProtTask::addLogMessage(const QString&, const QVariant& sender_id, const QVariant& type, const QVariant& text )
{
/*    if ( sender_id.name() != QString("QVariant")) {
        qWarning() << "bad type for sender_id: (" << sender_id.name()<< ")"; return QVariant();
    }

    if (type.name() != QString("QVariant")) {
        qWarning() << "bad type for type (" << sender_id.name()<<")"; return  QVariant();
    }

    if (text.name() != QString("QVariant")) {
        qWarning() << "bad type for text (" << sender_id.name()<<")"; return  QVariant();
    }*/

    addLogMessageP( reinterpret_cast<QVariant*>(sender_id.data())->toInt(),
                   reinterpret_cast<QVariant*>(type.data())->toInt(),
                   reinterpret_cast<QVariant*>(text.data())->toString() );

//    addLogMessageP( sender_id.toInt(),
//                   type.toInt(),
//                   text.toString() );
    return QVariant();

}


void ProtTask::addLogMessageP( int sender_id, int type, const QString& text )
{
    //message_logs.push_back( message_log{sender_id, type, text} );
}
