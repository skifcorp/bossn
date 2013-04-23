
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

}

void WebServiceSequence::onDisappearOnWeight(const QString&, AlhoSequence*)
{

}
