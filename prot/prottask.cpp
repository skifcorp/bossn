#include "prottask.h"
#include "settingstool.h"
#include "protdb.h"
#include "async_func.h"

BossnFactoryRegistrator<ProtTask> ProtTask::registrator("ProtTask");

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

        QString dz_type = get_setting<QString>("dz_type", tg, QString());
        if (dz_type == "absolute")
            tag_prot_conf.dz_type = TagProtConf::DzAbs;
        else if ( dz_type == "perc" )
            tag_prot_conf.dz_type = TagProtConf::DzPerc;


        tag_prot_confs.push_back(tag_prot_conf);
    }

    initTagsValues();


    config_database = QSqlDatabase::addDatabase(get_setting<QString>("database_driver", s), get_setting<QString>("connection_name", s) + "_conf");
    config_database.setHostName(get_setting<QString>("database_host", s));
    config_database.setDatabaseName(get_setting<QString>("config_database", s));
    config_database.setUserName(get_setting<QString>("database_user", s));
    config_database.setPassword(get_setting<QString>("database_password", s));
    config_database.setConnectOptions( get_setting<QString>("connection_options", s, QString() ) );

    tryInitializeProtViewerConf(get_setting<QString>("database_name", s),
                                        get_setting<QString>("database_ru_name", s));

    database = QSqlDatabase::addDatabase(get_setting<QString>("database_driver", s), get_setting<QString>("connection_name", s));
    database.setHostName(get_setting<QString>("database_host", s));
    database.setDatabaseName(get_setting<QString>("database_name", s));
    database.setUserName(get_setting<QString>("database_user", s));
    database.setPassword(get_setting<QString>("database_password", s));
    database.setConnectOptions( get_setting<QString>("connection_options", s, QString() ) );


    tryInitializeProtDataTables();

    save_timer.start();
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
        tryInitializeProtViewerConf_ = [db_name, db_ru_name, this] { tryInitializeProtViewerConf(db_name, db_ru_name); };
    }

}

void ProtTask::tryInitializeProtDataTables()
{
    try {
        initProtDataTables();
        prot_conf_initialized = true;
    }
    catch (  MainSequenceException& ex ) {
        qWarning() << "cant initProtDatatable! " << ex.systemMessage();
    }
}


void ProtTask::initProtDataTables()  throw (MainSequenceException)
{
    for (TagProtConf & tpc : tag_prot_confs) {
        //try {
            async_func_.wrap_async_ex( QString(), QString(), [this, &tpc]{return async_func_.async_create_table<prot_values>(tpc.NameVar);});
        //}
        //catch (MainSequenceException& ex)  {
            //qWarning() << "cant initProtDatatable! " << ex.systemMessage();
        //}
    }


}

void ProtTask::insertProtConf() throw (MainSequenceException)
{
    config_async_func_.wrap_async_ex(QString(), QString(), [this]{return config_async_func_.async_delete_all<prot_conf>();} );
    QList<prot_conf> prot_confs;

    for (TagProtConf & tpc : tag_prot_confs) {
        prot_confs.push_back(tpc);
    }

    config_async_func_.async_insert(prot_confs);
}

void ProtTask::insertDbNames(const QString& db_name, const QString& db_ru_name) throw (MainSequenceException)
{
    config_async_func_.wrap_async_ex(QString(), QString(), [this]{return config_async_func_.async_delete_all<db_names>();} );

    db_names dn;
    dn.DB_name   = db_name;
    dn.DB_r_name = db_ru_name;

    config_async_func_.async_insert(dn);
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

void ProtTask::exec()
{    
    qDebug () << "exec!";

    if ( !viewer_prot_initialized )
        tryInitializeProtViewerConf_();

    if ( !prot_conf_initialized && viewer_prot_initialized ) {
        // have initialize prot_conf if viewer_prot_conf dont initialized because they are in the same host
        tryInitializeProtDataTables();
    }

    if (!prot_conf_initialized) return;

    TagsValues::Iterator iter = tags_values.begin();
    for (const TagProtConf & tpc : tag_prot_confs) {
        QVariant val = tags[ tpc.NameVar ]->func( tpc.func_name );
        if ( !val.isValid() ) continue;

        iter->push_back( prot_values{ QDateTime::currentDateTime().toUTC(), val.toFloat() } );

        ++iter;
    }
}

void ProtTask::initTagsValues()
{
    for (const TagProtConf & tpc : tag_prot_confs) {
        tags_values.push_back(TagValues());
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

    QtConcurrent::run( [tags_values, names, &database, &saving_now]()mutable {
        saving_now.ref();
        QList<QString>::const_iterator iter = names.begin();

        //for (TagValues & tv : tags_values) {
        for (TagsValues::iterator values_iter = tags_values.begin(); values_iter != tags_values.end(); ++values_iter  ) {
        QSqlError err = qx::dao::insert(*values_iter, &database, *iter);
            if ( err.isValid() ) {
                qWarning() << "prot_error while saving data! "<< err.databaseText() << " " << err.driverText();
            }
            ++iter;
        }
        saving_now.deref();
        qDebug() << "saved!";
    } );

    //tags_values.clear();
    clearDataInTagsValues();
}
