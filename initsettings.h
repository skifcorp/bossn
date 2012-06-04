#ifndef INITSETTINGS_H
#define INITSETTINGS_H


#include <QVariant>
#include <QDomDocument>
#include <QList>

#ifndef PROG_OPTIONS_ONLY
#include "porter.h"
#include "tags.h"
#endif

class AppSettings
{
public:
    AppSettings(){}
    ~AppSettings(){}


    void initProgOptions(QVariantMap & opts);
#ifndef PROG_OPTIONS_ONLY
    void initPorters(QVector<Porter::Pointer>& porters, Tags &);
    void initTags(Tags &);
#endif
private:
    void openDocument();
    QDomElement findSettingsElement ( const QString& ) const;
    QVariant convertToType( const QString&, const QString&
#ifndef PROG_OPTIONS_ONLY
, Tags * t = nullptr
#endif
    ) const;
   QVariantMap getDynamicSettings( const QDomNode& ) const;

#ifndef PROG_OPTIONS_ONLY
    void initPorterDrivers(Porter::Pointer, const QDomNode& porter_node, Tags& tags) const;
    QList<TagMethod> getTagMethods(const QDomNode& driver_node, const Tags& ) const;
    void bindTags( const QDomNode&, Tags&, Porter::Pointer ) const;
#endif

    QDomDocument document;

    static const QString settings_file_name;
};

#endif // INITSETTINGS_H
