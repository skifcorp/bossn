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
grater_than_op  grater_than;
smaller_than_op smaller_than;

BossnFactoryRegistrator<StableTask> StableTask::registrator("StableTask");

void StableTask::exec()
{
    QVariant v = tags[controlled_tag_name]->func(controlled_tag_func);

    if ( !v.isValid() ) {
        is_stable = false; return;
    }

    values.enqueue( v );

    if (values.size() < 2 ) {is_stable = false; return;}

    VariantFixedQueue::const_iterator iter1 , iter2;
    iter1 = iter2 = values.begin();


    ++iter1;


    is_stable = true;
    QVariant max_delta = count_delta( *iter1, *iter2 );

    for ( ; iter1 != values.end();  ++iter1, ++iter2 ) {
     //   qDebug () << "i1: "<<*iter1 << " i2: "<<*iter2;
        QVariant cur_delta = count_delta( *iter1, *iter2 );
        if ( grater_than(cur_delta, max_delta).toBool() ) {
            max_delta = cur_delta;
        }
    }


    is_stable = grater_than( delta, max_delta ).toBool();



   // qDebug () << "stable: " << is_stable << "\n";
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