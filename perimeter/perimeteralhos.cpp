#include "perimeteralhos.h"
#include "settingstool.h"


BossnFactoryRegistrator<PerimeterControlByDi> PerimeterControlByDi::registator("PerimeterControlByDi");



void PerimeterControlByDi::setSettings( const QMap<QString, QVariant>& s)
{
    if (!get_setting("AppearDi"   , s, appear_di_name )) return;
    if (!get_setting("DisappearDi", s, disappear_di_name )) return;
    if (!get_setting("method"     , s, method )) return;

    get_setting("AppearDiValue"     , s, appear_di_value, false);
    get_setting("DisappearDiValue"  , s, disappear_di_value, false);

    //initializing prev_di_values
    prev_appear_di      = tags_[appear_di_name]->func(method).toBool();
    prev_disappear_di   = tags_[disappear_di_name]->func(method).toBool();
}

bool PerimeterControlByDi::appeared()
{
    bool ret = tags_[appear_di_name]->func(method) == appear_di_value && prev_appear_di != appear_di_value;
    prev_appear_di = tags_[appear_di_name]->func(method).toBool();
    return ret;
}

bool PerimeterControlByDi::disappeared()
{
    //qDebug() << "disappear val:"<<tags_[disappear_di_name]->value();
    bool ret = tags_[disappear_di_name]->func(method) == disappear_di_value && prev_disappear_di != disappear_di_value;
    prev_disappear_di = tags_[disappear_di_name]->func(method).toBool();
    return ret;
}




BossnFactoryRegistrator<PerimeterControlByWeight> PerimeterControlByWeight::registator("PerimeterControlByWeight");


void PerimeterControlByWeight::setSettings( const QMap<QString, QVariant>& s)
{
    if (!get_setting("weightName",s, weight_name)) return;
    if (!get_setting("minWeight" ,s, min_weight)) return;
    if (!get_setting("method"     , s, method )) return;

    //initializing prev_weight

}

bool PerimeterControlByWeight::appeared()
{
    bool ret = false;

    //qDebug() << "weight_name: " <<weight_name;

    if ( tags_[weight_name]->func(method).toFloat() > min_weight && !was_appeared) {
        was_appeared = true;
        ret = true;
    }

    return ret;
}

bool PerimeterControlByWeight::disappeared()
{
    bool ret = false;

    if ( tags_[weight_name]->func(method).toFloat() < min_weight && was_appeared) {
        was_appeared = false;
        ret = true;
    }

    return ret;
}





