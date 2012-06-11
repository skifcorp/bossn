
#include "portersettings.h"
#include "porter.h"

#include <QStringList>


void PorterSettings::initTasks(TaskExec & tasks, Tags& tags, const QVariantMap& as)
{
    openDocument();
    QDomElement el = findSettingsElement("tasks");

    QDomElement task_element = el.firstChild().toElement();

    while ( !task_element.isNull() ) {
        BaseTask::Pointer task = BaseTask::create(task_element.attribute("name"), tags, as);
#if 0
        if ( task_element.attribute("name") == "PerimeterTask" ) {
            task = BaseTask::Pointer(new PerimeterTask(tags));
        }
        else if (task_element.attribute("name") == "StableTask") {
            task = BaseTask::Pointer(new StableTask(tags));
        }
        else {
            qWarning() << task_element.attribute("name") << " dont supported!! ";
            qFatal("exit");
        }
#endif
        task->setSettings(getDynamicSettings(task_element));

        bindTags(task_element, tags, task.data());
        tasks.addTask( task_element.attribute("cycle").toInt(), task );
        task_element = task_element.nextSibling().toElement();
    }
}

QVariant PorterSettings::convertToType(const QString & type_name, const QString & value, Tags * tags) const
{

    if ( type_name == "TagPlaceholder" ) {
        return  QVariant::fromValue<TagPlaceholder>(TagPlaceholder(value.toInt()));
    }
    else if ( type_name == "Tag" ) {
        QStringList l = value.split("::");
        return QVariant::fromValue<TagBindable>(TagBindable( *tags->find(l[0]), l[1] ));
    }

    return AppSettings::convertToType(type_name, value);
}


void PorterSettings::initTags(Tags & tags)
{
    openDocument();
    QDomElement el = findSettingsElement("tags");

    QDomNode node = el.firstChild();
    while (!node.isNull()) {
        tags[node.nodeName()] = Tag::Pointer(new Tag( node.toElement().attribute("id") ));
        node = node.nextSibling();
    }
}


void PorterSettings::initPorters(QVector<Porter::Pointer> &porters, Tags & tags)
{
    openDocument();
    QDomElement el = findSettingsElement("porters");

    QDomNode porter_node = el.firstChild();
    while ( !porter_node.isNull() ) {

        porters.push_back( Porter::Pointer(new Porter(true)) );

        porters.last()->setDevice( porter_node.toElement().attribute("device_name"),  getDynamicSettings( porter_node ) );
        porters.last()->setScheduled( porter_node.toElement().attribute("scheduled") == "true" );

        initPorterDrivers( porters.last(), porter_node, tags );



        porter_node = porter_node.nextSibling();
    }
}


void PorterSettings::initPorterDrivers ( Porter::Pointer porter, const QDomNode& porter_node, Tags& tags ) const
{
    QDomNode maybe_driver_node = porter_node.firstChild();

    while (!maybe_driver_node.isNull()) {
        if ( maybe_driver_node.nodeName() == "driver" ) {
            QDomElement el = maybe_driver_node.toElement();
            porter->addDriver( el.attribute("name"), getDynamicSettings(maybe_driver_node), getTagMethods(maybe_driver_node, tags) );
            bindTags( maybe_driver_node, tags, porter.data() );

        }
        maybe_driver_node = maybe_driver_node.nextSibling();
    }
}


void PorterSettings::bindTags(const QDomNode & driver_node, Tags & tags, QObject * porter) const
{
    QDomElement maybe_tag_element = driver_node.firstChild().toElement();
    while ( !maybe_tag_element.isNull() ) {
        if ( maybe_tag_element.nodeName() == "tag" ) {
            QDomElement func_element = maybe_tag_element.firstChild().toElement();

            while ( !func_element.isNull() ) {
                Qt::ConnectionType ct = Qt::DirectConnection;
                if ( func_element.attribute("conn_type", "DirectConnection") == "QueuedConnection" )
                        ct = Qt::QueuedConnection;

                tags[maybe_tag_element.attribute("name")]->appendFunc( func_element.attribute("name"), porter, func_element.attribute("object_func"), ct );

                QDomElement arg_element = func_element.firstChild().toElement();
                while (!arg_element.isNull()) {
                    tags[maybe_tag_element.attribute("name")]->appendArgument(func_element.attribute("name"),
                                                                    convertToType(arg_element.attribute("type"), arg_element.attribute("value"), &tags));
                    arg_element = arg_element.nextSibling().toElement();
                }
                func_element = func_element.nextSibling().toElement();
            }
        }
        maybe_tag_element = maybe_tag_element.nextSibling().toElement();
    }
}

QList<TagMethod> PorterSettings::getTagMethods(const QDomNode& driver_node, const Tags& tags) const
{
    QList<TagMethod> tag_methods;

    QDomNode maybe_tag_node = driver_node.firstChild();

    while ( !maybe_tag_node.isNull() ) {
        if (maybe_tag_node.nodeName() == "tag") {
            QDomElement el = maybe_tag_node.toElement();

            TagMethod tm( tags[el.attribute("name")]->tagName(), el.attribute("func", QString()) );
            tag_methods.push_back(tm);

        }

        maybe_tag_node = maybe_tag_node.nextSibling();
    }

    return tag_methods;
}

