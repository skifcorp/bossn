#include "tasksettings.h"
#include "perimeter.h"
#include "mainsequence.h"
#include "stable.h"

void TaskSettings::initTasks(TaskExec & tasks, Tags& tags)
{
    openDocument();
    QDomElement el = findSettingsElement("tasks");

    QDomElement task_element = el.firstChild().toElement();

    while ( !task_element.isNull() ) {
        BaseTask::Pointer task;
        if ( task_element.attribute("name") == "PerimeterTask" ) {
            task = BaseTask::Pointer(new PerimeterTask(tags));
        }
        else if (task_element.attribute("name") == "StableTask") {
            task = BaseTask::Pointer (new StableTask(tags));
        }
        else {
            qWarning() << task_element.attribute("name") << " dont supported!! ";
            qFatal("exit");
        }

        task->setSettings(getDynamicSettings(task_element));

        bindTags(task_element, tags, task.data());
        tasks.addTask( task_element.attribute("cycle").toInt(), task );
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
