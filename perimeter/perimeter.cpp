#include "perimeter.h"
#include "settingstool.h"

BossnFactoryRegistrator<PerimeterTask> PerimeterTask::registrator("PerimeterTask");

PerimeterTask::PerimeterTask(Tags & t):tags(t), is_busy(false)
{

}

void PerimeterTask::setSettings ( const QMap<QString, QVariant>& s )
{       
    QString pt = get_setting<QString>("PerimeterType", s);

    appeared_tag_name    = get_setting<QString>("AppearedTag", s);
    appeared_tag_func    = get_setting<QString>("AppearedFunc", s);
    disappeared_tag_name = get_setting<QString>("DisappearedTag", s);
    disappeared_tag_func = get_setting<QString>("DisappearedFunc", s);

    perim = PerimeterControl::create(pt, tags);
    perim->setSettings(s);
}

void PerimeterTask::run()
{

    //qDebug() << "perimeter run!!!";

    if (!perim) return ;

    is_busy = true;

    if (perim->appeared(this)) {
        //emit(appeared());
        tags[appeared_tag_name]->func(appeared_tag_func, this);
    }

    if (perim->disappeared(this)) {
        //emit (disappeared());
        tags[disappeared_tag_name]->func(disappeared_tag_func, this);
    }

    is_busy = false;
}

bool PerimeterTask::busy() const
{
     //qDebug() << "want check perimeter: "<<is_busy;
     return is_busy;
}
