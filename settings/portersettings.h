#ifndef PORTERSETTINGS_H
#define PORTERSETTINGS_H

#include "appsettings.h"
#include "porter.h"
#include "tags.h"
#include "taskexec.h"

class PorterSettings : public AppSettings
{
public:
    PorterSettings(const QString& fn) : AppSettings(fn) {}
    void initPorters(QVector<Porter::Pointer>& porters, Tags &);
    void initTags(Tags &);
    void initTasks(TaskExec& , Tags&, const QVariantMap& );
protected:
    void initPorterDrivers(Porter::Pointer, const QDomNode& porter_node, Tags& tags) const;
    QList<TagMethod<TagMethodType::Simple>> getTagMethods(const QDomNode& driver_node, const Tags& ) const;
    void bindTags( const QDomNode&, Tags&, QObject * ) const;
    QVariant convertToType( const QDomNode& , Tags * t = nullptr) const;
};


#endif // PORTERSETTINGS_H
