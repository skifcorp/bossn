#include "prottask.h"
#include "settingstool.h"

BossnFactoryRegistrator<ProtTask> ProtTask::registrator("ProtTask");

void ProtTask::setSettings(const QVariantMap & s)
{
    //qDebug() << "GOT SETTINGS!!!" << s;

    database = QSqlDatabase::addDatabase(get_setting<QString>("database_driver", s));

    database.setHostName(get_setting<QString>("database_host", s));
    database.setDatabaseName(get_setting<QString>("database_name", s));
    database.setUserName(get_setting<QString>("database_user", s));
    database.setPassword(get_setting<QString>("database_password", s));

    QVariantList tgs = get_setting<QVariantList>("tags", s);
    for( QVariant tg_ : tgs) {
        QVariantMap tg = tg_.toMap();
        TagProtConf tag_prot_conf;

        tag_prot_conf.tag_name  = get_setting<QString>("tag", tg);
        tag_prot_conf.func_name = get_setting<QString>("func", tg);
        tag_prot_conf.dz        = get_setting<QVariant>("dz", tg, QVariant(0));

        QString dz_type = get_setting<QString>("dz_type", tg, QString());
        if (dz_type == "absolute")
            tag_prot_conf.dz_type = TagProtConf::DzAbs;
        else if ( dz_type == "perc" )
            tag_prot_conf.dz_type = TagProtConf::DzPerc;


        tag_prot_confs.push_back(tag_prot_conf);
    }



}

void ProtTask::exec()
{
    qDebug() << "exec!!!";
}
