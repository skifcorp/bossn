#include "mainsequence.h"
#include "mifarecard.h"
#include "settingstool.h"
#include "func.h"

#include "dbstructs.h"
#include "datetimehack.h"

#include <QBitArray>
#include <QtConcurrentRun>


//#include <future>
#include <atomic>

//using std::async;
//using std::future;


const QString greeting_message                       = QT_TRANSLATE_NOOP("MainSequence", "Go on the weight platfotrm");
const QString apply_card_message                     = QT_TRANSLATE_NOOP("MainSequence", "Apply card to reader");
const QString card_autorize_error_message            = QT_TRANSLATE_NOOP("MainSequence", "Card autorization error!");
const QString card_reading_error_message             = QT_TRANSLATE_NOOP("MainSequence", "Card reading error!");
const QString card_is_empty_error_message            = QT_TRANSLATE_NOOP("MainSequence", "Card is empty error!");
const QString car_blocked_message                    = QT_TRANSLATE_NOOP("MainSequence", "You car is blocked! Contact to dispatcher!");
const QString fetch_car_error_message                = QT_TRANSLATE_NOOP("MainSequence", "Fetch car error! Contact to dispatcher");
const QString fetch_ttn_error_message                = QT_TRANSLATE_NOOP("MainSequence", "Fetch ttn error! Contact to dispatcher");
const QString weight_not_stable_message              = QT_TRANSLATE_NOOP("MainSequence", "Weights dont stable!");
const QString brutto_rupture_failed_message          = QT_TRANSLATE_NOOP("MainSequence", "Brutto rupture to big!");
const QString update_ttn_error_message               = QT_TRANSLATE_NOOP("MainSequence", "Update ttn error! Contact to dispatcher");
const QString autodetect_platform_type_error_message = QT_TRANSLATE_NOOP("MainSequence", "Platform autodetect failed! Contact to dispatcher");
const QString kvo_mah_error_message                  = QT_TRANSLATE_NOOP("MainSequence", "KvoMah failed! Contact to dispatcher");
const QString kontr_get_error_message                = QT_TRANSLATE_NOOP("MainSequence", "KontrGet failed! Contact to dispatcher");
//const QString something_failed_in_alho               = QT_TRANSLATE_NOOP("MainSequence", "Platform autodetect failed! Contact to dispatcher");



template <class Callable, class... Args >
typename std::result_of<Callable(Args...)>::type async_call(Callable c, Args ... args)
{
    //auto f = async(std::launch::async, c, args...);

    auto f = QtConcurrent::run(c, args...);

    while (!f.isFinished()) {
        qApp->processEvents();
    }

    return f.result();
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

    return tags["weight1_1"]->func("readMethod").toInt();
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

    printOnTablo(apply_card_message);

    tags["reader1"]->func("doOn");

    QByteArray card_code = get_setting<QByteArray>("card_code" , options);
    uint data_block      = get_setting<uint>      ("data_block", options);

    while(on_weight) {
        ActivateCardISO14443A act = tags["reader1"]->func("activateIdleA").value<ActivateCardISO14443A>();
        MifareCard card(tags["reader1"], act);

        if ( !card.active() ) {
            qDebug() << "card not active!!!";            
            sleepnbtm(); continue;
        }

        qDebug()<< "card active!";

        if ( !card.autorize(card_code, data_block) ) {
            qWarning() << "fail to autorize!!!!";
            sleepnbtmerr(card_autorize_error_message, apply_card_message); continue;
        }


        QVariantMap bill = card.readStruct(bill_conf(options));
        if ( bill.isEmpty() ) {
            qWarning() << "cant read bill!!!!";
            sleepnbtmerr(card_reading_error_message, apply_card_message); continue;
        }

        if (!checkMember("billNumber", bill, 0) || !checkMember("driver", bill, 0)) {
            qWarning() << "bill is empty!!!";
            sleepnbtmerr(card_is_empty_error_message, apply_card_message); continue;
        }

        auto car = async_fetch<t_cars>( carCodeFromDriver( memberValue<uint>("driver", bill) ) );
        if (!car) {
            qWarning() << "fetching car failed!!!: driver: "<<memberValue<uint>("driver", bill);

            sleepnbtmerr(fetch_car_error_message, apply_card_message); continue;
        }

        if (car->block) {
            qWarning() << "car is blocked!!!";
            sleepnbtmerr(car_blocked_message, apply_card_message); continue;
        }

        auto ttn = async_fetch<t_ttn>(bill["billNumber"].toUInt());

        if ( !ttn ) {
            qWarning() << "fetching ttn failed!!!";
            sleepnbtmerr(fetch_ttn_error_message, apply_card_message); continue;
        }


        QString platform_type = detectPlatformType(bill);

        if ( platform_type.isEmpty() ) {
            sleepnbtmerr(autodetect_platform_type_error_message, apply_card_message); continue;
        }

        if (platform_type == "brutto" && brutto(bill, ttn)) {
            updateBruttoValues(bill, ttn);
        }
        else if (platform_type == "tara" && tara(bill, ttn)) {
            updateTaraValues(bill, ttn);
        }
        else {
            continue;
        }

        if (!async_update(ttn)) {
            qWarning() << "cant update ttn!!!";
            sleepnbtmerr(update_ttn_error_message, apply_card_message); continue;
        }


        sleepnbtm();
    }

}

void MainSequence::repairBeetFieldCorrectnessIfNeeded(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn)
{
    uint real_num_field = memberValue<uint>("realNumField", bill);//bill["realNumField"].toUInt();

    //qDebug () << "real_num_field: "<< real_num_field<<" num_field: "<<memberValue<uint>("numField", bill);


    if ( real_num_field == 0 || !async_fetch<t_field>(real_num_field) )  {
        ttn->real_field = bill["numField"].toUInt();
        if ( !async_update(ttn) ) {
            qWarning() << "error updating beet field correctenss";
        }
        setMemberValue("realNumField", ttn->real_field, bill);
    }
}



void MainSequence::processChemicalAnalysis(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn)
{   
    long count          = 0;
    uint analysis_place = 1;

    { //doing something called getkvomah
        QDate workDate = QTime::currentTime().hour()<8 ? QDate::currentDate().addDays(-1) : QDate::currentDate();

        qx_query q("where floor(real_field/100) = "  + QString::number( memberValue<uint>("realNumField", bill)/100 ) +
                   " and dt_of_brutto>='" + workDate.toString("yyyy.MM.dd") + " 08:00:00'"                             \
                   " and dt_of_brutto<='" + workDate.addDays(1).toString("yyyy.MM.dd") + " 08:00:00'");

        if ( !async_count<t_ttn>( count, q ) ) {
            qWarning() << "cant get count of ttns for chemical analysis!!! field: "<<memberValue<uint>("realNumField", bill)/100;
            //sleepnbtmerr(kvo_mah_error_message, apply_card_message);
            //return false;
        }
    }

    QString alho = get_setting<QString>("common_algorithm_of_analysis", options);

    if ( alho == "discrete" ) {
        uint num_in_group = (count + 1) % get_setting<uint>("common_size_of_group", options);
        if ( num_in_group != get_setting<uint>("common_number_from_group", options) ) return;
    }
    else if (alho == "database_const") {
        if ( count + 1 == 1 ) {

        }
        else {
            auto kontr = async_fetch<t_kontr>(memberValue<uint>("realNumField", bill)/100);
            //, carInPeriod = 1;

            if ( !kontr )  {
                qWarning() << "cant get kontr of ttns for chemical analysis!!! field: "<<memberValue<uint>("realNumField", bill)/100;
            }
            uint period = kontr->period;

            qx::dao::ptr<t_const> const_;

            if ( period == 0 ) {
                if (kontr->type == 0) {
                    const_ = async_fetch<t_const>("ПериодичностьПроверкиМашинЮрЛиц");
                }
                else if ( kontr->type == 3 ) {
                    const_ = async_fetch<t_const>("ПериодичностьПроверкиМашинФермеров");
                }
                else {
                    qWarning()<< "error for kontragent type when doint chemical analysis";
                }

                if (!const_) {
                    qWarning()<<"error for getting const for type: "<<kontr->type;
                }

                period = const_->value.toUInt();
            }

            uint carInPeriod = kontr->car_in_period;
            if (carInPeriod == 0 ) {
                carInPeriod = 1;
                qWarning() << "error for getting carInPeriod. Kontragent dont have corrent chemical analisys params";
            }
            if ( period == 0 )  {
                period = 1;
                qWarning() << "error for getting period. Kontragent dont have corrent chemical analisys params";
            }

            uint num_in_group = (count + 1)%period;
            if (num_in_group == 0) num_in_group = period;
            if (num_in_group != carInPeriod) return;
        }
    }

    //определим место отбора
    analysis_place = (count+1)%3;
    if (analysis_place==0) analysis_place=3;

    QBitArray v(3);
    v.setBit(2, true);
    setMemberValue("flags", memberValue<QBitArray>("flags", bill) | v, bill );
    setMemberValue("pointOfAnal", analysis_place, bill);
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
        //printOnTablo(weight_not_stable_message); return false;
        sleepnbtmerr(weight_not_stable_message, apply_card_message); return false;
    }


    if (memberValue<uint>("bruttoWeight", bill) == 0) {

    }
    else if (memberValue<uint>("kagat", bill) == 0 ) {
        if ( qAbs( memberValue<uint>("bruttoWeight", bill) - weight ) >
             get_setting<uint>("brutto_delta_between_reweights", options) ) {
            qWarning() << "reweight: brutto rup failed!: prevWeight: "<< memberValue<uint>("bruttoWeight", bill)
                       << " cur: "<<weight<<" max_delta: "
                       << get_setting<uint>("brutto_delta_between_reweights", options);

            //printOnTablo( brutto_rupture_failed_message );
            sleepnbtmerr(brutto_rupture_failed_message, apply_card_message); return false;
            return false;
        }

        setMemberValue("flags", memberValue<QBitArray>("flags", bill) | QBitArray(1, true), bill );
    }

    setMemberValue("bruttoWeight", weight, bill);
    setMemberValue("dateOfBrutto", QDateTime::currentDateTime(), bill);

    processChemicalAnalysis( bill, ttn );

    return processFreeBum( bill, ttn );




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


