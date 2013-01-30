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



bool Reports::print(const QVariantMap & m)
{
     //QPrinter printer1(QPrinter::HighResolution);


/*    QString noPrinters(QLatin1String("qt_no_printers"));
    wchar_t buffer[256];
    GetProfileString(L"windows", L"device",
                     reinterpret_cast<const wchar_t *>(noPrinters.utf16()),
                     buffer, 256);
    QString output = QString::fromWCharArray(buffer);





    qDebug () << "ooooooooooooooooo: " << output;
*/

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

    //report->resize( report->minimumSizeHint() );

    QList<QLabel*> ll = report->findChildren<QLabel *>();

    for ( QLabel * l : ll ) {
        QFont f = l->font();
        f.setStyleStrategy(QFont::PreferAntialias);
        l->setFont(f);

        QString val = engine.evaluate(l->text()).toString();

        if ( engine.hasUncaughtException() ) {
            printOnDisplay("text: " + l->text() + " has exception: " + engine.uncaughtException().toString() );
            delete report;
            return false;
        }
        else {
            l->setText(val);
        }


    }
    //delete report;
    report->setAttribute( Qt::WA_QuitOnClose, false );
    report->show();
    //QPixmap pix = QPixmap::grabWidget(report);
    //delete report;
     qWarning() << "finished! 11111";
     //qDebug() << "printerInfo: " << QPrinterInfo::availablePrinters().count();
     //return true;
    QPrinter printer(QPrinter::HighResolution);
    qWarning() << "finished! 22222";
    //printer.setR
    //printer.setPaperSize(QSize(148, 72), QPrinter::Millimeter);
    printer.setPrinterName(printer_name);
    //printer.setResolution(72);
    //qWarning() << "finished! 33333";
    if (!printer.isValid()) {
        qWarning() << "WARNING! Printer " << printer_name << " is not valid!";
        return false;
    }
    QPainter painter(&printer);
    //painter.setRenderHint(QPainter::Antialiasing, true);
    //painter.setRenderHint(QPainter::TextAntialiasing , true);
    //qWarning() << "finished! 4444444";
    report->render(&painter);
    //qWarning() << "finished! 5555555";
    //QPainter painter(&printer);
    //painter.setRenderHint( QPainter::Antialiasing, true );
    //painter.drawPixmap(0, 0, pix);
    //report->render(&printer);

    //delete report;

    //printer.print

    //qWarning() << "finished!";

    return true;
}
