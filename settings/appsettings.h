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
    QDomElement findSettingsElement ( const QString& ) const;
    QVariant convertToType( const QString&, const QString&) const;
    QVariantMap getDynamicSettings( const QDomNode& ) const;

    QDomDocument document;

   QString settings_file_name;
};

#endif // INITSETTINGS_H
