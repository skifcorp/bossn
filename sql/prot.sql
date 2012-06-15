
drop database if exists Config_files;
create database Config_files;
use Config_files;

drop table if exists prot_conf;

create table prot_conf 
(
	NameDB CHAR(255),
	Names CHAR(255), 
	NameVar CHAR(255), 
	Parametrs CHAR (255)
);

create table db_names 
(
	DB_name CHAR(255), 
	DB_r_name CHAR(255)
);


drop database if exists LOGDB;
create database LOGDB;


