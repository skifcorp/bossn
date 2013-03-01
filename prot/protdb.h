#ifndef PROTDB_H
#define PROTDB_H

#include "rdb_pch.h"

#if 0
BOOST_RDB_BEGIN_TABLE(prot_conf)
    BOOST_RDB_COLUMN(NameDB   , varchar<50> )
    BOOST_RDB_COLUMN(Names    , varchar<50> )
    BOOST_RDB_COLUMN(NameVar  , varchar<256>)
    BOOST_RDB_COLUMN(Parametrs, varchar<512>)
BOOST_RDB_END_TABLE(prot_conf)

#endif

#include <QString>


/*
struct prot_conf
{
    QString NameDB;
    QString Names;
    QString NameVar;
    QString Parametrs;


    prot_conf() {}

    virtual ~prot_conf() {}
};
*/

/*
BOOST_RDB_BEGIN_TABLE(db_names)
    BOOST_RDB_COLUMN(DB_name, varchar<50>)
    BOOST_RDB_COLUMN(DB_r_name, varchar<50>)
BOOST_RDB_END_TABLE(db_names)
*/

/*
struct db_names
{
    QString DB_name;
    QString DB_r_name;

    db_names() {}
    virtual ~db_names() {}

};*/

BOOST_RDB_BEGIN_TABLE(prot_values_table)
    BOOST_RDB_COLUMN(time, datetime)
    BOOST_RDB_COLUMN(value, float_)
BOOST_RDB_END_TABLE(prot_values_table)

/*struct prot_values
{
    QDateTime   time;
    float       value;

    prot_values() :value(0.0f) {}
    prot_values(const QDateTime& dt, float v) : time(dt), value(v){}
    virtual ~prot_values(){}

};*/
/*
BOOST_RDB_BEGIN_TABLE(prot_value_scale)
    BOOST_RDB_COLUMN(time, datetime)
    BOOST_RDB_COLUMN(min, float_)
    BOOST_RDB_COLUMN(max, float_)
BOOST_RDB_END_TABLE(prot_value_scale)
*/
/*
struct prot_value_scale
{
    QDateTime time;
    float min;
    float max;

    prot_value_scale(): min(0.0f), max(0.0f)
    {

    }

    prot_value_scale(float mn, float mx ): time( timeShitToDateTime(0) ), min(mn), max(mx)
    {

    }

    virtual ~prot_value_scale() {}

};*/

BOOST_RDB_BEGIN_TABLE(prot_work_table)
    BOOST_RDB_COLUMN(start_from, datetime)
    BOOST_RDB_COLUMN(work_till, datetime)
BOOST_RDB_END_TABLE(prot_work_table)

/*
struct prot_work
{
    QDateTime start_from;
    QDateTime work_till;

    virtual ~prot_work(){}
};*/

BOOST_RDB_BEGIN_TABLE(message_log_table)
    BOOST_RDB_COLUMN(num, integer)
    BOOST_RDB_COLUMN(sender_id, integer)
    BOOST_RDB_COLUMN(type, integer)
    BOOST_RDB_COLUMN(message_date, datetime)
    BOOST_RDB_COLUMN(message, varchar<2048>)
BOOST_RDB_END_TABLE(message_log_table)


/*
struct message_log
{
    long num;
    int sender_id;
    int type;
    QDateTime when;
    QString message;
    message_log() : num(0), sender_id(0), type(-1){}
    message_log(int send_id, int t, const QString& txt):num(0), sender_id(send_id),
                                                        type(t), when(QDateTime::currentDateTime().toUTC()),
                                                        message(txt)
    {

    }

    virtual ~message_log() {}
};
*/


#endif // PROTDB_H
