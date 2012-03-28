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
    }

    void onDisappearOnWeight()
    {
        qDebug() << "something disappeared on weight!!!!";
    }

private:
    Tags & tags;
};

#endif // MAINSEQUENCE_H
