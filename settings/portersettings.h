#ifndef PORTERSETTINGS_H
#define PORTERSETTINGS_H

#include "appsettings.h"
#include "porter.h"
#include "tags.h"


class PorterSettings : public AppSettings
{
public:
    void initPorters(QVector<Porter::Pointer>& porters, Tags &);
    void initTags(Tags &);
protected:
    void initPorterDrivers(Porter::Pointer, const QDomNode& porter_node, Tags& tags) const;
    QList<TagMethod> getTagMethods(const QDomNode& driver_node, const Tags& ) const;
    void bindTags( const QDomNode&, Tags&, QObject * ) const;
    QVariant convertToType( const QString&, const QString&, Tags * t = nullptr) const;
};


#endif // PORTERSETTINGS_H
