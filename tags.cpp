#include "tags.h"
#include <QDebug>

int __id = qRegisterMetaType<Tag::WeakPointer>("Tag::WeakPointer");

QVariant Tag::arg(int n)
{
    if (args[n].userType() == __id) {
        //need get tag value
        QVariant ret = args[n].value<Tag::WeakPointer>().data()->value();
        return ret;
    }
    return args[n];
}

QVariant Tag::value (QGenericArgument val0, QGenericArgument val1, QGenericArgument val2,
               QGenericArgument val3, QGenericArgument val4, QGenericArgument val5,
               QGenericArgument val6, QGenericArgument val7 )
{
    QVariant ret;

    switch (args.count()) {
    case 0:
        QMetaObject::invokeMethod( read_object, read_method.toAscii().data(),
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
        QMetaObject::invokeMethod( read_object, read_method.toAscii().data(),
                                   Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(0))) ,
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2),
                                   Q_ARG(QGenericArgument, val3),
                                   Q_ARG(QGenericArgument, val4),
                                   Q_ARG(QGenericArgument, val5),
                                   Q_ARG(QGenericArgument, val6));
        break;

    case 2:
        QMetaObject::invokeMethod( read_object, read_method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(1))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2),
                                   Q_ARG(QGenericArgument, val3),
                                   Q_ARG(QGenericArgument, val4),
                                   Q_ARG(QGenericArgument, val5));
        break;

    case 3:
        QMetaObject::invokeMethod( read_object, read_method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(2))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2),
                                   Q_ARG(QGenericArgument, val3),
                                   Q_ARG(QGenericArgument, val4));
        break;

    case 4:
        QMetaObject::invokeMethod( read_object, read_method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(3))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2),
                                   Q_ARG(QGenericArgument, val3));
        break;

    case 5:
        QMetaObject::invokeMethod( read_object, read_method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(3))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(4))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1),
                                   Q_ARG(QGenericArgument, val2));
        break;

    case 6:
        QMetaObject::invokeMethod( read_object, read_method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(3))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(4))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(5))),
                                   Q_ARG(QGenericArgument, val0),
                                   Q_ARG(QGenericArgument, val1));
        break;

    case 7:
        QMetaObject::invokeMethod( read_object, read_method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(3))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(4))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(5))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(6))),
                                   Q_ARG(QGenericArgument, val0));
        break;

    case 8:
        QMetaObject::invokeMethod( read_object, read_method.toAscii().data(), Q_RETURN_ARG(QVariant,ret),
                                   Q_ARG(QString, tag_name),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(0))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(1))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(2))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(3))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(4))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(5))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(6))),
                                   Q_ARG(QGenericArgument, Q_ARG(QVariant, arg(7))));
        break;

    };

    return ret;
}

