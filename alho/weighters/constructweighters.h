#ifndef CONSTRUCTWEIGHTERS_H
#define CONSTRUCTWEIGHTERS_H


#include "kryzh/grain/grainacceptanceculture.h"
#include "kryzh/beet/beetacceptanceculture.h"
#include "kryzh/grain/grainshipmentculture.h"

#include "kryzh/weighter/acceptanceweighter.h"
#include "kryzh/weighter/shipmentweighter.h"

#include "factory.h"

namespace alho {
    //namespace kryzh = alho::kryzh;

    using KryzhBeetAcceptance  = kryzh::AcceptanceWeighter<kryzh::BeetAcceptanceCulture>;
    using KryzhGrainAcceptance = kryzh::AcceptanceWeighter<kryzh::GrainAcceptanceCulture>;
    using KryzhGrainShipment   = kryzh::ShipmentWeighter<kryzh::GrainShipmentCulture>;

    namespace {
    BossnFactoryRegistrator2<KryzhBeetAcceptance>  r1("KryzhBeetAcceptance");
    BossnFactoryRegistrator2<KryzhGrainAcceptance> r2("KryzhGrainAcceptance");
    BossnFactoryRegistrator2<KryzhGrainShipment>   r3("KryzhGrainShipment");
    }

}


#endif // CONSTRUCTWEIGHTERS_H
