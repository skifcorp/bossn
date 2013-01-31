#ifndef GRAIN_DBSTRUCTS_H
#define GRAIN_DBSTRUCTS_H

#include "qxorm_pch.h"

#include <QtGlobal>
#include <QDateTime>

#include "func.h"

#include "genericdbstructs.h"

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
    uint culture;
    uint sort;
    uint repr;
    QString culture_name;

    t_ttn();
    t_ttn(long id);//:num_nakl(id){}

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
    int heavyweight;
    int client;
    int field;

    t_bum():id(0), state(false), queue(0) {}
    t_bum(long id_):id(id_), state(false), queue(0){}
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
    int amount_of_car_for_middle_tara;
    uint vremja_na_hodku;
    bool copy;
    bool fl_perimetr;
    QString smena;

    t_cars():id(-1), dump_body_truck(false), side_board(false),
        back_board(false), tara(0), atp_id(0), num_field(0), num_loader(0),
        speed(0), brutto(0), block(0),
        start_time(timeShitToDateTime(0)), amount_of_car_for_middle_tara(0),
        vremja_na_hodku(0)
    {}
    t_cars(long id_):id(id_), dump_body_truck(false), side_board(false),
        back_board(false), tara(0), atp_id(0), num_field(0), num_loader(0),
        speed(0), brutto(0), block(0),
        start_time(timeShitToDateTime(0)), amount_of_car_for_middle_tara(0),
        vremja_na_hodku(0) {}

    bool isValid() const {return id > 0;}
    virtual ~t_cars(){}
};

struct t_kagat
{
    long id;
    bool state;

    t_kagat():id(-1), state(false){}
    t_kagat(long id_):id(id_), state(false) {}
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
    int firstcar;

    t_kontr():id(-1) ,period(0), car_in_period(0), type(0){}
    t_kontr(long id_):id(id_),period(0), car_in_period(0), type(0){}
    bool isValid() const {return id > 0;}
    virtual ~t_kontr(){}
};


struct t_field
{
    long id;
    uint distance;
    QString info;

    t_field():id(-1), distance(0){}
    t_field(long id_): id(id_),distance(0){}
    bool isValid () const {return id > 0;}
    virtual ~t_field(){}
};


struct t_const
{
    QString id;
    QString value;

    t_const(){}
    t_const(const QString& id_):id(id_){}
    bool isValid() const {return !id.isEmpty();}

    virtual ~t_const(){}
};

#if 0
QX_REGISTER_PRIMARY_KEY(t_const, QString)
#endif

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

struct t_prikaz
{
    long num_nakl;
    QDateTime date_time;
    int lim;
    int rest;
    int place_out;
    int place_in;
    bool copy;
    QString culture;
    int sort;
    int repr;
    int virtual_rest;

    t_prikaz(long id);
    t_prikaz();

    virtual ~t_prikaz() {}
};

struct t_ttno
{
    t_ttno(long id);
    t_ttno();

    virtual ~t_ttno() {}

    QDateTime date_time;
    long num_nakl;
    int car;
    int prikaz;
    int place_out;
    int place_in;
    int driver;
    int brutto;
    QDateTime dt_of_brutto;
    int tara;
    QDateTime dt_of_tara;
    QString num_kart;
    int copy;
    int trailer;
    QString culture;
    int sort;
    int repr;

    int netto() const {return brutto - tara;}
};

#if 0
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
QX_REGISTER_HPP_EXPORT_DLL(t_prikaz       , qx::trait::no_base_class_defined, 1)
QX_REGISTER_HPP_EXPORT_DLL(t_ttno         , qx::trait::no_base_class_defined, 1)
#endif

#endif // DBSTRUCTS_H
