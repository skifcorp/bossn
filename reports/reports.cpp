
#include "reports.h"
#include "func.h"

#include <QtUiTools>
#include <QtScript>
#include <QWidget>
#include <QList>
#include <QLabel>
#include <QPrinter>

bool Reports::print(const QVariantMap & m)
{
    if (file_name.isEmpty()) {
        qWarning() << "reports file_name is empty!"; return false;
    }

    QScriptEngine engine;
    for ( auto iter = m.begin(); iter != m.end(); ++iter  ) {
        engine.globalObject().setProperty( iter.key(), engine.newVariant( *iter )) ;
    }


    QUiLoader uil;

    QFile f(file_name);
    f.open(QFile::ReadOnly);

    QWidget * report = uil.load(&f, 0);


    QList<QLabel*> ll = report->findChildren<QLabel *>();

    for ( QLabel * l : ll ) {
        QString val = engine.evaluate(l->text()).toString();

        if ( engine.hasUncaughtException() ) {
            printOnDisplay("text: " + l->text() + " has exception: " + engine.uncaughtException().toString() );
            return false;
        }
        else {
            l->setText(val);
        }


    }

    report->show();
    //QPrinter printer;

    //report->render(&printer);

    //printer.print

    return true;
}
