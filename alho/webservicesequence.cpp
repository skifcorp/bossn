#include "soapTestSoapBindingProxy.h"
#include "TestSoapBinding.nsmap"
#include "webservicesequence.h"

#include "asyncfuncbase.h"
#include "settingstool.h"

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



WebServiceSequence::WebServiceSequence(Tags & t, const QVariantMap& m) : MainSequenceBaseOp("WebServiceSequence", tags, m)
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
    if ( init  ) {
        init = false;
        return;
    }
    //while ( on_weight ) {

    //}

    WebServiceAsync w(*this);
    auto ret = w.exchangeData(QMap<QString, QString>());

    qDebug() << "got result: " << ret;
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

}






