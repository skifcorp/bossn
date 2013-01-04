#ifndef BASEWEIGHTER_H
#define BASEWEIGHTER_H

#include <QSharedPointer>
#include <QSqlDatabase>

#include "qxorm_pch.h"

#include "factory.h"


class WeighterConf;
class MifareCardData;
class MifareCardSector;
class MainSequence;

namespace alho { namespace common {


class Weighter : public BossnFactory<Weighter, MainSequence&, QSqlDatabase&, boost::rdb::mysql::mysql_database& >
{
public:

    Weighter() {}
    Weighter(const Weighter & ) = delete;

    virtual ~Weighter() {}

    virtual void processWeighting(MifareCardData&, MifareCardSector&, const WeighterConf&  ) = 0 ;
   // virtual QString detectPlatformType(const MifareCardData& ) const throw (MainSequenceException) = 0;
   // QString platformType(const MifareCardData & bill, const WeighterConf& ) const throw (MainSequenceException);
	
 
	
    void setCardBanned(const QString& );
    void checkCardBanned(const QString& );
};


} }




#endif // BASEWEIGHTER_H
