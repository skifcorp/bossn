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
const QString get_backboard_bum_weight_const_error   = QT_TRANSLATE_NOOP("MainSequence", "Getting backboard bum weight error! Contact to dispatcher");
const QString get_free_bum_error                     = QT_TRANSLATE_NOOP("MainSequence", "Getting free bum error! Contact to dispatcher");
const QString update_bum_queue_error                 = QT_TRANSLATE_NOOP("MainSequence", "Updating bum queue error! Contact to dispatcher");
//const QString update_ttn_platform_error              = QT_TRANSLATE_NOOP("MainSequence", "Up! Contact to dispatcher");

//const QString something_failed_in_alho               = QT_TRANSLATE_NOOP("MainSequence", "Platform autodetect failed! Contact to dispatcher");

const QString brutto_finish_weight_message                  = QT_TRANSLATE_NOOP("MainSequence", "%1 kg");
const QString brutto_finish_lab_message                     = QT_TRANSLATE_NOOP("MainSequence", "Lab(%1)");
const QString brutto_finish_bum_message                     = QT_TRANSLATE_NOOP("MainSequence", "BUM %1");

inline QString MainSequence::bruttoFinishMessage(const QVariantMap & bill) const
{
    QString ret;
    ret = brutto_finish_weight_message.arg( memberValue<QString>("bruttoWeight", bill) );
    if ( memberValue<QBitArray>("flags", bill).at(2) ) {
        ret += brutto_finish_lab_message.arg( memberValue<QString>("pointOfAnal", bill) );
    }
    ret += brutto_finish_bum_message.arg( memberValue<QString>("bum", bill) );

    return ret;
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



            QString platform_type = detectPlatformType(bill);

            if (platform_type == "brutto" ) {
                auto ttn = wrap_async_ex(fetch_ttn_error_message, "fetching ttn failed!!!",
                                  [&bill, this]{return async_fetch<t_ttn>(bill["billNumber"].toUInt());});
                brutto(bill, ttn, car);
                updateBruttoValues(bill, ttn, card);
                card.writeStruct(bill_conf(options), bill);

                printOnTablo( bruttoFinishMessage(bill) );
                sleepnb( get_setting<int>("brutto_finish_pause", bill) );
                printOnTablo( apply_card_message );
                continue;
            }
            else if (platform_type == "tara" ) {
                //tara(bill, ttn);
                //updateTaraValues(bill, ttn);
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
            //async_update(ttn);
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
                                [this]{return async_fetch<t_const>(get_setting<QString>("corpotare_check_period_name", options));});
    }
    else if ( typ == 3 ) {
        const_ = wrap_async_ex( "error for kontragent type when doint chemical analysis typ=3", QString(),
                                [this]{return async_fetch<t_const>(get_setting<QString>("farmer_check_period_name", options)); });
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

QString MainSequence::getBumsClause(const QVariantMap & bill, qx::dao::ptr<t_cars> car) const throw(MainSequenceException)
{
    QStringList ret;
    if (car->dump_body_truck) {
        ret.append("id = 99");
    }

    if (car->side_board) {
        ret.append("(id % 10) = 2");
    }

    if (car->back_board) {
        qx::dao::ptr<t_const> const_ = wrap_async_ex(get_backboard_bum_weight_const_error, "error getting const for backboard bum",
                        //[this]{return async_fetch<t_const>();});
                        [this]{return async_fetch<t_const>(get_setting<QString>("bum11_name", options));});

        if ( memberValue<int>("bruttoWeight", bill) < const_->value.toInt() ) {
            ret.append("id = 11");
        }
        else {
            ret.append("(id % 10) = 1");
        }
    }

    return  ret.join(" or ");
}

void MainSequence::processFreeBum(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car) const throw(MainSequenceException)
{
    QString bums_where_clause = getBumsClause(bill, car);

    const QString q1 =  "select * from t_bum where state=1 and queue=(select min(queue) from t_bum where state=1 and ("+bums_where_clause+")) and ("+bums_where_clause +");";
    const QString q2 =  "select * from t_bum where queue=(select min(queue) from t_bum where ("+bums_where_clause+")) and ("+bums_where_clause +");";

    qx::dao::ptr<t_bum> bum = wrap_async_ex(get_free_bum_error, "Error getting free bum1",
        [&bums_where_clause, &q1, this]{ return async_exec_query<t_bum>(q1);});

    if ( !bum ) {
        bum = wrap_async_ex(get_free_bum_error, "Error getting free bum2",
            [&bums_where_clause, &q2, this]{ return async_exec_query<t_bum>(q2);});
    }

    bum->queue += 1;
    wrap_async_ex( update_bum_queue_error, "Error updating bum queue", [&bum, this]{ async_update(bum); });

    if ( bum->id != 99 ) {
        ttn->bum_platforma = bum->id % 10;
        //wrap_async_ex( update_ttn_platform_error, "Error updating ttn platform", [&ttn]{ async_update(ttn); });
        setMemberValue("bum", ttn->bum_platforma / 10, bill);
    }
    else {
        setMemberValue("bum", ttn->bum_platforma, bill);
    }
}

void MainSequence::updateBruttoValues(QVariantMap& bill, qx::dao::ptr<t_ttn> ttn, const MifareCard& card) const throw(MainSequenceException)
{    
    ttn->real_field       = memberValue<int>("realNumField", bill);
    ttn->loader           = memberValue<int>("numLoader", bill);
    ttn->dt_of_load       = memberValue<QDateTime>("dateOfLoad", bill);
    ttn->brutto           = memberValue<int>("bruttoWeight", bill);
    ttn->dt_of_brutto     = memberValue<QDateTime>("dateOfBrutto", bill);
    ttn->driver           = carCodeFromDriver( memberValue<int>("driver", bill) ) ;
    ttn->bum              = memberValue<int>("bum", bill);
    ttn->routed_to_lab    = memberValue<QBitArray>("flags", bill).at(2);
    ttn->num_kart         = byteArrayToString (card.uid());
    ttn->copy             = 0;
    ttn->time_of_brutto   = ttn->dt_of_brutto.time().toString("hh:mm:ss");
    ttn->brutto_platforma = 88;

    wrap_async_ex( update_ttn_error_message, "Error updating ttn", [&ttn, this]{ async_update(ttn); });
}

void MainSequence::updateTaraValues(QVariantMap&, qx::dao::ptr<t_ttn>) const throw(MainSequenceException)
{
  /*("pointOfAnal"   , bill);
    ("taraWeight"    , bill);
    ("normTaraWeight", bill);
    ("dateOfTara"    , bill);
    ("impurity"      , bill);
    ("shugarContent" , bill);
    ("greenWeight"   , bill);
    ("bumFact"       , bill);
    ("kagat"         , bill);
    ("dateOfUnload"  , bill);*/
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

void MainSequence::brutto(QVariantMap & bill, qx::dao::ptr<t_ttn> ttn, qx::dao::ptr<t_cars> car) const throw(MainSequenceException)
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

    processFreeBum( bill, ttn, car );




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


