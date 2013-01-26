#include "seqdebug.h"
#include "mainsequence.h"
#include "func.h"

SeqDebug::~SeqDebug()
{
    //qDebug() << "seqqqqqqqqqqqqqqq11111111111111";

    operator <<("platform: ").operator << (seq.seqId()).operator <<(" ");

  //  qDebug() << "seqqqqqqqqqqqqqqq222222222222222";

    QString print_string = QDateTime::currentDateTimeUtc().toString("yyyy-mm-dd hh:mm:ss") + " " + QString::number(seq.seqId()).leftJustified(5) +
            QString::number(error_code).leftJustified(5) + " " + buffer;
//qDebug() << "seqqqqqqqqqqqqqqq333333333333333333";
    if ( use_database ) {
/*        seq.tags[ seq.alho_settings.logging.tag_name ]->func(seq.alho_settings.logging.method_name, &seq,
                                                                    Q_ARG(const QVariant&, QVariant(seq.seq_id) ),
                                                                    Q_ARG(const QVariant&, QVariant(error_code) ),
                                                                    Q_ARG(const QVariant&, QVariant(buffer) ) );
*/
        //qDebug() << "seqqqqqqqqqqqqqqq14444444444444444";
        seq.alhoSettings().logging.func(Q_ARG(const QVariant&, QVariant(seq.seqId())) ,
                                        Q_ARG(const QVariant&, QVariant(error_code) ),
                                        Q_ARG(const QVariant&, QVariant(buffer)));
      //  qDebug() << "seqqqqqqqqqqqqqqq155555555555555555555555";
        if ( to_console ) {
            //qWarning() <<  print_string;
            printOnDisplay( print_string );
        }
    }
    else {
        if (to_console)
            qDebug() << print_string;
    }
    //qDebug() << "seqqqqqqqqqqqqqqq166666666666666666666666";
}
