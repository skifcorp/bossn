#ifndef SEQDEBUG_H
#define SEQDEBUG_H

#include <QDebug>
#include <QString>

class MainSequenceBaseOp;

class SeqDebug : public QDebug
{
public:
    SeqDebug(bool use_db, MainSequenceBaseOp& s, int err_code, bool toCons = true) :
        QDebug(&buffer), use_database(use_db), seq(s),
        error_code(err_code), to_console(toCons) {}
    ~SeqDebug();
private:
    bool use_database;
    MainSequenceBaseOp & seq;
    QString buffer;
    int error_code;
    bool to_console;
};

#endif // SEQDEBUG_H
