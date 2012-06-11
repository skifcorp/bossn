
#include <QApplication>
#include <QVector>
#include <QSharedPointer>

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





void initTasks(TaskExec & tasks, Tags & tags, MainSequence & seq )
{


    PerimeterTask::Pointer perim (new PerimeterTask(tags));
    QMap<QString, QVariant> perim_settings;
    perim_settings["PerimeterType"] = "PerimeterControlByWeight";
    perim_settings["weightName"]    = "weight1_1";
    perim_settings["minWeight"]     = 50;
    perim_settings["method"]        = "readMethod";



    /*perim_settings["PerimeterType"] = "PerimeterControlByDi";
    perim_settings["AppearDi"]      = "di1";
    perim_settings["DisappearDi"]   = "di2";
    perim_settings["method"]        = "readMethod";*/


    //QObject::connect(perim.data(), SIGNAL(appeared()), &seq, SLOT(onAppearOnWeight()), Qt::QueuedConnection );
    //QObject::connect(perim.data(), SIGNAL(disappeared()), &seq, SLOT(onDisappearOnWeight()), Qt::QueuedConnection);


    perim->setSettings(perim_settings);

    tasks.addTask(500, perim.staticCast<BaseTask::Pointer::Type>());
}


int main(int argc, char *argv[])
{  
    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    printOnDisplay("Hello");

    const QString settings_file_name = "settings.xml";

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



