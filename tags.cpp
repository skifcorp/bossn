#include "tags.h"
#include <QDebug>

int __id_bindable    = qRegisterMetaType<TagBindable>("TagBindable");
int __id_placeholder = qRegisterMetaType<TagPlaceholder>("TagPlaceholder");

void Tag::appendArgument(const QString &fn, const QVariant &arg)
{
    auto iter = funcs.find(fn);
    if (iter == funcs.end()) {
        qWarning() << "Tag::appendArgunment: cant find func " << fn <<" in arg method!!!"; return;
    }

    QVariant arg1 = arg;

    /*if (arg.userType() == __id_placeholder) {
        TagPlaceholder ph = arg.value<TagPlaceholder>();
        if (ph.tag.isNull() ){
            ph.tag = sharedFromThis();
            arg1 = QVariant::fromValue<TagPlaceholder>(ph);
        }
    }*/

    iter->args.append(arg1);
}
/*
QVariant Tag::arg(const QString& fn, int n)
{
    auto iter = funcs.find(fn);
    if (iter == funcs.end()) {
        qWarning() << "tag:arg: cant find func " << fn <<" in arg method!!!";
        return QVariant();
    }

    if (iter->args[n].userType() == __id_bindable) {
        //need get tag value
        QVariant ret = iter->args[n].value<TagBindable>().execute();//->func("readMethod");
                //iter->args[n].value<Tag::WeakPointer>().data()->func("readMethod");

        return ret;
    }

    return iter->args[n];
}
*/

QVariant Tag::execObject(FuncContext& func, const QList<QGenericArgument>& external_args)
{
    QList<QGenericArgument> args;

    for (QVariantList::size_type i = 0 ; i < func.args.count(); ++i ) {
        if ( func.args[i].userType() == __id_bindable ) {
            TagBindable tg = func.args[i].value<TagBindable>();
            args.append( Q_ARG(QVariant, tg.tag.data()->func(tg.method, external_args) ) );
        }
        else if ( func.args[i].userType() == __id_placeholder ){
            args.append(external_args[func.args[i].value<TagPlaceholder>().arg_num] );
        }
        else {
            args.append( Q_ARG(QVariant, func.args[i]) );
        }
    }

    QVariant ret;
    bool invoke_ret = false;

    switch (args.count()) {
    case 0:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(), Q_RETURN_ARG(QVariant,ret), Q_ARG(QString, tag_name) );
        break;
    case 1:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(), Q_RETURN_ARG(QVariant,ret), Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, args[0]) );
        break;

    case 2:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(), Q_RETURN_ARG(QVariant,ret), Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, args[0]),
                                   Q_ARG(QGenericArgument, args[1]));
        break;
    case 3:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(), Q_RETURN_ARG(QVariant,ret), Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, args[0]),
                                   Q_ARG(QGenericArgument, args[1]),
                                   Q_ARG(QGenericArgument, args[2]));
        break;
    case 4:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(), Q_RETURN_ARG(QVariant,ret), Q_ARG(QString, tag_name),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]));
        break;
    case 5:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(), Q_RETURN_ARG(QVariant,ret), Q_ARG(QString, tag_name),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]),
                                                Q_ARG(QGenericArgument, args[4]));
        break;
    case 6:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(), Q_RETURN_ARG(QVariant,ret), Q_ARG(QString, tag_name),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]),
                                                Q_ARG(QGenericArgument, args[4]),
                                                Q_ARG(QGenericArgument, args[5]));
        break;
    case 7:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(), Q_RETURN_ARG(QVariant,ret), Q_ARG(QString, tag_name),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]),
                                                Q_ARG(QGenericArgument, args[4]),
                                                Q_ARG(QGenericArgument, args[5]),
                                                Q_ARG(QGenericArgument, args[6]));
        break;
    };

    if (!invoke_ret) {
        qWarning()<<"Tag::func: function "<<func.method <<" failed to invoke!!";
    }

    return ret;
}

QVariant Tag::func (const QString& func_name, const QList<QGenericArgument>& external_args)
{
    auto iter = funcs.find(func_name);

    if (iter == funcs.end()) {
        qWarning()<<"Tag::func: Cant find func: " <<func_name; return QVariant();
    }

    return execObject(*iter, external_args);
}

QVariant Tag::func (const QString& func_name, QGenericArgument val0,
                                              QGenericArgument val1,
                                              QGenericArgument val2,
                                              QGenericArgument val3,
                                              QGenericArgument val4,
                                              QGenericArgument val5,
                                              QGenericArgument val6,
                                              QGenericArgument val7 )
{
    auto iter = funcs.find(func_name);

    if (iter == funcs.end()) {
        qWarning()<<"Tag::func: Cant find func: " <<func_name; return QVariant();
    }

    QList<QGenericArgument> passed_args;
    passed_args.append(val0);
    passed_args.append(val1);
    passed_args.append(val2);
    passed_args.append(val3);
    passed_args.append(val4);
    passed_args.append(val5);
    passed_args.append(val6);
    passed_args.append(val7);

    return execObject(*iter, passed_args);


 /*
    QVariant ret;
    bool invoke_ret = false;

    switch (iter->args.count()) {
    case 0:

        invoke_ret = QMetaObject::invokeMethod( iter->object, iter->method.toAscii().data(),
                                   Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2),
                                   Q_ARG(QGenericArgument, val3),
                                   Q_ARG(QGenericArgument, val4),
                                   Q_ARG(QGenericArgument, val5),
                                   Q_ARG(QGenericArgument, val6),
                                   Q_ARG(QGenericArgument, val7) );
        break;
    case 1:
        qDebug() << "111111111111";
        invoke_ret = QMetaObject::invokeMethod( iter->object, iter->method.toAscii().data(),
                                   Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 0))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2),
                                   Q_ARG(QGenericArgument, val3),
                                   Q_ARG(QGenericArgument, val4),
                                   Q_ARG(QGenericArgument, val5),
                                   Q_ARG(QGenericArgument, val6));
        break;

    case 2:
          qDebug() << "33333333333: "<<val0.name();
        invoke_ret = QMetaObject::invokeMethod( iter->object, iter->method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 1))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2),
                                   Q_ARG(QGenericArgument, val3),
                                   Q_ARG(QGenericArgument, val4),
                                   Q_ARG(QGenericArgument, val5));
        break;

    case 3:
        qDebug() << "44444444444";
        invoke_ret = QMetaObject::invokeMethod( iter->object, iter->method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 2))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2),
                                   Q_ARG(QGenericArgument, val3),
                                   Q_ARG(QGenericArgument, val4));
        break;

    case 4:
        invoke_ret = QMetaObject::invokeMethod( iter->object, iter->method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 3))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2),
                                   Q_ARG(QGenericArgument, val3));
        break;

    case 5:
        invoke_ret = QMetaObject::invokeMethod( iter->object, iter->method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 3))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 4))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2));
        break;

    case 6:
        invoke_ret = QMetaObject::invokeMethod( iter->object, iter->method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 3))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 4))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 5))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1));
        break;

    case 7:
        invoke_ret = QMetaObject::invokeMethod( iter->object, iter->method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 3))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 4))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 5))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 6))),
                                   Q_ARG(QGenericArgument, val0));
        break;

    case 8:
        invoke_ret = QMetaObject::invokeMethod( iter->object, iter->method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 3))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 4))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 5))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 6))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(func_name, 7))));
        break;

    };

    if (!invoke_ret) {
        qWarning()<<"Tag::func: function "<<iter->method <<" failed to invoke!!";
    }

    return ret;*/
}

