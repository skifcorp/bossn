#ifndef INITSETTINGS_H
#define INITSETTINGS_H


#include <QVariant>
#include <QDomDocument>
#include <QList>

class AppSettings
{
public:
    AppSettings(){}
    virtual ~AppSettings(){}


    void initProgOptions(QVariantMap & opts);

protected:
    void openDocument();
    QDomElement findSettingsElement ( const QString& ) const;
    QVariant convertToType( const QString&, const QString&) const;
    QVariantMap getDynamicSettings( const QDomNode& ) const;

    QDomDocument document;

    static const QString settings_file_name;
};

#endif // INITSETTINGS_H
