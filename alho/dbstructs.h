#ifndef DBSTRUCTS_H
#define DBSTRUCTS_H

#include <QxOrm.h>
#include <QxMemLeak.h>

#include <QtGlobal>
#include <QDateTime>

struct t_ttn
{
    long num_nakl;
    QDateTime date_time;
    uint car;
    uint field;
    uint real_field;
    uint loader;
    QDateTime dt_of_load;
    uint driver;
    uint brutto;
    QDateTime dt_of_brutto;
    uint tara;
    QDateTime dt_of_tara;
    uint bum;
    uint real_bum;
    uint kagat;
    QDateTime dt_of_unload;
    bool routed_to_lab;
    bool was_in_lab;
    QString num_kart;
    bool copy;
    QDateTime time_return;
    bool trailer;
    QString time_of_brutto;
    QString time_of_tara;
    QString time_of_return;
    uint real_loader;
    uint err;
    uint bum_platforma;
    uint rup_tara;
    uint real_rup_tara;
    uint brutto_platforma;
    uint tara_platforma;
    uint field_from_car;

    t_ttn();
    t_ttn(long id):num_nakl(id){}

    virtual ~t_ttn() {}

    bool isValid () const {return num_nakl > 0;}
};

struct t_rez
{
    long num_nakl;
    uint field;
    uint bum;
    uint kagat;

    t_rez() {}
    t_rez(long id_):num_nakl(id_){}
    bool isValid() const {return num_nakl > 0;}
    virtual ~t_rez() {}
};

struct t_def_field
{
    long id;
    uint num_field;
    uint num_loader;

    t_def_field() {}
    t_def_field(long id_):id(id_){}

    bool isValid() const {return id > 0;}

    virtual ~t_def_field (){}
};


struct t_bum
{
    long id;
    bool state;
    uint queue;

    t_bum() {}
    t_bum(long id_):id(id_){}
    bool isValid() const {return id > 0;}
    virtual ~t_bum(){}
};


struct t_cars
{
    long id;
    QString name;
    bool dump_body_truck;
    bool side_board;
    bool back_board;
    QString mark;
    uint tara;
    QString driver1;
    QString driver2;
    uint atp_id;
    QString atp_name;
    uint num_field;
    uint num_loader;
    QString trailer;
    uint speed;
    uint brutto;
    bool block;
    QDateTime start_time;
    uint amount_of_car_for_middle_tara;
    uint vremja_na_hodku;

    t_cars() {}
    t_cars(long id_):id(id_) {}
    bool isValid() const {return id > 0;}
    virtual ~t_cars(){}
};

struct t_kagat
{
    long id;
    bool state;

    t_kagat(){}
    t_kagat(long id_):id(id_){}
    bool isValid() const {return id > 0;}

    virtual ~t_kagat(){}
};



struct t_kontr
{
    long id;
    QString name;
    QString area;
    QString settlement;
    uint period;
    uint car_in_period;
    uint type;

    t_kontr(){}
    t_kontr(long id_):id(id_){}
    bool isValid() const {return id > 0;}
    virtual ~t_kontr(){}
};


struct t_field
{
    long id;
    uint distance;
    QString info;

    t_field(){}
    t_field(long id_): id(id_){}
    bool isValid () const {return id > 0;}
    virtual ~t_field(){}
};


struct t_const
{
    long id;
    QString value;

    t_const(){}
    t_const(long id_):id(id_){}
    bool isValid() const {return id > 0;}

    virtual ~t_const(){}
};


struct t_bum_state_log
{
    long id;
    QString date_time;
    uint bum;
    bool state;

    t_bum_state_log (){}
    t_bum_state_log (long id_):id(id_){}
    bool isValid() const {return id > 0;}

    virtual ~t_bum_state_log (){}
};


struct t_action_log
{
    long id;
    QString dt;
    uint event;
    QString info;
    QTime time;
    uint type;
    uint ind;

    t_action_log(){}
    virtual ~t_action_log(){}
};


QX_REGISTER_HPP_EXPORT_DLL(t_ttn          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_rez          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_def_field    , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_bum          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_cars         , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_kagat        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_kontr        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_field        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_const        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_bum_state_log, qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_action_log   , qx::trait::no_base_class_defined, 1)


#endif // DBSTRUCTS_H
