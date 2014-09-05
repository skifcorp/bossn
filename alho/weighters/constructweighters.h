#ifndef CONSTRUCTWEIGHTERS_H
#define CONSTRUCTWEIGHTERS_H

#ifdef AAAA

#include "kryzh/grain/kryzhgrainacceptanceculture.h"
#include "kryzh/beet/kryzhbeetacceptanceculture.h"
#include "kryzh/grain/kryzhgrainshipmentculture.h"

#include "kryzh/weighter/kryzhacceptanceweighter.h"
#include "kryzh/weighter/kryzhshipmentweighter.h"

#include "mria/beet/mriabeetacceptanceculture.h"
#include "mria/weighter/mriaacceptanceweighter.h"

#include "factory.h"

#include "mpllibs/metaparse/string.hpp"
#define _S(X) MPLLIBS_STRING(X)



namespace alho {
    //namespace kryzh = alho::kryzh;

    using KryzhBeetAcceptance  = kryzh::AcceptanceWeighter<kryzh::BeetAcceptanceCulture>;
    using KryzhGrainAcceptance = kryzh::AcceptanceWeighter<kryzh::GrainAcceptanceCulture>;
    using KryzhGrainShipment   = kryzh::ShipmentWeighter<kryzh::GrainShipmentCulture>;

    using MriaBeetAcceptance   = mria::AcceptanceWeighter<mria::BeetAcceptanceCulture>;

    namespace {
        BossnFactoryRegistrator2<KryzhBeetAcceptance , _S("KryzhBeetAcceptance" )>  r1;
        BossnFactoryRegistrator2<KryzhGrainAcceptance, _S("KryzhGrainAcceptance")>  r2;
        BossnFactoryRegistrator2<KryzhGrainShipment  , _S("KryzhGrainShipment"  )>  r3;

        BossnFactoryRegistrator2<MriaBeetAcceptance  , _S("MriaBeetAcceptance"  )>   r4;
    }

}

#endif

#undef _S


#endif // CONSTRUCTWEIGHTERS_H
