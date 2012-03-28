#include "perimeter.h"
#include "settingstool.h"


PerimeterTask::PerimeterTask(Tags & t):tags(t)
{

}

void PerimeterTask::setSettings ( const QMap<QString, QVariant>& s )
{
    QString type;


    if (!get_setting("PerimeterType", s, type)) return;

    perim = PerimeterControl::create(type, tags);

    perim->setSettings(s);

}

void PerimeterTask::exec()
{
    if (!perim) return ;

    if (perim->appeared()) emit(appeared());

    if (perim->disappeared()) emit (disappeared());

}



