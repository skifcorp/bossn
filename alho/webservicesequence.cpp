#include "soapTestSoapBindingProxy.h"
#include "TestSoapBinding.nsmap"
#include "webservicesequence.h"

WebServiceSequence::WebServiceSequence(Tags & t, const QVariantMap& m) : AlhoSequence("WebServiceSequence"), tags_(t),
    app_settings(m)
{

}

WebServiceSequence::~WebServiceSequence()
{

}

void WebServiceSequence::setSettings(const QVariantMap &)
{

}

void WebServiceSequence::run()
{

}


void WebServiceSequence::onAppearOnWeight(const QString&, AlhoSequence*)
{
    TestSoapBindingProxy proxy;
    _ns1__Hello hello;
    _ns1__HelloResponse resp;

    int ret = proxy.Hello( &hello, &resp );
    if ( ret != SOAP_OK ) {
        qWarning() << "soap FAILED: " << ret; return;
    }

    qDebug() << " ret: " << QString::fromStdString( resp.return_ );
}

void WebServiceSequence::onDisappearOnWeight(const QString&, AlhoSequence*)
{

}
