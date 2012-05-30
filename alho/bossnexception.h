#ifndef BOSSNEXCEPTION_H
#define BOSSNEXCEPTION_H

#include <QString>

class MysqlException
{
public:
    MysqlException (const QString& db_txt, const QString& drv_txt):database_text(db_txt), driver_text(drv_txt)
    {

    }
    ~MysqlException () {}

    QString databaseText() const {return database_text;}
    QString driverText() const {return driver_text;}
private:
    QString database_text;
    QString driver_text;
};



class MainSequenceException
{
public:
    MainSequenceException(const QString& user_msg, const QString& admin_msg, const QString& sys_msg = QString())
        :user_message(user_msg), admin_message(admin_msg), system_message(sys_msg){}

    ~MainSequenceException() {}

    QString userMessage() const {return user_message;}
    QString adminMessage() const {return admin_message;}
    QString systemMessage() const {return system_message;}
private:
    QString user_message;
    QString admin_message;
    QString system_message;
};

#endif // BOSSNEXCEPTION_H
