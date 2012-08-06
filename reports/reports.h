#ifndef REPORTS_H
#define REPORTS_H

#include <QString>
#include <QVariant>
//#include <QtScript>

class Reports
{
public:
    Reports(const QString& f) : file_name(f)
    {

    }

    Reports()
    {

    }
    void setFileName(const QString& fn) {file_name = fn;}
    QString fileName() const {return file_name;}
    void setPrinterName(const QString& pn) {printer_name = pn;}
    QString  printerName()const {return printer_name;}
    bool print(const QVariantMap&);
private:
    QString file_name;
    QString printer_name;

};

#endif // REPORTS_H
