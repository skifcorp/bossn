#include "tags.h"
#include <QDebug>

int __id_bindable      = qRegisterMetaType<TagBindable>("TagBindable");
int __id_placeholder   = qRegisterMetaType<TagPlaceholder>("TagPlaceholder");
int __id_alho_sequence = qRegisterMetaType<AlhoSequence*>("AlhoSeqence*");

void Tag::appendArgument(const QString &fn, const QVariant &arg)
{
    auto iter = funcs.find(fn);
    if (iter == funcs.end()) {
        qWarning() << "Tag::appendArgunment: cant find func " << fn <<" in arg method!!!"; return;
    }

    QVariant arg1 = arg;  

    iter->args.append(arg1);
}

QVariant Tag::call_as_func(FuncContext& func, AlhoSequence * caller, const QList<QGenericArgument>& args)
{
    QVariant ret;
    bool invoke_ret = false;

    switch (args.count()) {
    case 0:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type, Q_RETURN_ARG(QVariant,ret),
                                                Q_ARG(QString, tag_name) ,
                                                Q_ARG(AlhoSequence *, caller));
        break;
    case 1:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(), func.conn_type, Q_RETURN_ARG(QVariant,ret),
                                                Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                   Q_ARG(QGenericArgument, args[0]) );
        break;

    case 2:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                   func.conn_type, Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                    Q_ARG(AlhoSequence *, caller),
                                   Q_ARG(QGenericArgument, args[0]),
                                   Q_ARG(QGenericArgument, args[1]));
        break;
    case 3:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                   func.conn_type, Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(AlhoSequence *, caller),
                                   Q_ARG(QGenericArgument, args[0]),
                                   Q_ARG(QGenericArgument, args[1]),
                                   Q_ARG(QGenericArgument, args[2]));
        break;
    case 4:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type, Q_RETURN_ARG(QVariant,ret),
                                                Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]));
        break;
    case 5:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type, Q_RETURN_ARG(QVariant,ret),
                                                Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]),
                                                Q_ARG(QGenericArgument, args[4]));
        break;
    case 6:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type, Q_RETURN_ARG(QVariant,ret),
                                                Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]),
                                                Q_ARG(QGenericArgument, args[4]),
                                                Q_ARG(QGenericArgument, args[5]));
        break;

    };

    if (!invoke_ret) {
        qWarning()<<"Tag::func: function "<<func.method <<" failed to invoke!!";
    }
    return ret;
}


void Tag::call_as_proc(FuncContext& func, AlhoSequence * caller, const QList<QGenericArgument>& args)
{
    bool invoke_ret = false;

    switch (args.count()) {
    case 0:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type, Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller) );
        break;
    case 1:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type,  Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                   Q_ARG(QGenericArgument, args[0]) );
        break;

    case 2:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type, Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                   Q_ARG(QGenericArgument, args[0]),
                                   Q_ARG(QGenericArgument, args[1]));
        break;
    case 3:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type,  Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                   Q_ARG(QGenericArgument, args[0]),
                                   Q_ARG(QGenericArgument, args[1]),
                                   Q_ARG(QGenericArgument, args[2]));
        break;
    case 4:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type,  Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]));
        break;
    case 5:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type,  Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]),
                                                Q_ARG(QGenericArgument, args[4]));
        break;
    case 6:
        invoke_ret = QMetaObject::invokeMethod( func.object, func.method.toAscii().data(),
                                                func.conn_type,  Q_ARG(QString, tag_name),
                                                Q_ARG(AlhoSequence *, caller),
                                                Q_ARG(QGenericArgument, args[0]),
                                                Q_ARG(QGenericArgument, args[1]),
                                                Q_ARG(QGenericArgument, args[2]),
                                                Q_ARG(QGenericArgument, args[3]),
                                                Q_ARG(QGenericArgument, args[4]),
                                                Q_ARG(QGenericArgument, args[5]));
        break;

    };

    if (!invoke_ret) {
        qWarning()<<"Tag::func: function "<<func.method <<" failed to invoke!!";
    }
}


QVariant Tag::execObject(FuncContext& func, AlhoSequence * caller, const QList<QGenericArgument>& external_args)
{
    QList<QGenericArgument> args;

    for (QVariantList::size_type i = 0 ; i < func.args.count(); ++i ) {
        if ( func.args[i].userType() == __id_bindable ) {
            TagBindable tg = func.args[i].value<TagBindable>();
            args.append( Q_ARG(QVariant, tg.tag.data()->func(tg.method, caller, external_args) ) );
        }
        else if ( func.args[i].userType() == __id_placeholder ){
            args.append(external_args[func.args[i].value<TagPlaceholder>().arg_num] );
        }
        else {
            args.append( Q_ARG(QVariant, func.args[i]) );
        }
    }

    if ( func.conn_type == Qt::QueuedConnection) {
        call_as_proc( func, caller, args );
        return QVariant();
    }

    return call_as_func( func, caller, args );
}

QVariant Tag::func (const QString& func_name, AlhoSequence* caller, const QList<QGenericArgument>& external_args)
{
    auto iter = funcs.find(func_name);

    if (iter == funcs.end()) {
        qWarning()<<"Tag::func: Cant find func: " <<func_name; return QVariant();
    }

    return execObject(*iter, caller, external_args);
}

QVariant Tag::func (const QString& func_name, AlhoSequence* caller,
                    QGenericArgument val0, QGenericArgument val1,
                    QGenericArgument val2, QGenericArgument val3,
                    QGenericArgument val4,  QGenericArgument val5 )
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
    //passed_args.append(val6);
    \

    return execObject(*iter, caller, passed_args);
}

