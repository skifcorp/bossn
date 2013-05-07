
#include <QApplication>
#include <QVector>
#include <QSharedPointer>
#include <QTranslator>

#include "rdb_pch.h"

#include "func.h"

#include "porter.h"
#include "qextserialport.h"
#include "tags.h"
#include "basetask.h"
#include "taskexec.h"
#include "perimeter.h"
#include "mainsequence.h"
#include "formatersequence.h"
#include "alhosequence.h"
#include "tasksettings.h"



#include "constructweighters.h"

int main(int argc, char *argv[])
{  
    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));


    QTranslator translator;
    if ( !translator.load("proj/bossn_ua") )  {
        qWarning() << "cant load translator!";
    }
    else {
        qDebug() << "translator loaded! is it empty: " << translator.isEmpty();
    }

    app.installTranslator(&translator);

    //printOnDisplay("Hello");

    const QString settings_file_name = "proj/settings.xml";

    TaskSettings app_settings(settings_file_name);

    QMap<QString, QVariant> options;
    app_settings.initProgOptions(options);

    Tags tags;
    app_settings.initTags(tags);

    QVector<Porter::Pointer> porters;
    app_settings.initPorters(porters, tags);

    TaskExec task_exec;
    app_settings.initTasks(task_exec, tags, options);

    QVector<AlhoSequence::Pointer> alhos;
    app_settings.initAlhos(alhos, tags, options);

    return app.exec();
}



