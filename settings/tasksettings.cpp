#include "tasksettings.h"
#include "perimeter.h"
#include "mainsequence.h"

void TaskSettings::initTasks(TaskExec & tasks, Tags& tags)
{
    openDocument();
    QDomElement el = findSettingsElement("tasks");

    QDomElement task_element = el.firstChild().toElement();

    while ( !task_element.isNull() ) {
        if ( task_element.attribute("name") == "PerimeterTask") {
            PerimeterTask::Pointer perim (new PerimeterTask(tags));
            perim->setSettings( getDynamicSettings(task_element) );

            tasks.addTask( task_element.attribute("cycle").toInt(), perim.staticCast<BaseTask::Pointer::Type>() );
        }
        else {
            qWarning() << task_element.attribute("name") << " dont supported!! ";
            qFatal("exit");
        }

        task_element = task_element.nextSibling().toElement();
    }
}


void TaskSettings::initAlhos (QVector<AlhoSequence::Pointer> &alhos, Tags &tags, const QVariantMap& app_settings )
{
    openDocument();
    QDomElement el = findSettingsElement("alhos");

    QDomElement alho_elem = el.firstChild().toElement();

    while (!alho_elem.isNull()) {
        if  ( alho_elem.attribute("name") == "MainSequence") {
            AlhoSequence::Pointer seq (new MainSequence(tags, app_settings));

            bindTags( alho_elem, tags, seq.data()  );

            alhos.push_back(seq);
        }
        else {
            qWarning() << alho_elem.attribute("name") << " dont supported!! ";
            qFatal("exit");
        }
        alho_elem = alho_elem.nextSibling().toElement();
    }
}
