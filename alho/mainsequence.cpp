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
const QString get_car_for_bum_error_message          = QT_TRANSLATE_NOOP("MainSequence", "Getting car for bum failed! Contact to dispatcher");
const QString confuse_brutto_tara_error_message      = QT_TRANSLATE_NOOP("MainSequence", "You confused brutto with tara! Contact to dispatcher");

//const QString something_failed_in_alho               = QT_TRANSLATE_NOOP("MainSequence", "Platform autodetect failed! Contact to dispatcher");






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

QString MainSequence::detectPlatformType(const QVariantMap & bill) const throw (MainSequenceException)
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

    throw MainSequenceException(autodetect_platform_type_error_message ,"something terrible happens!!! cant detect platform type. Maybe bill corrupted :(" );
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

        try {
            card.autorize(card_code, data_block);

            QVariantMap bill = card.readStruct(bill_conf(options));

            if (!checkMember("billNumber", bill, 0) || !checkMember("driver", bill, 0)) {
                throw MainSequenceException(card_is_empty_error_message, "bill is empty");
            }

            auto car = wrap_async_ex(fetch_car_error_message, "fetching car failed!!!: driver: " + memberValue<QString>("driver", bill),
                                  [&bill, this]{return async_fetch<t_cars>( carCodeFromDriver( memberValue<uint>("driver", bill) ) );});


            if (car->block) {
                throw MainSequenceException(car_blocked_message, "car is blocked!!!");
            }

            auto ttn = wrap_async_ex(fetch_ttn_error_message, "fetching ttn failed!!!",
                                  [&bill, this]{return async_fetch<t_ttn>(bill["billNumber"].toUInt());});

            QString platform_type = detectPlatformType(bill);

            if (platform_type == "brutto" ) {
                brutto(bill, ttn);
                updateBruttoValues(bill, ttn);
            }
            else if (platform_type == "tara" ) {
                tara(bill, ttn);
                updateTaraValues(bill, ttn);
            }
        }
        catch (MifareCardAuthException& ex) {
            qWarning() << "auth_exeption! "<<ex.message();
            sleepnbtmerr(card_autorize_error_message, apply_card_message);
            continue;
        }
        catch (MifareCardException& ex) {
            qWarning() << "card_exception! "<<ex.message();
            sleepnbtmerr(ex.message(), apply_card_message);
            continue;
        }
        catch (MainSequenceException& ex) {
            qWarning()<< "sequence_exception! "<<ex.adminMessage();
            sleepnbtmerr(ex.userMessage(), apply_card_message);
            continue;
        }
    }

}

void MainSequence::repairBeetFieldCorrectnessIfNeeded(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn) const throw()
{  
    if ( memberValue<uint>("realNumField", bill) == 0 ||
         !wrap_async( [&bill, this]{
                                    return async_fetch<t_field>( memberValue<uint>("realNumField", bill) );
                                 } ) )  {

        setMemberValue("realNumField", memberValue<uint>("numField", bill), bill);
        ttn->real_field = memberValue<uint>("numField", bill);

        try {
            async_update(ttn);
        }
        catch(MysqlException& ex) {
            qWarning()<< ex.databaseText() + " " + ex.driverText();
        }
    }
}


uint MainSequence::countCarsFromFieldForDay(uint field_num) const throw()
{
    try {
        QDate workDate = QTime::currentTime().hour()<8 ? QDate::currentDate().addDays(-1) : QDate::currentDate();

        qx_query q("where floor(real_field/100) = "  + QString::number( field_num ) +
                   " and dt_of_brutto>='" + workDate.toString("yyyy.MM.dd") + " 08:00:00'"                            \
                   " and dt_of_brutto<='" + workDate.addDays(1).toString("yyyy.MM.dd") + " 08:00:00'");
        long count = 0;
        async_count<t_ttn>( count, q );

        return count;
    }
    catch (MysqlException& ex) {
        qWarning() << "cant get count of ttns for chemical analysis!!! field: "<<field_num;
    }

    return 0;
}


bool MainSequence::checkForNeedDiscreteAnalisys(long count) const throw()
{
    uint num_in_group = (count + 1) % get_setting<uint>("common_size_of_group", options);
    return num_in_group == get_setting<uint>("common_number_from_group", options);
}

uint MainSequence::getAnalisysPeriodFromStorage(uint typ) const throw(MysqlException, MainSequenceException)
{
    qx::dao::ptr<t_const> const_;

    if ( typ == 0 ) {
        const_ = wrap_async_ex( "error for kontragent type when doint chemical analysis typ=0", QString(),
                                [this]{return async_fetch<t_const>("ПериодичностьПроверкиМашинЮрЛиц");});
    }
    else if ( typ == 3 ) {
        const_ = wrap_async_ex( "error for kontragent type when doint chemical analysis typ=0", QString(),
                                [this]{return async_fetch<t_const>("ПериодичностьПроверкиМашинФермеров"); });
    }
    else {
        //qWarning()<< "error for kontragent type when doint chemical analysis";
        throw MainSequenceException("error for kontragent type when doint chemical analysis", QString());
    }

    return const_->value.toUInt();
}

bool MainSequence::checkForNeedDatabaseConstAnalisys(long count) const throw ()
{
    if (count + 1 == 1) return true; //first car always go for analysis

    try {
        auto kontr = wrap_async_ex( "cant get kontr of ttns for chemical analysis!!! field: " + QString::number(count),
                                     QString(),
                                    [&count, this]{return async_fetch<t_kontr>(count);} );

        uint period = kontr->period;

        if ( period == 0 ) {
            period = wrap_async_ex("error for getting const for type:" + QString::number(kontr->type), QString(),
                                   [&kontr, this]{return getAnalisysPeriodFromStorage(kontr->type);} );
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
        return num_in_group == carInPeriod;
    }
    catch (MainSequenceException& ex) {
        return true;
    }

}


void MainSequence::processChemicalAnalysis(QVariantMap & bill, qx::dao::ptr<t_ttn> )const throw()
{   
    long count   = countCarsFromFieldForDay( memberValue<uint>("realNumField", bill)/100 );

    QString alho = get_setting<QString>("common_algorithm_of_analysis", options);


    if ( alho == "discrete"  ) {
        if ( !checkForNeedDiscreteAnalisys(count) )
            return;
    }
    else if (alho == "database_const") {        
        if ( !checkForNeedDatabaseConstAnalisys( memberValue<uint>("realNumField", bill)/100 ) ) {
            return;
        }
    }

    uint analysis_place = 1;
    //определим место отбора
    analysis_place = (count+1)%3;
    if (analysis_place==0) analysis_place=3;

    QBitArray v(3);
    v.setBit(2, true);
    setMemberValue("flags", memberValue<QBitArray>("flags", bill) | v, bill );
    setMemberValue("pointOfAnal", analysis_place, bill);
}

void MainSequence::processFreeBum(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn) const
{
    /*auto car = async_fetch<t_cars>( carCodeFromDriver( memberValue<uint>("driver", bill) ) );
    if (!t_cars) {
        ///qWarning()<<"cant get car for nh"
    }*/

    qDebug() << "LETS START BUM!";
}

bool MainSequence::updateBruttoValues(QVariantMap&, qx::dao::ptr<t_ttn>) const
{
    return false;
}

bool MainSequence::updateTaraValues(QVariantMap&, qx::dao::ptr<t_ttn>) const
{
    return false;
}

bool MainSequence::isPureBruttoWeight(const QVariantMap& bill) const throw (MainSequenceException)
{
    if ( memberValue<uint>("bruttoWeight", bill) == 0 ) return true;
    if ( memberValue<uint>("kagat", bill) == 0 ) return false;

    throw MainSequenceException(confuse_brutto_tara_error_message, "confused brutto with tara");
}

bool MainSequence::checkDeltaForReweights(int prev_weight, int weight) const
{
    return qAbs( prev_weight - weight ) < get_setting<int>("brutto_delta_between_reweights", options);
}

void MainSequence::brutto(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn) const throw(MainSequenceException)
{
    repairBeetFieldCorrectnessIfNeeded(bill, ttn);

    int weight = getWeight();

    if ( !isWeightCorrect( weight ) ) {
        throw MainSequenceException(weight_not_stable_message, "brutto: weights dont stable!" );
    }

    if ( !isPureBruttoWeight(bill) && !checkDeltaForReweights(memberValue<uint>("bruttoWeight", bill), weight) ) {
        throw MainSequenceException(brutto_rupture_failed_message,
                                    "reweight: brutto rup failed!: prevWeight: " +
                                     memberValue<QString>("bruttoWeight", bill)  +
                                    " cur: " + QString::number(weight)  + " max_delta: " +
                                    get_setting<QString>("brutto_delta_between_reweights", options));
    }

    setMemberValue("bruttoWeight", weight, bill);
    setMemberValue("dateOfBrutto", QDateTime::currentDateTime(), bill);

    processChemicalAnalysis( bill, ttn );

    processFreeBum( bill, ttn );




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

void MainSequence::tara(QVariantMap & bill , qx::dao::ptr<t_ttn>) const throw (MainSequenceException)
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


