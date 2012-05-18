
#include "reports.h"

#include <QtUiTools>
#include <QtScript>
#include <QWidget>

void Reports::print()
{
    QUiLoader uil;

    QFile f(file_name);
    f.open(QFile::ReadOnly);

    QWidget * w = uil.load(&f, 0);
    w->show();
}
