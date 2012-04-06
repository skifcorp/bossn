#ifndef MAINSEQUENCE_H
#define MAINSEQUENCE_H

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

#include "tags.h"


class MainSequence : public QObject
{
    Q_OBJECT
public:
    MainSequence(Tags & t):tags(t), on_weight(false)
    {
        if (card_code.isEmpty()) {
            card_code.append(0xFE);
            card_code.append(0x86);
            card_code.append(0x01);
            card_code.append(0x28);
            card_code.append(0x9F);
            card_code.append(0x3A);
        }
    }
    ~MainSequence() {}
    static inline void sleepnb(int msec)
    {
        QTime tm;
        tm.start();

        while (tm.elapsed() < msec) qApp->processEvents();
    }

public slots:
    void onAppearOnWeight();
    void onDisappearOnWeight();
private:
    Tags & tags;
    QString tablo_func;
    QString red_tag;
    QString green_tag;
    bool on_weight;

    static QByteArray card_code;


};

#endif // MAINSEQUENCE_H
