#ifndef BEET_DBSTRUCTS_H
#define BEET_DBSTRUCTS_H

#include "qxorm_pch.h"

#include <QtGlobal>
#include <QDateTime>

#include "func.h"

#include "genericdbstructs.h"


struct t_ttn_beet_mria
{
    long num_nakl;
    QDateTime date_time;
    uint car;
    uint field;
    uint real_field;
    uint loader;
    QDateTime dt_of_load;
    uint driver;
    int brutto;
    QDateTime dt_of_brutto;
    int tara;
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

    t_ttn_beet_mria();
    t_ttn_beet_mria(long id);//:num_nakl(id){}

    virtual ~t_ttn_beet_mria() {}

    bool isValid () const {return num_nakl > 0;}
};



struct t_rez_beet_mria
{
    long num_nakl;
    uint field;
    uint bum;
    uint kagat;

    t_rez_beet_mria() {}
    t_rez_beet_mria(long id_):num_nakl(id_){}
    bool isValid() const {return num_nakl > 0;}
    virtual ~t_rez_beet_mria() {}
};

struct t_def_field_beet_mria
{
    long id;
    uint num_field;
    uint num_loader;

    t_def_field_beet_mria() {}
    t_def_field_beet_mria(long id_):id(id_){}

    bool isValid() const {return id > 0;}

    virtual ~t_def_field_beet_mria (){}
};


struct t_bum_beet_mria
{
    long id;
    bool state;
    int queue;
    int kvoday;

    t_bum_beet_mria():id(0), state(false), queue(0) {}
    t_bum_beet_mria(long id_):id(id_), state(false), queue(0){}
    bool isValid() const {return id > 0;}
    virtual ~t_bum_beet_mria(){}
};


struct t_cars_beet_mria
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
    int amount_of_car_for_middle_tara;
    uint vremja_na_hodku;
    bool copy;
    bool fl_perimetr;
    QString smena;

    t_cars_beet_mria():id(-1), dump_body_truck(false), side_board(false),
        back_board(false), tara(0), atp_id(0), num_field(0), num_loader(0),
        speed(0), brutto(0), block(0),
        start_time(timeShitToDateTime(0)), amount_of_car_for_middle_tara(0),
        vremja_na_hodku(0)
    {}
    t_cars_beet_mria(long id_):id(id_), dump_body_truck(false), side_board(false),
        back_board(false), tara(0), atp_id(0), num_field(0), num_loader(0),
        speed(0), brutto(0), block(0),
        start_time(timeShitToDateTime(0)), amount_of_car_for_middle_tara(0),
        vremja_na_hodku(0) {}

    bool isValid() const {return id > 0;}
    virtual ~t_cars_beet_mria(){}
};

struct t_kagat_beet_mria
{
    long id;
    bool state;

    t_kagat_beet_mria():id(-1), state(false){}
    t_kagat_beet_mria(long id_):id(id_), state(false) {}
    bool isValid() const {return id > 0;}

    virtual ~t_kagat_beet_mria(){}
};



struct t_kontr_beet_mria
{
    long id;
    QString name;
    QString area;
    QString settlement;
    uint period;
    uint car_in_period;
    uint type;


    t_kontr_beet_mria():id(-1) ,period(0), car_in_period(0), type(0){}
    t_kontr_beet_mria(long id_):id(id_),period(0), car_in_period(0), type(0){}
    bool isValid() const {return id > 0;}
    virtual ~t_kontr_beet_mria(){}
};


struct t_field_beet_mria
{
    long id;
    uint distance;
    QString info;

    t_field_beet_mria():id(-1), distance(0){}
    t_field_beet_mria(long id_): id(id_),distance(0){}
    bool isValid () const {return id > 0;}
    virtual ~t_field_beet_mria(){}
};


struct t_const_beet_mria
{
    QString id;
    QString value;

    t_const_beet_mria(){}
    t_const_beet_mria(const QString& id_):id(id_){}
    bool isValid() const {return !id.isEmpty();}

    virtual ~t_const_beet_mria(){}
};

QX_REGISTER_PRIMARY_KEY(t_const_beet_mria, QString)

struct t_bum_state_log_beet_mria
{
    long id;
    QString date_time;
    uint bum;
    bool state;

    t_bum_state_log_beet_mria (){}
    t_bum_state_log_beet_mria (long id_):id(id_){}
    bool isValid() const {return id > 0;}

    virtual ~t_bum_state_log_beet_mria (){}
};


struct t_action_log_beet_mria
{
    long id;
    QString dt;
    uint event;
    QString info;
    QTime time;
    uint type;
    uint ind;

    t_action_log_beet_mria(){}
    virtual ~t_action_log_beet_mria(){}
};



QX_REGISTER_HPP_EXPORT_DLL(t_ttn_beet_mria          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_rez_beet_mria          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_def_field_beet_mria    , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_bum_beet_mria          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_cars_beet_mria         , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_kagat_beet_mria        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_kontr_beet_mria        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_field_beet_mria        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_const_beet_mria        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_bum_state_log_beet_mria, qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_action_log_beet_mria   , qx::trait::no_base_class_defined, 1)

#endif // DBSTRUCTS_H
