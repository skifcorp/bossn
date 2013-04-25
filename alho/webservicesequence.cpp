#include "soapTestSoapBindingProxy.h"
#include "TestSoapBinding.nsmap"
#include "webservicesequence.h"

#include "asyncfuncbase.h"
#include "settingstool.h"

#include "warnmessages.h"
#include "mifarereader.h"
#include "mifarecard.h"

#include <QString>


class WebServiceAsync : public async_func_base2
{
public:
    WebServiceAsync(Coroutine2& c) : async_func_base2(c)
    {

    }

    ~WebServiceAsync()
    {

    }

    QMap<QString, QString> exchangeData( const QMap<QString, QString>& )
    {
        return async_exec([]{
            TestSoapBindingProxy proxy;
            _ns1__Hello hello;
            _ns1__HelloResponse resp;

            hello.param = "URRAAA";

            int ret = proxy.Hello( &hello, &resp );
            if ( ret != SOAP_OK ) {
                qWarning() << "soap FAILED: " << ret;

            }
            else {
                qDebug() << " ret: " << QString::fromStdString( resp.return_ );
            }

            QMap<QString, QString> retm;
            retm["aaa"] = QString::fromStdString( resp.return_ );

            return retm;
        }, "Exchange web service data", "Exchange web service data" );
    }

    void acceptedCardResult( bool )
    {

    }
};



WebServiceSequence::WebServiceSequence(Tags & t, const QVariantMap& m) : MainSequenceBaseOp("WebServiceSequence", t, m)
{

}

WebServiceSequence::~WebServiceSequence()
{

}

void WebServiceSequence::setSettings(const QVariantMap & s)
{
    alho_settings.init(s);

    seq_id                              = get_setting<int>("id", s);
#if 0
    current_card_tag = get_setting<QString>("current_card_tag", s);
    current_card_prop = get_setting<QString>("current_card_prop", s);
#endif

    setObjectName( "MainSequence num: " + QString::number(seq_id) );

    restart();

    cont();  //for initializing tablo and do
}

void WebServiceSequence::run()
{
    if ( init ) {
        printOnTablo(tr(greeting_message));
        setLightsToGreen();
        init = false;
        return;
    }

    seqDebug() << "something appeared on weight!!!! id" << seq_id;

    on_weight = true;

    printOnTablo(tr(apply_card_message));

    setLightsToRed();

    alho_settings.reader.do_on.func();


    QByteArray card_code = get_setting<QByteArray>("card_code" , app_settings);
    uint data_block      = get_setting<uint>      ("data_block", app_settings);

    while(on_weight) {
        ActivateCardISO14443A act = alho_settings.reader.activate_idle.func().value<ActivateCardISO14443A>();
        MifareCardSector card(act, alho_settings.reader, card_code, data_block);

        if ( !card.active() ) {
            sleepnbtm();
            continue;
        }

        try {
            printOnTablo(tr(processing_message));

            card.autorize();

            WebServiceAsync w(*this);
            auto ret = w.exchangeData(QMap<QString, QString>());

            sleepnb( get_setting<int>("brutto_finish_pause", app_settings) );
            printOnTablo( tr(apply_card_message) );                                  
            continue;
        }
        catch (MifareCardAuthException& ex) {
            seqWarning() << "auth_exeption! "<<ex.message();

            sleepnbtmerr(tr(card_autorize_error_message), tr(apply_card_message));
            continue;
        }
        catch (MifareCardReadException& ex) {
            seqWarning() << "read_card_exception! "<<ex.message();
            sleepnbtmerr(ex.message(), tr(apply_card_message));
            continue;
        }
        catch (MifareCardWriteException& ex) {
            seqWarning() << "write_card_exception! "<<ex.message();
            sleepnbtmerr(ex.message(), tr(apply_card_message));
            continue;
        }
        catch (MainSequenceException& ex) {
            seqWarning()<<"sequence_exception: " << ex.adminMessage() << " sys: " + ex.systemMessage();
            sleepnbtmerr(ex.userMessage(), tr(apply_card_message));
            continue;
        }
        catch (MifareCardException& ex) {
            seqWarning()<<"mifare_card_exception: " << ex.message();
            sleepnbtmerr(ex.message(), tr(apply_card_message));
            continue;
        }

    }
    seqDebug () << "\n\nexit from onAppearOnWeight!!!!!!!";

    printOnTablo(tr(greeting_message));
    setLightsToGreen();
    //tags[current_card_tag]->setProperty(current_card_prop,
    //                                   QVariant::fromValue<ActivateCardISO14443A>(ActivateCardISO14443A()));
    alho_settings.reader.do_off.func();


}


void WebServiceSequence::onAppearOnWeight(const QString&, AlhoSequence*)
{
    if ( status() == NotStarted || status() == Terminated ) {
        restart();
        cont();
    }
}

void WebServiceSequence::onDisappearOnWeight(const QString&, AlhoSequence*)
{
    seqDebug() << "something disappeared on weight!!!!";

    on_weight = false;

    if ( wake_timer.isActive() ) {
        //qDebug( )     << "1";
        wake_timer.stop();
        //qDebug( )     << "2";

        if (  status() == Stopped  )
            wakeUp();
    }
    //qDebug( )     << "3";

    qDebug() << "disappear finished!!!";

}


void WebServiceSequence::wakeUp()
{
    cont();
}



