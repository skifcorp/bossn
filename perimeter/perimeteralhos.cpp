#include "perimeteralhos.h"
#include "settingstool.h"

#if 0
BossnFactoryRegistrator<PerimeterControlByDi> PerimeterControlByDi::registator("PerimeterControlByDi");



void PerimeterControlByDi::setSettings( const QMap<QString, QVariant>& s)
{

    appear_di_name    = get_setting<QString>("AppearDi"   , s);
    disappear_di_name = get_setting<QString>("DisappearDi", s);
    method            = get_setting<QString>("method"     , s);

    appear_di_value    = get_setting("AppearDiValue"     , s, true);
    disappear_di_value = get_setting("DisappearDiValue"  , s, true);

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
    //qDebug() << "disappear val:"<<tags_[disappear_di_name]->func(method);

    bool ret = tags_[disappear_di_name]->func(method) == disappear_di_value && prev_disappear_di != disappear_di_value;
    prev_disappear_di = tags_[disappear_di_name]->func(method).toBool();
    return ret;
}


#endif

BossnFactoryRegistrator<PerimeterControlByWeight> PerimeterControlByWeight::registator("PerimeterControlByWeight");


void PerimeterControlByWeight::setSettings( const QMap<QString, QVariant>& s)
{
    weight_name = get_setting<QString>("weightName", s);
    min_weight  = get_setting<float>  ("minWeight" , s);
    get_weight_method      = get_setting<QString>("method"    , s);
}

bool PerimeterControlByWeight::appeared(AlhoSequence * caller)
{
    bool ret = false;

    QVariant val = tags_[weight_name]->func(get_weight_method, caller);

    if ( !val.isValid() ) return false;

    if ( val.toInt() > min_weight && !was_appeared) {
        was_appeared = true;
        ret = true;
    }

    return ret;
}

bool PerimeterControlByWeight::disappeared(AlhoSequence * caller)
{
    bool ret = false;

    QVariant val = tags_[weight_name]->func(get_weight_method, caller);

    if (!val.isValid()) return false;

    if ( val.toInt() < min_weight && was_appeared) {
        was_appeared = false;
        ret = true;
    }

    return ret;
}





