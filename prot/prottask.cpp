//#include <winsock.h>
//#include "mysql.h"

#include "prottask.h"
#include "settingstool.h"
#include "protdb.h"
#include "async_func.h"


#include <cmath>

BossnFactoryRegistrator<ProtTask> ProtTask::registrator("ProtTask");

bool ProtTask::busy() const
{
    return is_busy;
}

void ProtTask::setSettings(const QVariantMap & s)
{
    save_timer.setInterval( get_setting<int>("save_interval", s) );
    connect(&save_timer, SIGNAL(timeout()), this, SLOT(onSaveTimer()));

    QVariantList tgs = get_setting<QVariantList>("tags", s);
    for( QVariant tg_ : tgs) {
        QVariantMap tg = tg_.toMap();
        TagProtConf tag_prot_conf;

        tag_prot_conf.NameVar  = get_setting<QString>("tag", tg);
        tag_prot_conf.Names    = get_setting<QString>("tag_ru", tg);
        tag_prot_conf.NameDB   = get_setting<QString>("database_name", s);
        tag_prot_conf.func_name = get_setting<QString>("func", tg);
        tag_prot_conf.dz        = get_setting<QVariant>("dz", tg, QVariant(0));
        tag_prot_conf.min        = get_setting<QVariant>("min", tg, QVariant(0));
        tag_prot_conf.max        = get_setting<QVariant>("max", tg, QVariant(0));

        QString dz_type = get_setting<QString>("dz_type", tg, QString());
        if (dz_type == "absolute")
            tag_prot_conf.dz_type = TagProtConf::DzAbs;
        else if ( dz_type == "perc" ) {
            tag_prot_conf.dz_type = TagProtConf::DzPerc;
        }

        tag_prot_confs.push_back(tag_prot_conf);
    }

    initTagsValues();


    config_database = QSqlDatabase::addDatabase(get_setting<QString>("database_driver", s), get_setting<QString>("connection_name", s) + "_conf");
    config_database.setHostName(get_setting<QString>("database_host", s));
    config_database.setDatabaseName(get_setting<QString>("config_database", s));
    config_database.setUserName(get_setting<QString>("database_user", s));
    config_database.setPassword(get_setting<QString>("database_password", s));
    config_database.setConnectOptions( get_setting<QString>("connection_options", s, QString() ) );

    /*if  ( config_database.open() ) {
        qDebug() << "OPENED!";
    }
    else {
        qDebug() << "cant open!!!: "<<config_database.lastError().text();
    }*/

    //qFatal( "exit;" );
    //MYSQL * m = *static_cast<MYSQL**>( config_database.driver()->handle().data() );
    //mysql_set_character_set(m, "cp1251");

    //tryInitializeProtViewerConf(get_setting<QString>("database_name", s),
    //                                    get_setting<QString>("database_ru_name", s));
    QString db_name = get_setting<QString>("database_name", s);
    QString db_ru_name = get_setting<QString>("database_ru_name", s);
    tryInitializeProtViewerConf_ = [db_name, db_ru_name, this] { tryInitializeProtViewerConf(db_name, db_ru_name); };

    database = QSqlDatabase::addDatabase(get_setting<QString>("database_driver", s), get_setting<QString>("connection_name", s));
    database.setHostName(get_setting<QString>("database_host", s));
    database.setDatabaseName(get_setting<QString>("database_name", s));
    database.setUserName(get_setting<QString>("database_user", s));
    database.setPassword(get_setting<QString>("database_password", s));
    database.setConnectOptions( get_setting<QString>("connection_options", s, QString() ) );

    //database.open();
    //m = *static_cast<MYSQL**>( database.driver()->handle().data() );
    //mysql_set_character_set(m, "cp1251");

    //tryInitializeProtDataTables();

    //save_timer.start();


    createStack(65535*4);
    cont();

    //qDebug() << "prot set settings!!!!!!!!!!!!!!";
}

void ProtTask::tryInitializeProtViewerConf(const QString& db_name, const QString& db_ru_name)
{
    try {
        initConfigForProtViewer(db_name, db_ru_name);

        config_database.close();
        viewer_prot_initialized = true;
    }
    catch(MainSequenceException& ex){
        qWarning() << "initConfigForProtViewer error!!: db_text: " << ex.systemMessage();
        //tryInitializeProtViewerConf_ = [db_name, db_ru_name, this] { tryInitializeProtViewerConf(db_name, db_ru_name); };
    }

}

void ProtTask::tryInitializeProtDataTables()
{
    try {
        initProtDataTables();
        //qDebug() << "AFTER INITPROTDATA_TABLSE!!!!!!!!!!!!!";
        prot_conf_initialized = true;
    }
    catch (  MainSequenceException& ex ) {
        qWarning() << "cant initProtDatatable! " << ex.systemMessage();
    }


}


void ProtTask::initProtDataTables()  throw (MainSequenceException)
{
#if 0
    for (TagProtConf & tpc : tag_prot_confs) {
        //try {
        //qDebug() << "creating table: " << tpc.NameVar;

        wrap_async_ex( QString(), QString(), [this, &tpc]{return async_func_.async_create_table<prot_values>(tpc.NameVar);});
        //qDebug() << "creating table: " << tpc.NameVar << " OK!!!!!!!!!!!!!!!";

        wrap_async_ex( QString(), QString(), [this, &tpc]{return async_func_.async_create_table<prot_value_scale>("Scale_" + tpc.NameVar);});

            prot_value_scale sc(tpc.min.toFloat(), tpc.max.toFloat());

            wrap_async_ex( QString(), QString(), [this, &tpc, &sc]{return async_func_.async_insert(sc, true, "Scale_" + tpc.NameVar);});
        //}
        //catch (MainSequenceException& ex)  {
            //qWarning() << "cant initProtDatatable! " << ex.systemMessage();
        //}
    }

#endif
}

void ProtTask::insertProtConf() throw (MainSequenceException)
{
#if 0
    wrap_async_ex(QString(), QString(), [this]{return config_async_func_.async_delete_all<prot_conf>();} );
    QList<prot_conf> prot_confs;

    for (TagProtConf & tpc : tag_prot_confs) {
        prot_confs.push_back(tpc);
    }

    //config_async_func_.async_insert(prot_confs);
    wrap_async_ex(QString(), QString(), [this, &prot_confs]{config_async_func_.async_insert(prot_confs);});
#endif

}

void ProtTask::insertDbNames(const QString& db_name, const QString& db_ru_name) throw (MainSequenceException)
{
    //qDebug() << "before delete all";
#if 0
    wrap_async_ex(QString(), QString(), [this]{return config_async_func_.async_delete_all<db_names>();} );

    //qDebug() << "after";

    db_names dn;
    dn.DB_name   = db_name;
    dn.DB_r_name = db_ru_name;

    //config_async_func_.async_insert(dn);
    wrap_async_ex(QString(), QString(), [this, &dn]{config_async_func_.async_insert(dn);});
#endif

}


void ProtTask::initConfigForProtViewer(const QString& db_name, const QString& db_ru_name) throw (MainSequenceException)
{
    //try {

        insertDbNames(db_name, db_ru_name);

        insertProtConf();
    //}
    //catch(MainSequenceException& ex){
       // qWarning() << "initConfigForProtViewer error!!: db_text: " << ex.systemMessage();


    //}


}

void ProtTask::initializeProtWork()
{
#if 0
    cur_prot_work->start_from = cur_prot_work->work_till = QDateTime::currentDateTime().toUTC();

    try {
        wrap_async_ex( QString(), QString(), [this]{return async_func_.async_create_table<prot_work>();});
        wrap_async_ex( QString(), QString(), [this]{return async_func_.async_insert(cur_prot_work, true); });

        prot_work_initialized = true;
    }
    catch(MainSequenceException& ex) {
        qWarning() << "cant initProtWORK!!!! " << ex.systemMessage();
    }
#endif
}


void ProtTask::initializeMessageLogs()
{
#if 0
    try {
        wrap_async_ex( QString(), QString(), [this]{return async_func_.async_create_table<message_log>();});

        message_logs_initialized = true;
    }
    catch(MainSequenceException& ex) {
        qWarning() << "cant initProtWORK!!!! " << ex.systemMessage();
    }
#endif
}

void ProtTask::run()
{  
    if (init) {
        is_busy = true;

        while (!viewer_prot_initialized) {
            //qDebug() << "cycle!!!!";
            tryInitializeProtViewerConf_();
        }

        while (!prot_conf_initialized)
            tryInitializeProtDataTables();

        while (!prot_work_initialized)
            initializeProtWork();

        while (!message_logs_initialized)
            initializeMessageLogs();


        save_timer.start();
        init = false;

        qDebug() << "PROT INITIALIZED!";

        is_busy = false;

        return;
    }

    exec();
}

void ProtTask::exec()
{    
    if (!viewer_prot_initialized || !prot_conf_initialized || !prot_work_initialized || !message_logs_initialized) return;


     is_busy = true;


    TagsValues::Iterator iter            = tags_values.begin();
    TagValues::Iterator  last_value_iter = last_values.begin();

    for (const TagProtConf & tpc : tag_prot_confs) {
        //qDebug () <<  "exec: " << tpc.NameVar;

        QVariant val = tags[ tpc.NameVar ]->func( tpc.func_name, this );
        if ( !val.isValid() ) {
            if ( last_value_iter->value == last_value_iter->value /*not nan*/ ) {
                *last_value_iter = prot_values{ QDateTime::currentDateTime().toUTC(), NAN };
                iter->push_back( *last_value_iter );
            }
        }
        else {

            float fval = val.toFloat();



            if ( /* true || */
                 ( last_value_iter->value != last_value_iter->value /*isnan*/) ||
                 ( tpc.dz_type == TagProtConf::DzNone && !qFuzzyCompare(fval, last_value_iter->value) ) ||
                 ( tpc.dz_type == TagProtConf::DzAbs && qAbs(fval - last_value_iter->value) > tpc.dz.toFloat() ) ) {
                //

                //qDebug () << "IN DEADZONE!!!" << QDateTime::currentDateTime().toUTC();
                *last_value_iter = prot_values{ QDateTime::currentDateTime().toUTC(), val.toFloat() };

                iter->push_back( *last_value_iter );
            }
        }

        ++iter; ++last_value_iter;
    }

    //qDebug() << "!!!!!!!EXECUTING PROT!!!!!!!";

    is_busy = false;
}

void ProtTask::initTagsValues()
{
    //for (const TagProtConf & tpc : tag_prot_confs) {
    for (int i = 0; i<tag_prot_confs.count(); ++i) {
        tags_values.push_back(TagValues());
        last_values.push_back(prot_values{QDateTime::currentDateTimeUtc(), NAN});
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
    //qDebug () << "want save!!!";

    if (!prot_conf_initialized) {
        qWarning() << "SAVING OPERATION NOT STARTED BECAUSE prot_conf not initialied!. Will try next time.... point count: " << tags_values.first().count();
        return;
    }

    if ( static_cast<int>(saving_now) > 0 ) {
        qWarning() << "SAVING OPERATION NOT BECAUSE PREVIOUS NOT FINISHED. Will try next time.... point count: " << tags_values.first().count();
        return;
    }

    QList<QString> names;
    for (const TagProtConf & tpc : tag_prot_confs) {
        names.push_back(tpc.NameVar);
    }

#if 0

    QtConcurrent::run( [this,  names]() mutable {
        saving_now.ref();
        cur_prot_work->work_till = QDateTime::currentDateTime().toUTC();
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
        }

        saving_now.deref();
        //qDebug() << "saved!";
    } );
#endif

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
    message_logs.push_back(message_log{sender_id, type, text});
}
