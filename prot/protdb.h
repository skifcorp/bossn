#ifndef PROTDB_H
#define PROTDB_H

#include "qxorm_pch.h"


#include <QString>

#include "func.h"

struct prot_conf
{
    QString NameDB;
    QString Names;
    QString NameVar;
    QString Parametrs;


    prot_conf() {}

    virtual ~prot_conf() {}
};


struct db_names
{
    QString DB_name;
    QString DB_r_name;

    db_names() {}
    virtual ~db_names() {}

};


struct prot_values
{
    QDateTime   time;
    float       value;

    prot_values() :value(0.0f) {}
    prot_values(const QDateTime& dt, float v) : time(dt), value(v){}
    virtual ~prot_values(){}

};

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

};

struct prot_work
{
    QDateTime start_from;
    QDateTime work_till;

    virtual ~prot_work(){}
};

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

#if 0

QX_REGISTER_PRIMARY_KEY(prot_values, QDateTime)
QX_REGISTER_PRIMARY_KEY(prot_work, QDateTime)
QX_REGISTER_PRIMARY_KEY(prot_value_scale, QDateTime)

QX_REGISTER_HPP_EXPORT_DLL(prot_conf          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(db_names           , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(prot_values        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(prot_value_scale   , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(prot_work          , qx::trait::no_base_class_defined, 1)

QX_REGISTER_HPP_EXPORT_DLL(message_log        , qx::trait::no_base_class_defined, 1)

#endif

#endif // PROTDB_H
