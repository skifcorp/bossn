#ifndef CONSTRUCTWEIGHTERS_H
#define CONSTRUCTWEIGHTERS_H


#include "kryzh/grain/grainacceptanceculture.h"
#include "kryzh/beet/beetacceptanceculture.h"
#include "kryzh/grain/grainshipmentculture.h"

#include "kryzh/weighter/acceptanceweighter.h"
#include "kryzh/weighter/shipmentweighter.h"

#include "factory.h"

#include "mpllibs/metaparse/string.hpp"
#define _S(X) MPLLIBS_STRING(X)

namespace alho {
    //namespace kryzh = alho::kryzh;

    using KryzhBeetAcceptance  = kryzh::AcceptanceWeighter<kryzh::BeetAcceptanceCulture>;
    using KryzhGrainAcceptance = kryzh::AcceptanceWeighter<kryzh::GrainAcceptanceCulture>;
    using KryzhGrainShipment   = kryzh::ShipmentWeighter<kryzh::GrainShipmentCulture>;

    namespace {
        BossnFactoryRegistrator2<KryzhBeetAcceptance , _S("KryzhBeetAcceptance" )>  r1;
        BossnFactoryRegistrator2<KryzhGrainAcceptance, _S("KryzhGrainAcceptance")>  r2;
        BossnFactoryRegistrator2<KryzhGrainShipment  , _S("KryzhGrainShipment"  )>  r3;
    }

}

#undef _S


#endif // CONSTRUCTWEIGHTERS_H
