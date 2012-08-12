#include "weighterconf.h"
#include "mainsequence.h"
#include "baseweighter.h"

BaseWeighter::Pointer WeighterConf::createWeighter (MainSequence& seq)
{
    return BaseWeighter::create( weighter_name, seq, database );
}

