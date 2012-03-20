#ifndef DIDOER_H
#define DIDOER_H

#include <QTimer>
#include <QObject>

#include "didodriver.h"

class Didoer : public QObject
{
public:
    Didoer()
    {
        connect(&anti_jitter_timer, SIGNAL(timeout()), this, SLOT(onAntiJitterTimer()));
    }
    Didoer(const Didoer& ) = delete;

    void setAntiJitterPeriod(int msec)
    {
        anti_jitter_timer.setInterval(msec);
    }

    ~Didoer(){}
private slots:
    void onAntiJitterTimer()
    {

    }
    QTimer anti_jitter_timer;
    DidoDriver::Pointer dido;
};

#endif // DIDOER_H
