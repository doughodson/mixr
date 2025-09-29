
#include "mixr/models/system/Jammer.hpp"

#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/system/Antenna.hpp"
#include "mixr/models/RfEmission.hpp"

#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/qty/angles.hpp"
#include "mixr/base/qty/frequencies.hpp"
#include "mixr/base/qty/powers.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Jammer, "Jammer")
EMPTY_SLOTTABLE(Jammer)
EMPTY_DELETEDATA(Jammer)

Jammer::Jammer()
{
    STANDARD_CONSTRUCTOR()
    setTransmitterEnableFlag(true);
    setReceiverEnabledFlag(false);
    setTypeId("JAMMER");
}

void Jammer::copyData(const Jammer& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// transmit() -- send jam emissions
//------------------------------------------------------------------------------
void Jammer::transmit(const double)
{
    // Send the emission to the other player
    if ( !areEmissionsDisabled() && isTransmitting() ) {
        const auto em = new RfEmission();
        em->setFrequency(getFrequency());
        const double p{getPeakPower()};
        em->setPower(p);
        em->setTransmitLoss(getRfTransmitLoss());
        em->setMaxRangeNM(getRange());
        em->setBandwidth(getBandwidth());
        em->setTransmitter(this);
        em->setReturnRequest(false);
        em->setECM(RfEmission::ECM_NOISE);
        getAntenna()->rfTransmit(em);
        em->unref();
    }
}

}
}
