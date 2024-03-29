#ifndef BEET_DBSTRUCTS_H
#define BEET_DBSTRUCTS_H

#include "qxorm_pch.h"

#include <QtGlobal>
#include <QDateTime>

#include "func.h"

#include "genericdbstructs.h"



struct t_ttn_beet
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

    t_ttn_beet();
    t_ttn_beet(long id);//:num_nakl(id){}

    virtual ~t_ttn_beet() {}

    bool isValid () const {return num_nakl > 0;}
};



struct t_rez_beet
{
    long num_nakl;
    uint field;
    uint bum;
    uint kagat;

    t_rez_beet() {}
    t_rez_beet(long id_):num_nakl(id_){}
    bool isValid() const {return num_nakl > 0;}
    virtual ~t_rez_beet() {}
};

struct t_def_field_beet
{
    long id;
    uint num_field;
    uint num_loader;

    t_def_field_beet() {}
    t_def_field_beet(long id_):id(id_){}

    bool isValid() const {return id > 0;}

    virtual ~t_def_field_beet (){}
};


struct t_bum_beet
{
    long id;
    bool state;
    int queue;
    int kvoday;

    t_bum_beet():id(0), state(false), queue(0) {}
    t_bum_beet(long id_):id(id_), state(false), queue(0){}
    bool isValid() const {return id > 0;}
    virtual ~t_bum_beet(){}
};


struct t_cars_beet
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
    QString smena;

    t_cars_beet():id(-1), dump_body_truck(false), side_board(false),
        back_board(false), tara(0), atp_id(0), num_field(0), num_loader(0),
        speed(0), brutto(0), block(0),
        start_time(timeShitToDateTime(0)), amount_of_car_for_middle_tara(0),
        vremja_na_hodku(0)
    {}
    t_cars_beet(long id_):id(id_), dump_body_truck(false), side_board(false),
        back_board(false), tara(0), atp_id(0), num_field(0), num_loader(0),
        speed(0), brutto(0), block(0),
        start_time(timeShitToDateTime(0)), amount_of_car_for_middle_tara(0),
        vremja_na_hodku(0) {}

    bool isValid() const {return id > 0;}
    virtual ~t_cars_beet(){}
};

struct t_kagat_beet
{
    long id;
    bool state;

    t_kagat_beet():id(-1), state(false){}
    t_kagat_beet(long id_):id(id_), state(false) {}
    bool isValid() const {return id > 0;}

    virtual ~t_kagat_beet(){}
};



struct t_kontr_beet
{
    long id;
    QString name;
    QString area;
    QString settlement;
    uint period;
    uint car_in_period;
    uint type;


    t_kontr_beet():id(-1) ,period(0), car_in_period(0), type(0){}
    t_kontr_beet(long id_):id(id_),period(0), car_in_period(0), type(0){}
    bool isValid() const {return id > 0;}
    virtual ~t_kontr_beet(){}
};


struct t_field_beet
{
    long id;
    uint distance;
    QString info;

    t_field_beet():id(-1), distance(0){}
    t_field_beet(long id_): id(id_),distance(0){}
    bool isValid () const {return id > 0;}
    virtual ~t_field_beet(){}
};


struct t_const_beet
{
    QString id;
    QString value;

    t_const_beet(){}
    t_const_beet(const QString& id_):id(id_){}
    bool isValid() const {return !id.isEmpty();}

    virtual ~t_const_beet(){}
};

#if 0
QX_REGISTER_PRIMARY_KEY(t_const_beet, QString)
#endif

struct t_bum_state_log_beet
{
    long id;
    QString date_time;
    uint bum;
    bool state;

    t_bum_state_log_beet (){}
    t_bum_state_log_beet (long id_):id(id_){}
    bool isValid() const {return id > 0;}

    virtual ~t_bum_state_log_beet (){}
};


struct t_action_log_beet
{
    long id;
    QString dt;
    uint event;
    QString info;
    QTime time;
    uint type;
    uint ind;

    t_action_log_beet(){}
    virtual ~t_action_log_beet(){}
};


#if 0
QX_REGISTER_HPP_EXPORT_DLL(t_ttn_beet          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_rez_beet          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_def_field_beet    , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_bum_beet          , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_cars_beet         , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_kagat_beet        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_kontr_beet        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_field_beet        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_const_beet        , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_bum_state_log_beet, qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_action_log_beet   , qx::trait::no_base_class_defined, 1)

#endif


#endif // DBSTRUCTS_H
