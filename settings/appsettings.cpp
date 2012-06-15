#include "appsettings.h"
#include "tags.h"



#include <QList>
#include <QVariantMap>
#include <QFile>
#include <QStringList>






void AppSettings::openDocument()
{
    if (!document.isNull()) return;

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

QVariant AppSettings::convertToType(const QDomNode &value_node) const
{
    const QString type_name = value_node.nodeName();

    if ( type_name == "QVariantList"  ) {
        //qDebug() << "LIST!!!!!!!!!!!!!!\n";

        QVariantList list;

        QDomNode list_node = value_node.firstChild();
        while(!list_node.isNull()) {
            list.push_back( convertToType(list_node) );

            list_node = list_node.nextSibling();
        }
        return list;
    }
    else if ( type_name == "QVariantMap") {

        //qDebug() << "MAP!!!!!!!!!!!!!!!!\n";

        QVariantMap map;
        //QDomNode map_node = value_node.firstChild();
        fillVariantMap(map, value_node);

        return map;
    }


    const QString value = simpleValueFromPropertyNode(value_node);//par_node.firstChild().nodeValue();

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

void AppSettings::fillVariantMap(QVariantMap & map, const QDomNode & node) const
{
/*    QDomNode prop_node = node.firstChild();
    while (!prop_node.isNull()) {
        QDomElement prop_elem = prop_node.toElement();

        map[ prop_elem.attribute("name") ] = convertToType( prop_elem.firstChild() ) ;

        prop_node = prop_node.nextSibling();
    }*/
    map = getDynamicSettings(node);
}

void AppSettings::initProgOptions(QVariantMap & opts)
{
    openDocument();


    fillVariantMap( opts, findSettingsElement("app") );
}


QVariantMap AppSettings::getDynamicSettings( const QDomNode& par_node) const
{
    QDomNode n = par_node.firstChild();
    QVariantMap ret;
    while ( !n.isNull() ) {
        if (n.nodeName() == "property") {
            QDomElement el = n.toElement();

            ret[el.attribute("name")] = convertToType( n.firstChild() );
        }
        n = n.nextSibling();
    }
    return ret;
}



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

