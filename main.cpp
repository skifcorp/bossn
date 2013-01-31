
#include <QApplication>
#include <QVector>
#include <QSharedPointer>
#include <QTranslator>

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

#include <QPrinter>

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
    {
        QPrinter p;
    }
    //printOnDisplay("Hello");

//CreateDC(0, L"Microsoft XPS Document Writer", 0, 0);

    const QString settings_file_name = "proj/settings.xml";

    TaskSettings app_settings(settings_file_name);

//CreateDC(0, L"Microsoft XPS Document Writer", 0, 0);

    QMap<QString, QVariant> options;
    app_settings.initProgOptions(options);

//CreateDC(0, L"Microsoft XPS Document Writer", 0, 0);

    Tags tags;
    app_settings.initTags(tags);

//CreateDC(0, L"Microsoft XPS Document Writer", 0, 0);

    QVector<Porter::Pointer> porters;
    app_settings.initPorters(porters, tags);

//CreateDC(0, L"Microsoft XPS Document Writer", 0, 0);

    TaskExec task_exec;
    app_settings.initTasks(task_exec, tags, options);

//CreateDC(0, L"Microsoft XPS Document Writer", 0, 0);

    QVector<AlhoSequence::Pointer> alhos;
    app_settings.initAlhos(alhos, tags, options);

//CreateDC(0, L"Microsoft XPS Document Writer", 0, 0);

    return app.exec();

}



