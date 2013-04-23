#include "tasksettings.h"
#include "perimeter.h"
#include "mainsequence.h"
#include "webservicesequence.h"
#include "stable.h"




void TaskSettings::initAlhos (QVector<AlhoSequence::Pointer> &alhos, Tags &tags, const QVariantMap& app_settings )
{
    openDocument();
    QDomElement el = findSettingsElement("alhos");

    //AlhoSequence::setSettings(getDynamicSettings(el));

    QDomElement maybe_alho_elem = el.firstChildElement();

    while (!maybe_alho_elem.isNull()) {
        if ( maybe_alho_elem.nodeName() == "alho" ) {
            if  ( maybe_alho_elem.attribute("name") == "MainSequence") {
                AlhoSequence::Pointer seq (new MainSequence(tags, app_settings));
                seq->setSettings( getDynamicSettings(maybe_alho_elem) );
                bindTags( maybe_alho_elem, tags, seq.data()  );

                alhos.push_back(seq);
            }
            else if  ( maybe_alho_elem.attribute("name") == "WebServiceSequence") {
                AlhoSequence::Pointer seq (new WebServiceSequence(tags, app_settings));
                seq->setSettings( getDynamicSettings(maybe_alho_elem) );
                bindTags( maybe_alho_elem, tags, seq.data()  );

                alhos.push_back(seq);
            }

            else {
                qWarning() << maybe_alho_elem.attribute("name") << " dont supported!! ";
                qFatal("exit");
            }
        }
        maybe_alho_elem = maybe_alho_elem.nextSiblingElement();
    }
}
