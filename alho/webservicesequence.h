#ifndef WEBSERVICESEQUENCE_H
#define WEBSERVICESEQUENCE_H

#include "alhosequence.h"
#include "tags.h"

class WebServiceSequence : public AlhoSequence
{
    Q_OBJECT
public:
    WebServiceSequence( Tags& t, const QVariantMap& m );
    ~WebServiceSequence();

    virtual void setSettings(const QVariantMap &) override;
private:
    Tags & tags_;
    const QVariantMap& app_settings;
};


#endif // WEBSERVICESEQUENCE_H
