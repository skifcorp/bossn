
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
