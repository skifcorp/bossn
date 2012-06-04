#include "initsettings.h"
#include "tags.h"



#include <QList>
#include <QVariantMap>
#include <QFile>
#include <QStringList>

#ifndef PROG_OPTIONS_ONLY
#include "porter.h"
#endif

//#include "qextserialport.h"

const QString AppSettings::settings_file_name = "settings.xml";

void AppSettings::openDocument()
{
    QFile file(settings_file_name);


    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "cant open settings file!!! "<<settings_file_name;
        qFatal("exit!");
    }

    QString error_msg; int error_line = 0, error_column = 0;
    if (!document.setContent(&file, &error_msg, &error_line, &error_column)) {
        qWarning() << "cant set content of settings file!!!! msg: " << error_msg << " line: "<<error_line << " error_col: "<<error_column;
        qFatal("exit!");
    }
}

QDomElement AppSettings::findSettingsElement(const QString & n) const
{
    QDomElement el = document.firstChildElement("doc");

    if ( el.isNull() ) {
        qWarning() << "cant find DOC element!!!";
        qFatal( "exit");
    }

    el = el.firstChildElement(n);

    if ( el.isNull() ) {
        qWarning() << "cant find " + n + " element!!!";
        qFatal( "exit");
    }
    return el;
}

QVariant AppSettings::convertToType(const QString & type_name, const QString & value
#ifndef PROG_OPTIONS_ONLY
, Tags * tags
#endif
) const
{
#ifndef PROG_OPTIONS_ONLY
    if ( type_name == "TagPlaceholder" ) {
        return  QVariant::fromValue<TagPlaceholder>(TagPlaceholder(value.toInt()));
    }
    else if ( type_name == "Tag" ) {
        QStringList l = value.split("::");
        return QVariant::fromValue<TagBindable>(TagBindable( *tags->find(l[0]), l[1] ));
    }
#endif

    if ( type_name == "QByteArray" ) {
        QByteArray arr;
        for (int i = 0; i<value.count(); i+=2) {
            QString byte = value.mid(i, 2);
            arr.push_front( byte.toInt(0, 16) );
        }
        return QVariant(arr);
    }

    QVariant v (value);

    if  ( !v.convert( QVariant::nameToType( type_name.toAscii() ) ) ) {
        qWarning() << "cant convert value: "  <<  value << " to type: " << type_name;
        qFatal("exit");
    }

    return v;
}

void AppSettings::initProgOptions(QVariantMap & opts)
{
    openDocument();
    QDomElement el = findSettingsElement("app");

    QDomNode prop_node = el.firstChild();
    while (!prop_node.isNull()) {
        QDomElement prop_elem = prop_node.toElement();

        opts[ prop_elem.attribute("name") ] = convertToType( prop_elem.attribute("type"), prop_elem.attribute("value") ) ;

        prop_node = prop_node.nextSibling();
    }
}


#ifndef PROG_OPTIONS_ONLY
void AppSettings::initTags(Tags & tags)
{
    openDocument();
    QDomElement el = findSettingsElement("tags");

    QDomNode node = el.firstChild();
    while (!node.isNull()) {
        tags[node.nodeName()] = Tag::Pointer(new Tag( node.toElement().attribute("id") ));
        node = node.nextSibling();
    }
}
#endif

#ifndef PROG_OPTIONS_ONLY
void AppSettings::initPorters(QVector<Porter::Pointer> &porters, Tags & tags)
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
#endif

#ifndef PROG_OPTIONS_ONLY
void AppSettings::initPorterDrivers ( Porter::Pointer porter, const QDomNode& porter_node, Tags& tags ) const
{
    QDomNode maybe_driver_node = porter_node.firstChild();

    while (!maybe_driver_node.isNull()) {
        if ( maybe_driver_node.nodeName() == "driver" ) {
            QDomElement el = maybe_driver_node.toElement();
            porter->addDriver( el.attribute("name"), getDynamicSettings(maybe_driver_node), getTagMethods(maybe_driver_node, tags) );
            bindTags( maybe_driver_node, tags, porter );

        }
        maybe_driver_node = maybe_driver_node.nextSibling();
    }
}
#endif

#ifndef PROG_OPTIONS_ONLY
void AppSettings::bindTags(const QDomNode & driver_node, Tags & tags, Porter::Pointer porter) const
{
    QDomElement tag_element = driver_node.firstChild().toElement();
    while ( !tag_element.isNull() ) {
        QDomElement func_element = tag_element.firstChild().toElement();

        while ( !func_element.isNull() ) {
            tags[tag_element.attribute("name")]->appendFunc( func_element.attribute("name"), porter.data(), func_element.attribute("porter_func") );

            QDomElement arg_element = func_element.firstChild().toElement();
            while (!arg_element.isNull()) {
                tags[tag_element.attribute("name")]->appendArgument(func_element.attribute("name"),
                                                                    convertToType(arg_element.attribute("type"), arg_element.attribute("value"), &tags));
                arg_element = arg_element.nextSibling().toElement();
            }

            func_element = func_element.nextSibling().toElement();
        }

        tag_element = tag_element.nextSibling().toElement();
    }
}
#endif
QVariantMap AppSettings::getDynamicSettings( const QDomNode& par_node) const
{
    QDomNode n = par_node.firstChild();
    QVariantMap ret;
    while ( !n.isNull() ) {
        if (n.nodeName() == "property") {
            QDomElement el = n.toElement();
            ret[el.attribute("name")] = el.attribute("value");
        }
        n = n.nextSibling();
    }
    return ret;
}

#ifndef PROG_OPTIONS_ONLY
QList<TagMethod> AppSettings::getTagMethods(const QDomNode& driver_node, const Tags& tags) const
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
#endif

#if 0
void AppSettings::initTablo(QVector<Porter::Pointer>& porters, Tags & tags)
{
    QVariantMap serial_settings_tablo;

    serial_settings_tablo["baudRate"]    = BAUD19200;
    serial_settings_tablo["dataBits"]    = DATA_8;
    serial_settings_tablo["flowControl"] = FLOW_OFF;
    serial_settings_tablo["parity"]      = PAR_ODD;
    serial_settings_tablo["stopBits"]    = STOP_1;
    serial_settings_tablo["timeout"]     = 100;
    serial_settings_tablo["portName"]    = "COM4";

    QList<TagMethod> tag_method_tablo;
    tag_method_tablo.append( TagMethod("tablo") );

    Porter::Pointer w = Porter::Pointer(new Porter(true));
    w->setScheduled(false);

    w->setDevice("IoDeviceSerial", serial_settings_tablo);
    w->addDriver("DisplayFutaba", QVariantMap(), tag_method_tablo);

    porters.push_back(w);
    tags["tablo"]->appendFunc("print", w.data(), "exec" );
    tags["tablo"]->appendArgument("print", "printText");
    tags["tablo"]->appendArgument("print", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    qDebug() <<"tablo initialized!";
}

void AppSettings::initReader(QVector<Porter::Pointer>& porters, Tags & tags)
{
    QList<TagMethod> tag_method_reader;
    tag_method_reader.append(TagMethod("reader1"));


    Porter::Pointer w = Porter::Pointer(new Porter(true));
    w->setScheduled(false);

    QMap <QString, QVariant> serial_settings;

    serial_settings["baudRate"]    = BAUD9600;
    serial_settings["dataBits"]    = DATA_8;
    serial_settings["flowControl"] = FLOW_OFF;
    serial_settings["parity"]      = PAR_NONE;
    serial_settings["stopBits"]    = STOP_1;
    serial_settings["timeout"]     = 100;
    serial_settings["portName"]    = "COM3";

    QMap<QString, QVariant> opts;
    opts["address"] = 2;

    w->setDevice("IoDeviceSerial", serial_settings);
    w->addDriver("MifareReader", opts, tag_method_reader);

    porters.push_back(w);

    tags["reader1"]->appendFunc("doOn", w.data(), "exec");
    tags["reader1"]->appendArgument("doOn", "doOn");

    tags["reader1"]->appendFunc("doOff", w.data(), "exec");
    tags["reader1"]->appendArgument("doOff", "doOff");

    tags["reader1"]->appendFunc("doSound", w.data(), "exec");
    tags["reader1"]->appendArgument("doSound", "doSound");
    tags["reader1"]->appendArgument("doSound", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    tags["reader1"]->appendFunc("activateIdleA", w.data(), "exec");
    tags["reader1"]->appendArgument("activateIdleA", "activateIdleA");

    tags["reader1"]->appendFunc("getHostCodedKey", w.data(), "exec");
    tags["reader1"]->appendArgument("getHostCodedKey", "getHostCodedKey");
    tags["reader1"]->appendArgument("getHostCodedKey", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    tags["reader1"]->appendFunc("doAuth", w.data(), "exec");
    tags["reader1"]->appendArgument("doAuth", "doAuth");
    tags["reader1"]->appendArgument("doAuth", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    tags["reader1"]->appendFunc("readBlock", w.data(), "exec");
    tags["reader1"]->appendArgument("readBlock", "readBlock");
    tags["reader1"]->appendArgument("readBlock", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    tags["reader1"]->appendFunc("writeBlock", w.data(), "exec");
    tags["reader1"]->appendArgument("writeBlock", "writeBlock");
    tags["reader1"]->appendArgument("writeBlock", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));
    tags["reader1"]->appendArgument("writeBlock", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(1)));

}

void AppSettings::initWeight(QVector<Porter::Pointer>& porters, Tags & tags)
{
    QMap <QString, QVariant> serial_settings;

    serial_settings["baudRate"]    = BAUD9600;
    serial_settings["dataBits"]    = DATA_8;
    serial_settings["flowControl"] = FLOW_OFF;
    serial_settings["parity"]      = PAR_NONE;
    serial_settings["stopBits"]    = STOP_1;
    serial_settings["timeout"]     = 100;
    serial_settings["portName"]    = "COM1" ;//MrwSettings::instance()->platformaWeightPort[0];

    QList<TagMethod> tag_method_weight;
    tag_method_weight.append( TagMethod("weight1_1", "readWeight") );

    Porter::Pointer w = Porter::Pointer(new Porter(true));

    QMap<QString, QVariant> opts;
    opts["address"] = 31;//MrwSettings::instance()->platformaWeightAddress[0];
    opts["method"] = "readMethod";

    w->setDevice("IoDeviceSerial", serial_settings);
    w->addDriver("Hbm2110", opts, tag_method_weight);

    porters.push_back(w);
    tags["weight1_1"]->appendFunc("readMethod", w.data(), "value");

}

void AppSettings::initDiDo(QVector<Porter::Pointer>& porters, Tags& tags)
{
    QList<TagMethod> tag_method_dido;
    tag_method_dido.append(TagMethod("dido", "getDi"));
    tag_method_dido.append(TagMethod("do", "setDo"));

    tag_method_dido.append(TagMethod("di1", "getDiBit"));
    tag_method_dido.append(TagMethod("di2", "getDiBit"));
    tag_method_dido.append(TagMethod("di3", "getDiBit"));
    tag_method_dido.append(TagMethod("di4", "getDiBit"));

    tag_method_dido.append(TagMethod("do1_tmp", "setDoBit"));
    tag_method_dido.append(TagMethod("do2_tmp", "setDoBit"));
    tag_method_dido.append(TagMethod("do3_tmp", "setDoBit"));
    tag_method_dido.append(TagMethod("do4_tmp", "setDoBit"));


    QMap<QString, QVariant> dido_port_settings;
//    dido_port_settings["fileName"] = "dido.bin";

    Porter::Pointer p = Porter::Pointer(new Porter(true));

    p->setScheduled(false);
    p->setDevice("IsmmDevice", dido_port_settings);
    p->addDriver("DidoFile", QMap<QString, QVariant>(), tag_method_dido);

    porters.push_back(p);

    tags["dido"]->appendFunc("readMethod" , p.data(), "value");

    tags["di1"]->appendFunc("readMethod", p.data(), "value");
    tags["di1"]->appendArgument("readMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
    tags["di1"]->appendArgument("readMethod", 0);

    tags["di2"]->appendFunc("readMethod", p.data(), "value");
    tags["di2"]->appendArgument("readMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
    tags["di2"]->appendArgument("readMethod", 1);

    tags["di3"]->appendFunc("readMethod", p.data(), "value");
    tags["di3"]->appendArgument("readMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
    tags["di3"]->appendArgument("readMethod", 2);

    tags["di4"]->appendFunc("readMethod", p.data(), "value");
    tags["di4"]->appendArgument("readMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
    tags["di4"]->appendArgument("readMethod", 3);

    tags["do1_tmp"]->appendFunc("writeMethod", p.data(), "value");
    tags["do1_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
    tags["do1_tmp"]->appendArgument("writeMethod", 0);
    tags["do1_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    tags["do2_tmp"]->appendFunc("writeMethod", p.data(), "value");
    tags["do2_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
    tags["do2_tmp"]->appendArgument("writeMethod", 1);
    tags["do2_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    tags["do3_tmp"]->appendFunc("writeMethod", p.data(), "value");
    tags["do3_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
    tags["do3_tmp"]->appendArgument("writeMethod", 2);
    tags["do3_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    tags["do4_tmp"]->appendFunc("writeMethod", p.data(), "value");
    tags["do4_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["dido"], "readMethod")));
    tags["do4_tmp"]->appendArgument("writeMethod", 3);
    tags["do4_tmp"]->appendArgument("writeMethod", QVariant::fromValue<TagPlaceholder>(TagPlaceholder(0)));

    tags["do1"]->appendFunc("writeMethod", p.data(), "value");
    tags["do1"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do1_tmp"], "writeMethod")));

    tags["do2"]->appendFunc("writeMethod", p.data(), "value");
    tags["do2"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do2_tmp"], "writeMethod")));

    tags["do3"]->appendFunc("writeMethod", p.data(), "value");
    tags["do3"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do3_tmp"], "writeMethod")));

    tags["do4"]->appendFunc("writeMethod", p.data(), "value");
    tags["do4"]->appendArgument("writeMethod", QVariant::fromValue<TagBindable>(TagBindable(tags["do4_tmp"], "writeMethod")));
}

#endif

