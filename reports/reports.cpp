#include <windows.h>

#include "reports.h"
#include "func.h"

#include <QtUiTools>
#include <QtScript>
#include <QWidget>
#include <QList>
#include <QLabel>
#include <QPrinter>
#include <QPixmap>
#include <QPainter>
#include <QPrinterInfo>


template <class T>
struct deleter
{
    T * t;
    ~deleter()
    {
        delete t;
    }
};


bool Reports::print(const QVariantMap & m)
{
     //QPrinter printer1(QPrinter::HighResolution);


     // HDC hdc = CreateDC(0, reinterpret_cast<const wchar_t *>(name.utf16()), 0, 0);

       //HDC hdc = CreateDC(0, L"Microsoft XPS Document Writer", 0, 0);


//    qDebug () << "ooooooooooooooooo: " << program <<  name;



    if (file_name.isEmpty()) {
        qWarning() << "reports file_name is empty!"; return false;
    }

    QScriptEngine engine;
    for ( auto iter = m.begin(); iter != m.end(); ++iter  ) {
        if ( iter->type() == QVariant::DateTime )
            engine.globalObject().setProperty( iter.key(), engine.newDate( iter->toDateTime() )) ;
        else
            engine.globalObject().setProperty( iter.key(), engine.newVariant( *iter )) ;
    }


    QUiLoader uil;

    QFile f(file_name);
    f.open(QFile::ReadOnly);

    QWidget * report = uil.load(&f, 0);
    deleter<QWidget> rep_guard{report};

    //report->resize( report->minimumSizeHint() );

    QList<QLabel*> ll = report->findChildren<QLabel *>();

    for ( QLabel * l : ll ) {
        QFont f = l->font();
        f.setStyleStrategy(QFont::PreferAntialias);
        l->setFont(f);

        QString val = engine.evaluate(l->text()).toString();

        if ( engine.hasUncaughtException() ) {
            printOnDisplay("text: " + l->text() + " has exception: " + engine.uncaughtException().toString() );
            return false;
        }
        else {
            l->setText(val);
        }


    }

    report->setAttribute( Qt::WA_QuitOnClose, false );
    report->show();  

    QPrinter printer(QPrinter::HighResolution);

    printer.setPrinterName(printer_name);

    if (!printer.isValid()) {
        qWarning() << "WARNING! Printer " << printer_name << " is not valid!";

        return false;
    }

    QPainter painter(&printer);
    report->render(&painter);

    return true;
}
