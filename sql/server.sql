/*==============================================================*/
/* DBMS name:      MySQL 4.0                                    */
/* Created on:     01.10.2009 14:09:11                          */
/*==============================================================*/


/*==============================================================*/
/* Table: t_action_log                                          */
/*==============================================================*/

drop database if exists testdb;

create database testdb;
use testdb;

drop table if exists t_action_log;
create table t_action_log
(
   id                             int unsigned                   not null AUTO_INCREMENT,
   dt                             datetime                       not null,
   event                          tinyint unsigned               not null,
   info                           varchar(256)                   not null,
   time_event                     time                           not null,
   type                           tinyint unsigned               not null,
   ind                            tinyint unsigned               not null,
   primary key (id)
);

/*==============================================================*/
/* Table: t_bum                                                 */
/*==============================================================*/
drop table if exists t_bum;
create table t_bum
(
   id                             tinyint unsigned               not null,
   state                          bool                           not null,
   queue                          tinyint,
   primary key (id)
);

/*==============================================================*/
/* Table: t_bum_state_log                                       */
/*==============================================================*/
drop table if exists t_bum_state_log;
create table t_bum_state_log
(
   id                             int unsigned                   not null AUTO_INCREMENT,
   date_time                      datetime                       not null,
   bum                            tinyint unsigned               not null,
   state                          bool                           not null,
   primary key (id)
);

/*==============================================================*/
/* Table: t_cars                                                */
/*==============================================================*/
drop table if exists t_cars;
create table t_cars
(
   id                             smallint unsigned              not null,
   name                           varchar(20)                    not null,
   dump_body_truck                bool                           not null,
   side_board                     bool                           not null,
   back_board                     bool                           not null,
   mark                           varchar(30)                    not null,
   tara                           smallint unsigned              not null,
   driver1                        varchar(20)                    not null,
   driver2                        varchar(20)                    not null,
   atp_id                         int unsigned                   not null,
   atp_name                       varchar(30)                    not null,
   num_field                      smallint unsigned              not null,
   num_loader                     tinyint unsigned               not null,
   trailer                        varchar(10),
   speed                          tinyint unsigned               not null,
   brutto                         smallint unsigned,
   block                          bool                           default 0,
   start_time                     datetime                       default '2000-01-01 00:00:00',
   amount_of_car_for_middle_tara  tinyint                        default -1,
   vremja_na_hodku                smallint unsigned,
   primary key (id)
);

/*==============================================================*/
/* Table: t_const                                               */
/*==============================================================*/
drop table if exists t_const;
create table t_const
(
   id                             varchar(64)                    not null,
   value                          varchar(256)                   not null,
   primary key (id)
);

/*==============================================================*/
/* Table: t_def_field                                           */
/*==============================================================*/
drop table if exists t_def_field;
create table t_def_field
(
   id                             tinyint unsigned               not null,
   num_field                      smallint unsigned,
   num_loader                     tinyint unsigned,
   primary key (id)
);

/*==============================================================*/
/* Table: t_field                                               */
/*==============================================================*/
drop table if exists t_field;
create table t_field
(
   id                             smallint unsigned              not null,
   distance                       smallint unsigned              not null,
   info                           varchar(100),
   primary key (id)
);

/*==============================================================*/
/* Table: t_kagat                                               */
/*==============================================================*/
drop table if exists t_kagat;
create table t_kagat
(
   id                             tinyint unsigned               not null,
   state                          bool                           not null default 0,
   primary key (id)
);

/*==============================================================*/
/* Table: t_kontr                                               */
/*==============================================================*/
drop table if exists t_kontr;
create table t_kontr
(
   id                             tinyint unsigned               not null,
   name                           varchar(50)                    not null,
   area                           varchar(50)                    not null,
   settlement                     varchar(50)                    not null,
   period                         tinyint unsigned               default 0,
   car_in_period                  tinyint unsigned               default 0,
   type                           tinyint unsigned               not null,
   primary key (id)
);

/*==============================================================*/
/* Table: t_rez                                                 */
/*==============================================================*/
drop table if exists t_rez;
create table t_rez
(
   num_nakl                       int unsigned                   not null,
   field                          smallint unsigned,
   bum                            tinyint unsigned,
   kagat                          tinyint unsigned,
   primary key (num_nakl)
);

/*==============================================================*/
/* Table: t_ttn                                                 */
/*==============================================================*/
drop table if exists t_ttn;
create table t_ttn
(
   date_time                      datetime                       not null,
   num_nakl                       int unsigned                   not null AUTO_INCREMENT,
   car                            smallint unsigned              not null,
   field                          smallint unsigned              not null,
   real_field                     smallint unsigned,
   loader                         tinyint unsigned,
   dt_of_load                     datetime,
   driver                         smallint unsigned              not null,
   brutto                         smallint unsigned              default 0,
   dt_of_brutto                   datetime,
   tara                           smallint unsigned              default 0,
   dt_of_tara                     datetime,
   bum                            tinyint unsigned,
   real_bum                       tinyint unsigned,
   kagat                          tinyint unsigned,
   dt_of_unload                   datetime,
   routed_to_lab                  bool,
   was_in_lab                     bool,
   num_kart                       varchar(20),
   copy                           bool,
   time_return                    datetime                       not null,
   trailer                        bool,
   time_of_brutto                 char(8),
   time_of_tara                   char(8),
   time_of_return                 char(8),
   real_loader                    tinyint unsigned,
   err                            tinyint unsigned               default 0,
   bum_platforma                  tinyint unsigned,
   rup_tara                       smallint unsigned,
   real_rup_tara                  smallint unsigned,
   brutto_platforma               tinyint unsigned,
   tara_platforma                 tinyint unsigned,
   field_from_car                 smallint unsigned,
   primary key (num_nakl)
);

