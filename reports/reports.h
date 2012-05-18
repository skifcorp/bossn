#ifndef REPORTS_H
#define REPORTS_H

#include <QString>

class Reports
{
public:
    Reports(const QString& f) : file_name(f)
    {

    }

    void print();
private:
    QString file_name;
};

#endif // REPORTS_H
