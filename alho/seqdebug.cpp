
//#include "rdb_pch.h"

#include "seqdebug.h"
#include "mainsequencebaseop.h"
#include "func.h"

SeqDebug::~SeqDebug()
{
    operator <<("platform: ").operator << (seq.seqId()).operator <<(" ");


    QString print_string = QDateTime::currentDateTimeUtc().toString("yyyy-mm-dd hh:mm:ss") + " " + QString::number(seq.seqId()).leftJustified(5) +
            QString::number(error_code).leftJustified(5) + " " + buffer;

    if ( use_database ) {
        seq.alhoSettings().logging.func(Q_ARG(const QVariant&, QVariant(seq.seqId())) ,
                                        Q_ARG(const QVariant&, QVariant(error_code) ),
                                        Q_ARG(const QVariant&, QVariant(buffer)));

        if ( to_console ) {
            printOnDisplay( print_string );
        }
    }
    else {
        if (to_console)
            qDebug() << print_string;
    }
}
