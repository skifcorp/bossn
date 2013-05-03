#include "mainsequencebaseop.h"
#include "warnmessages.h"
#include "bossnexception.h"

MainSequenceBaseOp::MainSequenceBaseOp(const QString& nm, Tags & t, const QVariantMap& s):AlhoSequence(nm),
    app_settings(s), tags(t), alho_settings(*this, tags), wake_timer(this)
{
    connect(&wake_timer, SIGNAL(timeout()), this, SLOT(wakeUp()));
    wake_timer.setSingleShot(true);
}

MainSequenceBaseOp::~MainSequenceBaseOp()
{

}



void MainSequenceBaseOp::checkForStealedCard(const ActivateCardISO14443A& card)
{
    if ( !alho_settings.current_card.containsProperty() || !alho_settings.current_card.property<ActivateCardISO14443A>().active() ) {
        alho_settings.current_card.setProperty( QVariant::fromValue<ActivateCardISO14443A>(card) );
        return;
    }

    if ( alho_settings.current_card.property<ActivateCardISO14443A>().uid == card.uid) return;

    throw MainSequenceException(tr(stealed_card_message), "You used stealed card!");
}
