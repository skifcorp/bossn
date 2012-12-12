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
                                        menu(new QMenu()), something_on_weights(false)
    {
        menu->addAction("WeightsOn", this, SLOT(onWeightOn()));
        menu->addAction("WeightsOff", this, SLOT(onWeightOff()));

        tray->setContextMenu(menu);

        tray->show();
    }
    ~SystemTrayIconEventsReceiver() {}
    bool somethingOnWeights() const
    {
        return something_on_weights;
    }
public slots:
    void onWeightOn();
    void onWeightOff();
private:
    QSystemTrayIcon * tray;
    QMenu           * menu;
    bool              something_on_weights;
};


#endif // SYSTEMTRAYICONRECEIVER_P_H
