#ifndef BASEWEIGHTER_H
#define BASEWEIGHTER_H

#include <QSharedPointer>

#include "factory.h"

class WeighterConf;
class MifareCardData;
class MifareCardSector;
class MainSequence;

namespace alho { namespace common {


class Weighter : public BossnFactory<Weighter, MainSequence&, QSqlDatabase& >
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
