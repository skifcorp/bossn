#include "stable.h"
#include "settingstool.h"


template <class F>
struct variant_op
{
    QVariant operator()( const QVariant& v1, const QVariant& v2)
    {
        F f;
        if ( v1.type() == QVariant::Int && v2.type() == QVariant::Int ) {
            return f(v1.toInt(), v2.toInt());
        }
        else if ( v1.type() == QVariant::UInt && v2.type() == QVariant::UInt ) {
            return f( v1.toUInt(),  v2.toUInt());
        }
        else if ( v1.type() == QVariant::Double && v2.type() == QVariant::Double ) {
            return f(v1.toDouble(), v2.toDouble());
        }

        qWarning() << "variant_op with args: " << v1 << " and "<<v2<<" not supported";

        return QVariant(0);
    }
};


struct minus_op
{
    template <class T>
    QVariant operator() (const T& t1, const T & t2)
    {
        return t1 - t2;
    }
};

struct greater_op
{
    template <class T>
    QVariant operator()(const T& t1, const T & t2)
    {
        return t1 > t2;
    }
};

struct smaller_op
{
    template <class T>
    QVariant operator()(const T& t1, const T & t2)
    {
        return t1 < t2;
    }
};

typedef variant_op<minus_op>   count_delta_op;
typedef variant_op<greater_op> grater_than_op;
typedef variant_op<smaller_op> smaller_than_op;

count_delta_op  count_delta;
grater_than_op  greater_than;
smaller_than_op smaller_than;

BossnFactoryRegistrator<StableTask> StableTask::registrator("StableTask");

void StableTask::run()
{

    is_busy = true;

    QVariant v = tags[controlled_tag_name]->func(controlled_tag_func, this);

    if ( !v.isValid() ) {
        //qDebug() << "stable not valid!!!";
        is_stable = false;
        is_busy = false;
        return;
    }

    values.enqueue( v );

    if (values.size() < 2 ) {is_stable = false; is_busy = false; return;}

    QVariant min_val = values.first() , max_val = values.first();

    for ( int i = 0; i<values.count(); ++i) {
        QVariant val = values[i];
        if ( greater_than(min_val, val).toBool() ) min_val = val;
        if ( smaller_than(max_val, val).toBool() ) max_val = val;
    }

    is_stable = greater_than( delta, count_delta(max_val, min_val) ).toBool();

    is_busy = false;
}


void StableTask::setSettings(const QVariantMap & s)
{
    controlled_tag_name = get_setting<QString>("control_tag", s);
    controlled_tag_func = get_setting<QString>("control_func", s);

    values.setMaxSize( get_setting<int>("values_num", s) );

    if ( values.maxSize() < 2 ) {
        qWarning() << "size of FixedQueue must be at least two!!!";
        qFatal("exiting!");
    }

    delta  = get_setting<QVariant>("delta", s);
}

bool StableTask::busy() const
{
    return is_busy;
}




