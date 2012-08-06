#ifndef SEQDEBUG_H
#define SEQDEBUG_H

#include <QDebug>
#include <QString>

class MainSequence;

class SeqDebug : public QDebug
{
public:
    SeqDebug(bool use_db, MainSequence& s, int err_code, bool toCons = true) :
        QDebug(&buffer), use_database(use_db), seq(s),
        error_code(err_code), to_console(toCons) {}
    ~SeqDebug();
private:
    bool use_database;
    MainSequence & seq;
    QString buffer;
    int error_code;
    bool to_console;
};

#endif // SEQDEBUG_H
