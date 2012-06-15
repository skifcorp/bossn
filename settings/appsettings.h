#ifndef INITSETTINGS_H
#define INITSETTINGS_H


#include <QVariant>
#include <QDomDocument>
#include <QList>


class AppSettings
{
public:
    AppSettings(const QString& fn):settings_file_name(fn){}
    virtual ~AppSettings(){}


    void initProgOptions(QVariantMap & opts);


protected:
    void openDocument();

    inline QString simpleValueFromPropertyNode(const QDomNode& value_node) const
    {
        return value_node.firstChild().nodeValue();
    }
    void fillVariantMap(QVariantMap& , const QDomNode& ) const;

    QDomElement findSettingsElement ( const QString& ) const;
    QVariant convertToType( const QDomNode& value_node) const; //const QString&, const QString&) const;
    QVariantMap getDynamicSettings( const QDomNode& ) const;

    QDomDocument document;

    QString settings_file_name;
};

#endif // INITSETTINGS_H
