#ifndef WEBSERVICESEQUENCE_H
#define WEBSERVICESEQUENCE_H

#include "alhosequence.h"
#include "mainsequencesettings.h"
#include "tags.h"
#include "mainsequencebaseop.h"





class WebServiceSequence : public MainSequenceBaseOp
{
    Q_OBJECT
public:
    WebServiceSequence( Tags& t, const QVariantMap& m );
    ~WebServiceSequence();

    virtual void setSettings(const QVariantMap &) override;

    Q_INVOKABLE void onAppearOnWeight(const QString&, AlhoSequence*);
    Q_INVOKABLE void onDisappearOnWeight(const QString&, AlhoSequence*);

protected:
    virtual void run () override;
protected slots:
    virtual  void wakeUp() override;
private:
    bool on_weight  = false;
    bool init       = true;
    int seq_id      = 0;
};


#endif // WEBSERVICESEQUENCE_H
