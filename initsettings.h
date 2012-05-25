#ifndef INITSETTINGS_H
#define INITSETTINGS_H

//#include "porter.h"

//class Tags;
//class Porter;
//class Porter::Pointer;

#include "porter.h"
#include "tags.h"

void initTags(Tags &);
void initTablo(QVector<Porter::Pointer>& porters, Tags &);
void initReader(QVector<Porter::Pointer>& porters, Tags &);
void initWeight(QVector<Porter::Pointer>& porters, Tags &);
void initDiDo(QVector<Porter::Pointer>& porters, Tags& );
void initProgOptions(QVariantMap & opts);


#endif // INITSETTINGS_H
