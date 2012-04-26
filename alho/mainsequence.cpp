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

const QString greeting_message            = QT_TRANSLATE_NOOP("MainSequence", "Go on the weight platfotrm");
const QString apply_card_message          = QT_TRANSLATE_NOOP("MainSequence", "Apply card to reader");
const QString card_autorize_error_message = QT_TRANSLATE_NOOP("MainSequence", "Card autorization error!");
const QString card_reading_error_message  = QT_TRANSLATE_NOOP("MainSequence", "Card reading error!");
const QString card_is_empty_error_message = QT_TRANSLATE_NOOP("MainSequence", "Card is empty error!");
const QString car_blocked_message         = QT_TRANSLATE_NOOP("MainSequence", "You car is blocked! Contact to dispatcher!");
const QString fetch_car_error_message     = QT_TRANSLATE_NOOP("MainSequence", "Fetch car error! Contact to dispatcher");
const QString fetch_ttn_error_message     = QT_TRANSLATE_NOOP("MainSequence", "Fetch ttn error! Contact to dispatcher");
//const QString car_blocked_message         = QT_TRANSLATE_NOOP("MainSequence", "Your car is blocked! Contact to dispatcher");

template <class Callable, class... Args >
typename std::result_of<Callable(Args...)>::type async_call(Callable c, Args ... args)
{
    auto f = async(std::launch::async, c, args...);

    while (!f.is_ready()) {
        qApp->processEvents();
    }

    return f.get();
}

inline uint carCodeFromDriver(uint dr)
{
    return dr / 10;
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

void MainSequence::printOnTablo(const QString & s)
{
    tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant(s)));
}

MainSequence::MainSequence(Tags & t, const QVariantMap& opts):tags(t), options(opts), on_weight(false)
{
    printOnTablo(greeting_message);

    qx::QxSqlDatabase::getSingleton()->setDriverName(get_setting<QString>("database_driver", options));
    qx::QxSqlDatabase::getSingleton()->setDatabaseName(get_setting<QString>("database_name", options));
    qx::QxSqlDatabase::getSingleton()->setHostName(get_setting<QString>("database_host", options));    
    qx::QxSqlDatabase::getSingleton()->setUserName(get_setting<QString>("database_user", options));
    qx::QxSqlDatabase::getSingleton()->setPassword(get_setting<QString>("database_password", options));

    //t_ttn ttn;


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

    //tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant(apply_card_message)));
    printOnTablo(apply_card_message);

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
            printOnTablo(card_autorize_error_message);
            sleepnbtm(); continue;
        }


        QVariantMap bill = card.readStruct(bill_conf(options));
        if ( bill.isEmpty() ) {
            qDebug() << "cant read bill!!!!";
            printOnTablo( card_reading_error_message );
            sleepnbtm(); continue;
        }

        if (!checkMember("billNumber", bill, 0) || !checkMember("driver", bill, 0)) {
            qDebug() << "bill is empty!!!";
            printOnTablo( card_is_empty_error_message );
            sleepnbtm(); continue;
        }

        auto car = async_fetch<t_cars>( carCodeFromDriver( bill["car"].toUInt() ) );
        if (!car) {
            qWarning() << "fetching car failed!!!";
            printOnTablo(fetch_ttn_error_message); sleepnbtm(); continue;
        }

        if (car->block) {
            qWarning() << "car is blocked!!!";
            printOnTablo(car_blocked_message); sleepnbtm(); continue;
        }

/*      t_ttn ttn = fetch<t_ttn>(bill["billNumber"].toUInt());
        if (!ttn.isValid()) {
            qWarning() << "fetching ttn failed!!!";
            printOnTablo(fetch_ttn_error_message); sleepnbtm(); continue;
        } */



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
/*
t_ttn MainSequence::fetchTtn(const QVariantMap & bill) const
{
    t_ttn ttn( card["billNumber"].toUint() );

    QSqlError err = async_call([&ttn]{return qx::dao::fetch_by_id(ttn);});

    if  (err.isValid()) {
        qWarning( ) << "failed fetchTtn: "<<err.databaseText()<<" "<<err.driverText();
        return t_ttn(0);
    }
    return t_ttn;
}*/

void MainSequence::checkBeetFieldCorrectness(QVariantMap & bill, qx::dao::ptr<t_ttn>)
{
    uint real_num_field = bill["realNumField"].toUInt();

    if ( real_num_field == 0 || !async_fetch<t_field>(real_num_field) )  {

        //async_update
    }
}

void MainSequence::brutto(QVariantMap & bill)
{
    //preparing:
    uint real_num_field = bill["realNumField"].toUInt();
    auto ttn = async_fetch<t_ttn>(bill["billNumber"].toUInt());

    //if ()

    //check for replace field
    //checkBeetFieldCorrectness(bill);
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
    //tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant("Zaidkte na vagu")));
    printOnTablo(greeting_message);
}


