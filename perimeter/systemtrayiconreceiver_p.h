#ifndef SYSTEMTRAYICONRECEIVER_P_H
#define SYSTEMTRAYICONRECEIVER_P_H


#include <QMenu>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QAction>

class SystemTrayIconEventsReceiver : public QObject
{
    Q_OBJECT
public:
    SystemTrayIconEventsReceiver() : tray (new QSystemTrayIcon(QIcon(":/icons/picts/car.png"),this)),
                                        menu(new QMenu())
    {
        menu->addAction("SetOn" , this, SLOT(onOn()));
        menu->addAction("SetOff", this, SLOT(onOff()));

        tray->setContextMenu(menu);

        tray->show();
    }
    ~SystemTrayIconEventsReceiver() {}
    bool isOn() const
    {
        return is_on;
    }
signals:
    void onPressed();
    void offPressed();
public slots:
    void onOn();
    void onOff();
private:
    QSystemTrayIcon * tray;
    QMenu           * menu;
    bool              is_on = false;
};



#endif // SYSTEMTRAYICONRECEIVER_P_H
