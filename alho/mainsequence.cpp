#include "mainsequence.h"
#include "mifarecard.h"
#include "settingstool.h"
#include "func.h"

#include "dbstructs.h"
#include "datetimehack.h"

//QByteArray MainSequence::card_code;

#include <future>
#include <atomic>

using std::async;
using std::future;
//using std::lunch;

template <class Callable, class... Args >
typename std::result_of<Callable(Args...)>::type async_call(Callable c, Args ... args)
{
    auto f = async(std::launch::async, c, args...);

    while (!f.is_ready()) {
        qApp->processEvents();
    }

    return f.get();
}

/*

template <class Callable, class... Args>
void async_call(Callable c, Args ... args)
{
    auto f = async(std::launch::async, c, args...);

    while (!f.is_ready()) {
        qApp->processEvents();
    }
}*/



MainSequence::MainSequence(Tags & t, const QVariantMap& opts):tags(t), options(opts), on_weight(false)
{
    tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant("Zaidjte na vagu")));

    qx::QxSqlDatabase::getSingleton()->setDriverName(get_setting<QString>("database_driver", options));
    qx::QxSqlDatabase::getSingleton()->setDatabaseName(get_setting<QString>("database_name", options));
    qx::QxSqlDatabase::getSingleton()->setHostName(get_setting<QString>("database_host", options));
    qx::QxSqlDatabase::getSingleton()->setHostName("192.168.0.231");
    qx::QxSqlDatabase::getSingleton()->setUserName(get_setting<QString>("database_user", options));
    qx::QxSqlDatabase::getSingleton()->setPassword(get_setting<QString>("database_password", options));

    t_ttn ttn;


    //qDebug() << "before";
    //qx::dao::insert(ttn);
    //async_call([&ttn]{qx::dao::insert(ttn);});
    //qDebug() << "after";
}

QString MainSequence::detectPlatformType(const QVariantMap & bill) const
{
    QString typ = get_setting<QString>("platform_type", options);
    if (typ != "auto") return typ;

    if (!checkMember("bruttoWeight", bill, 0) &&
        !checkMember("dateOfBrutto", bill, zero_date_time()))
        return "brutto";

    if ( !checkMember("bumFact", bill, 0) &&
         !checkMember("kagat", bill, 0) &&
         !checkMember("taraWeight", bill, 0) &&
         !checkMember("dateOfTara", bill, zero_date_time())) {
        return "brutto"; /*reweighting*/
    }

    if ( checkMember("bumFact", bill, 0) &&
         checkMember("kagat", bill, 0) ) {
        return "tara";
    }

    qWarning() << "something terrible happens!!! cant detect platform type. Maybe bill corrupted :( Bill: "<<bill;
    return QString();
}
void MainSequence::onAppearOnWeight()
{
    qDebug() << "something appeared on weight!!!!";
    on_weight = true;

    tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant("Pidnesith kartku")));
    tags["reader1"]->func("doOn");

    QByteArray card_code = get_setting<QByteArray>("card_code", options);
    uint data_block      = get_setting<uint>("data_block"     , options);

    while(on_weight) {
        ActivateCardISO14443A act = tags["reader1"]->func("activateIdleA").value<ActivateCardISO14443A>();
        MifareCard card(tags["reader1"], act);

        if ( !card.active() ) {
            qDebug() << "card not active!!!";

            sleepnbtm(); continue;
        }

        qDebug()<< "card active!";

        if ( !card.autorize(card_code, data_block) ) {
            qDebug() << "fail to autorize!!!!";

            sleepnbtm(); continue;
        }


        QVariantMap bill = card.readStruct(bill_conf(options));
        if ( bill.isEmpty() ) {
            qDebug() << "cant read bill!!!!";

            sleepnbtm(); continue;
        }

        if (!checkMember("billNumber", bill, 0) || !checkMember("driver", bill, 0)) {
            qDebug() << "bill is empty!!!";

            sleepnbtm(); continue;
        }

        /*
            if (isBlockedCar() ) {
            }
        */

        QString platform_type = detectPlatformType(bill);

        if (platform_type == "brutto") {
            brutto(bill);
        }
        else if (platform_type == "tara") {
            tara(bill);
        }
        else {

        }

        sleepnbtm();
    }

}

void MainSequence::checkFieldCorrectness(QVariantMap & bill)
{
    if (!checkMember("realNumField", bill , 0)  )  {

    }
}

void MainSequence::brutto(QVariantMap & bill)
{
    //preparing:
    //check for replace field
    checkFieldCorrectness(bill);
    //check if bill presents in db

    //weight or reweight:
    //for reweight checking delta with old and new weights
    //write weight and datetime
    //detect chemical analysis
    //detect free bum
    //updating mysql database
    //write to card
    //do sound
    //print message for tablo
}

void MainSequence::tara(QVariantMap & bill)
{
    //check for reweight tara and time for route and print

    //preparing:
    //check brutto
    //check bum
    //check kagat
    //check if kagat works

    //weight or reweight:
    //check if tara < brutto
    //for reweight checking delta with old and new weights
    //clear bum queque
    //write tara to db
    //analyze route time
    //printing
    //creating new task
    //
}

void MainSequence::onDisappearOnWeight()
{
    qDebug() << "something disappeared on weight!!!!";

    on_weight = false;

    tags["reader1"]->func("doOff");
    tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant("Zaidkte na vagu")));
}


