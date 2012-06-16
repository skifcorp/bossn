#include "prottask.h"
#include "settingstool.h"
#include "protdb.h"
#include "async_func.h"

BossnFactoryRegistrator<ProtTask> ProtTask::registrator("ProtTask");

void ProtTask::setSettings(const QVariantMap & s)
{
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


        //qDebug () << "name_var: " << tag_prot_conf.NameVar;

        tag_prot_confs.push_back(tag_prot_conf);
    }


    database = QSqlDatabase::addDatabase(get_setting<QString>("database_driver", s), get_setting<QString>("connection_name", s));
    database.setHostName(get_setting<QString>("database_host", s));
    database.setDatabaseName(get_setting<QString>("config_database", s));
    database.setUserName(get_setting<QString>("database_user", s));
    database.setPassword(get_setting<QString>("database_password", s));

    initConfigForProtViewer(s);

    database.close();

    //QSqlDatabase::removeDatabase(database.connectionName());


    //database = QSqlDatabase::addDatabase(get_setting<QString>("database_driver", s), get_setting<QString>("connection_name", s));

    database.setHostName(get_setting<QString>("database_host", s));
    database.setDatabaseName(get_setting<QString>("database_name", s));
    database.setUserName(get_setting<QString>("database_user", s));
    database.setPassword(get_setting<QString>("database_password", s));

    initProtDataTables();



    save_timer.setInterval( get_setting<int>("save_interval", s) );
    connect(&save_timer, SIGNAL(timeout()), this, SLOT(onSaveTimer()));



    initTagsValues();

    save_timer.start();
}

void ProtTask::initProtDataTables()
{
    for (TagProtConf & tpc : tag_prot_confs) {
        try {
            //qDebug () << "name_var: " << tpc.NameVar;

            async_func_.wrap_async_ex( QString(), QString(), [this, &tpc]{return async_func_.async_create_table<prot_values>(tpc.NameVar);});
        }
        catch (MainSequenceException& ex)  {
            qWarning() << "cant initProtDatatable! " << ex.systemMessage();
        }
    }


}

void ProtTask::insertProtConf() throw (MainSequenceException)
{


    async_func_.wrap_async_ex(QString(), QString(), [this]{return async_func_.async_delete_all<prot_conf>();} );
    QList<prot_conf> prot_confs;

    for (TagProtConf & tpc : tag_prot_confs) {
        prot_confs.push_back(tpc);
    }



    async_func_.async_insert(prot_confs);
}

void ProtTask::insertDbNames(const QString& db_name, const QString& db_ru_name) throw (MainSequenceException)
{
    async_func_.wrap_async_ex(QString(), QString(), [this]{return async_func_.async_delete_all<db_names>();} );

    db_names dn;
    dn.DB_name   = db_name;
    dn.DB_r_name = db_ru_name;

    async_func_.async_insert(dn);
}


void ProtTask::initConfigForProtViewer(const QVariantMap& s)
{
    try {

        insertDbNames(get_setting<QString>("database_name", s), get_setting<QString>("database_ru_name", s));

        insertProtConf();
    }
    catch(MainSequenceException& ex){
        qWarning() << "initConfigForProtViewer error!!: db_text: " << ex.systemMessage();
    }

}

void ProtTask::exec()
{
    qDebug() << "exec!!!";

    TagsValues::Iterator iter = tags_values.begin();
    for (const TagProtConf & tpc : tag_prot_confs) {
        iter->push_back( prot_values{ QDateTime::currentDateTime().toUTC(), tags[ tpc.NameVar ]->func( tpc.func_name ).toFloat() } );

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
    QList<QString> names;
    for (const TagProtConf & tpc : tag_prot_confs) {
        names.push_back(tpc.NameVar);
    }

    QtConcurrent::run( [tags_values, names, &database]()mutable {
        QList<QString>::const_iterator iter = names.begin();

        //for (TagValues & tv : tags_values) {
        for (TagsValues::iterator values_iter = tags_values.begin(); values_iter != tags_values.end(); ++values_iter  ) {
        QSqlError err = qx::dao::insert(*values_iter, &database, *iter);
            if ( err.isValid() ) {
                qWarning() << "prot_error while saving data! "<< err.databaseText() << " " << err.driverText();
            }
            ++iter;
        }
    } );

    //tags_values.clear();
    clearDataInTagsValues();
}
