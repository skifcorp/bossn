#ifndef MAINSEQUENCE_H
#define MAINSEQUENCE_H

#include <QObject>
#include <QDebug>

#include "tags.h"

class MainSequence : public QObject
{
    Q_OBJECT
public:
    MainSequence(Tags & t):tags(t) {}
    ~MainSequence() {}
public slots:
    void onAppearOnWeight()
    {
        qDebug() << "something appeared on weight!!!!";
        //tags["tablo"]->func("value", Q_ARG(QVariant, "Hello"));
        //need turn on red and off green and yellow

        //tags["do3"]->func("writeMethod", Q_ARG(QVariant, true));
        tags["reader1"]->func("doOn");
    }

    void onDisappearOnWeight()
    {
        qDebug() << "something disappeared on weight!!!!";
        //tags["tablo"]->func("value", Q_ARG(QVariant,"Bye"));
        //tags["tablo"]->func("value", Q_ARG(QVariant, "Hello"));
        //need turn on red and off green and yellow
        //tags["do1"]->func("writeValue", Q_ARG(QVariant, true));
        tags["reader1"]->func("doOff");
    }

private:
    Tags & tags;
    QString tablo_func;
    QString red_tag;
    QString green_tag;
};

#endif // MAINSEQUENCE_H
