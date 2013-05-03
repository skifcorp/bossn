#ifndef MAINSEQUENCE_H
#define MAINSEQUENCE_H

#include <vector>

using std::vector;

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QApplication>
#include <QBitArray>
#include <QtConcurrentRun>
#include <QSqlDatabase>
#include <QTimer>
#include <QVector>

#include "tags.h"
#include "alhosequence.h"
#include "settingstool.h"
#include "readersettings.h"
#include "mifarereader.h"
#include "mifarecarddata.h"
#include "mainsequencesettings.h"
#include "weighter.h"
#include "cardstructs.h"
#include "mifarecard.h"
#include "weighterconf.h"
#include "photomaker.h"
#include "mainsequencebaseop.h"

class MifareCardSector;



class MainSequence : public MainSequenceBaseOp
{
    Q_OBJECT
public:
    MainSequence(Tags & t, const QVariantMap& );

    ~MainSequence() {}

    void processPerimeter();

    Q_INVOKABLE void onAppearOnWeight(const QString&, AlhoSequence*);
    Q_INVOKABLE void onDisappearOnWeight(const QString&, AlhoSequence*);

    virtual void setSettings(const QVariantMap &);

    QString printerName() const {return printer_name;}
protected:
    virtual  void wakeUp() override;
private:    
    bool init = true;
    bool on_weight = false;

    QString printer_name;
    bool uses_photo = false;
    QVariantMap exit_photo;
    QVariantMap enter_photo;

    std::vector<WeighterConf> weighters_conf;

    //void setLightsToRed();
    //void setLightsToGreen();


//    void checkForStealedCard(const ActivateCardISO14443A& );

    void initWeightersConf(const QVariantMap& s);

    WeighterConf& findWeighterConf(int) ;

    WeighterConf& readStruct(MifareCardSector&, MifareCardData& d );
    alho::common::Weighter::Pointer createWeighter (WeighterConf& wc)
    {
        return alho::common::Weighter::create( wc.weighter_name, *this, wc.database, wc.db );
    }




    virtual void run();

    void makePhotoIfNeeded(long, const QString& , const WeighterConf& );
    CapturePhoto capture;
    QString current_card_tag;
    QString current_card_prop;
};


#endif // MAINSEQUENCE_H
