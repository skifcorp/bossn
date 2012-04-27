#include "mainsequence.h"
#include "mifarecard.h"
#include "settingstool.h"
#include "func.h"

#include "dbstructs.h"
#include "datetimehack.h"

#include <QBitArray>



#include <future>
#include <atomic>

using std::async;
using std::future;


const QString greeting_message              = QT_TRANSLATE_NOOP("MainSequence", "Go on the weight platfotrm");
const QString apply_card_message            = QT_TRANSLATE_NOOP("MainSequence", "Apply card to reader");
const QString card_autorize_error_message   = QT_TRANSLATE_NOOP("MainSequence", "Card autorization error!");
const QString card_reading_error_message    = QT_TRANSLATE_NOOP("MainSequence", "Card reading error!");
const QString card_is_empty_error_message   = QT_TRANSLATE_NOOP("MainSequence", "Card is empty error!");
const QString car_blocked_message           = QT_TRANSLATE_NOOP("MainSequence", "You car is blocked! Contact to dispatcher!");
const QString fetch_car_error_message       = QT_TRANSLATE_NOOP("MainSequence", "Fetch car error! Contact to dispatcher");
const QString fetch_ttn_error_message       = QT_TRANSLATE_NOOP("MainSequence", "Fetch ttn error! Contact to dispatcher");
const QString weight_not_stable_message     = QT_TRANSLATE_NOOP("MainSequence", "Weights dont stable!");
const QString brutto_rupture_failed_message = QT_TRANSLATE_NOOP("MainSequence", "Brutto rupture to big!");
const QString update_ttn_error_message      = QT_TRANSLATE_NOOP("MainSequence", "Update ttn error! Contact to dispatcher");

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
    //qDebug () << "after";
}

int MainSequence::getWeight() const
{

    return tags["weight1_1"]->func("readWeight").toInt();
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
    qDebug() << "after";
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

        auto car = async_fetch<t_cars>( carCodeFromDriver( memberValue<uint>("car", bill) ) );
        if (!car) {
            qWarning() << "fetching car failed!!!";
            printOnTablo(fetch_car_error_message); sleepnbtm(); continue;
        }

        if (car->block) {
            qWarning() << "car is blocked!!!";
            printOnTablo(car_blocked_message); sleepnbtm(); continue;
        }

        auto ttn = async_fetch<t_ttn>(bill["billNumber"].toUInt());

        if ( !ttn ) {
            qWarning() << "fetching ttn failed!!!";
            printOnTablo(fetch_ttn_error_message); sleepnbtm(); continue;
        }


        QString platform_type = detectPlatformType(bill);

        if (platform_type == "brutto" && brutto(bill, ttn)) {
            updateBruttoValues(bill, ttn);
        }
        else if (platform_type == "tara" && tara(bill, ttn)) {
            updateTaraValues(bill, ttn);
        }
        else {
            sleepnbtm(); continue;
        }

        if (!async_update(ttn)) {
            qWarning() << "cant update ttn!!!";
            printOnTablo(update_ttn_error_message);
        }


        sleepnbtm();
    }

}

void MainSequence::repairBeetFieldCorrectnessIfNeeded(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn)
{
    uint real_num_field = memberValue<uint>("realNumField", bill);//bill["realNumField"].toUInt();

    if ( real_num_field == 0 || !async_fetch<t_field>(real_num_field) )  {
        ttn->real_field = bill["numField"].toUInt();
        if ( !async_update(ttn) ) {
            qWarning() << "error updating beet field correctenss";
        }
    }
}

bool MainSequence::processChemicalAnalysis(QVariantMap &, qx::dao::ptr<t_ttn>)
{
    return false;
}

bool MainSequence::processFreeBum(QVariantMap &, qx::dao::ptr<t_ttn>)
{
    return false;
}

bool MainSequence::updateBruttoValues(QVariantMap&, qx::dao::ptr<t_ttn>)
{
    return false;
}

bool MainSequence::updateTaraValues(QVariantMap&, qx::dao::ptr<t_ttn>)
{
    return false;
}

bool MainSequence::brutto(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn)
{
    repairBeetFieldCorrectnessIfNeeded(bill, ttn);

    int weight = getWeight();

    if (weight < 0) {
        qDebug() << "brutto: weights dont stable!";
        printOnTablo(weight_not_stable_message); return false;
    }


    if (memberValue<uint>("bruttoWeight", bill) == 0) {

    }
    else if (memberValue<uint>("kagat", bill) == 0 ) {
        if ( qAbs( memberValue<uint>("bruttoWeight", bill) - weight ) >
             get_setting<uint>("brutto_delta_between_reweights", options) ) {
            qWarning() << "reweight: brutto rup failed!: prevWeight: "<< memberValue<uint>("bruttoWeight", bill)
                       << " cur: "<<weight<<" max_delta: "
                       << get_setting<uint>("brutto_delta_between_reweights", options);

            printOnTablo( brutto_rupture_failed_message );
            return false;
        }

        setMemberValue("flags", memberValue<QBitArray>("flags", bill) | QBitArray(1, true), bill );
    }

    setMemberValue("bruttoWeight", weight, bill);
    setMemberValue("dateOfBrutto", QDateTime::currentDateTime(), bill);

    return processChemicalAnalysis( bill, ttn ) && processFreeBum( bill, ttn );




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

bool MainSequence::tara(QVariantMap & bill , qx::dao::ptr<t_ttn>)
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

    return false;
}

void MainSequence::onDisappearOnWeight()
{
    qDebug() << "something disappeared on weight!!!!";

    on_weight = false;

    tags["reader1"]->func("doOff");
    //tags["tablo"]->func("print", Q_ARG(const QVariant&, QVariant("Zaidkte na vagu")));
    printOnTablo(greeting_message);
}


