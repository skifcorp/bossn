
#include "portersettings.h"
#include "porter.h"

#include <QStringList>


void PorterSettings::initTasks(TaskExec & tasks, Tags& tags, const QVariantMap& as)
{
    openDocument();
    QDomElement el = findSettingsElement("tasks");

    QDomElement task_element = el.firstChildElement();

    while ( !task_element.isNull() ) {
        BaseTask::Pointer task = BaseTask::create(task_element.attribute("name"), tags, as);

        task->setSettings(getDynamicSettings(task_element));

        bindTags(task_element, tags, task.data());
        tasks.addTask( task_element.attribute("cycle").toInt(), task );
        task_element = task_element.nextSiblingElement();
    }
}

QVariant PorterSettings::convertToType(const QDomNode& value_node, Tags * tags) const
{
    const QString type_name = value_node.nodeName();

    if ( type_name == "TagPlaceholder" ) {
        return QVariant::fromValue<TagPlaceholder>(TagPlaceholder(simpleValueFromPropertyNode(value_node).toInt()));
    }
    else if ( type_name == "Tag" ) {
        QStringList l = simpleValueFromPropertyNode(value_node).split("::");
        return QVariant::fromValue<TagBindable>(TagBindable( *tags->find(l[0]), l[1] ));
    }

    return AppSettings::convertToType(value_node);
}


void PorterSettings::initTags(Tags & tags)
{
    openDocument();
    QDomElement el = findSettingsElement("tags");

    QDomNode node = el.firstChildElement();
    while (!node.isNull()) {
        tags[node.nodeName()] = Tag::Pointer(new Tag( node.toElement().attribute("id") ));
        node = node.nextSiblingElement();
    }
}


void PorterSettings::initPorters(QVector<Porter::Pointer> &porters, Tags & tags)
{
    openDocument();
    QDomElement el = findSettingsElement("porters");

    QDomNode porter_node = el.firstChildElement();



    while ( !porter_node.isNull() ) {

        //qDebug() << porter_node.nodeName();

        porters.push_back( Porter::Pointer(new Porter(true)) );

        porters.last()->setDevice( porter_node.toElement().attribute("device_name"),  getDynamicSettings( porter_node ) );
        porters.last()->setScheduled( porter_node.toElement().attribute("scheduled") == "true" );

        initPorterDrivers( porters.last(), porter_node, tags );



        porter_node = porter_node.nextSiblingElement();
    }
}


void PorterSettings::initPorterDrivers ( Porter::Pointer porter, const QDomNode& porter_node, Tags& tags ) const
{
    QDomNode maybe_driver_node = porter_node.firstChildElement();

    while (!maybe_driver_node.isNull()) {
        if ( maybe_driver_node.nodeName() == "driver" ) {
            QDomElement el = maybe_driver_node.toElement();
            porter->addDriver( el.attribute("name"), getDynamicSettings(maybe_driver_node), getTagMethods(maybe_driver_node, tags) );
            bindTags( maybe_driver_node, tags, porter.data() );

        }
        maybe_driver_node = maybe_driver_node.nextSiblingElement();
    }
}


void PorterSettings::bindTags(const QDomNode & driver_node, Tags & tags, QObject * porter) const
{
    QDomElement maybe_tag_element = driver_node.firstChildElement().toElement();
    while ( !maybe_tag_element.isNull() ) {
        if ( maybe_tag_element.nodeName() == "tag" ) {
            QDomElement func_element = maybe_tag_element.firstChildElement().toElement();

            while ( !func_element.isNull() ) {
                Qt::ConnectionType ct = Qt::DirectConnection;
                if ( func_element.attribute("conn_type", "DirectConnection") == "QueuedConnection" )
                        ct = Qt::QueuedConnection;

                tags[maybe_tag_element.attribute("name")]->appendFunc( func_element.attribute("name"), porter, func_element.attribute("object_func"), ct );

                QDomElement arg_element = func_element.firstChildElement().toElement();
                while (!arg_element.isNull()) {
                    tags[maybe_tag_element.attribute("name")]->appendArgument(func_element.attribute("name"),
                                                                    //convertToType(arg_element.attribute("type"), arg_element.attribute("value"), &tags));
                                                                      convertToType(arg_element.firstChild(), &tags));
                    arg_element = arg_element.nextSiblingElement().toElement();
                }
                func_element = func_element.nextSiblingElement().toElement();
            }
        }
        maybe_tag_element = maybe_tag_element.nextSiblingElement().toElement();
    }
}

QList<TagMethod<false>> PorterSettings::getTagMethods(const QDomNode& driver_node, const Tags& tags) const
{
    QList<TagMethod<false>> tag_methods;

    QDomNode maybe_tag_node = driver_node.firstChildElement();

    while ( !maybe_tag_node.isNull() ) {
        if (maybe_tag_node.nodeName() == "tag") {
            QDomElement el = maybe_tag_node.toElement();

            TagMethod<false> tm( tags[el.attribute("name")]->tagName(), el.attribute("func", QString()) );
            tag_methods.push_back(tm);

        }

        maybe_tag_node = maybe_tag_node.nextSiblingElement();
    }

    return tag_methods;
}

