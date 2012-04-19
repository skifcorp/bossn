#include "perimeter.h"
#include "settingstool.h"


PerimeterTask::PerimeterTask(Tags & t):tags(t)
{

}

void PerimeterTask::setSettings ( const QMap<QString, QVariant>& s )
{       
    QString pt = get_setting<QString>("PerimeterType", s);

    perim = PerimeterControl::create(pt, tags);
    perim->setSettings(s);
}

void PerimeterTask::exec()
{
    if (!perim) return ;

    if (perim->appeared()) emit(appeared());

    if (perim->disappeared()) emit (disappeared());

}



