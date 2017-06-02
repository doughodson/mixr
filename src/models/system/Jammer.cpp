
#include "mxrp/models/system/Jammer.hpp"

#include "mxrp/models/player/Player.hpp"
#include "mxrp/models/system/Antenna.hpp"
#include "mxrp/models/Emission.hpp"

//#include "mxrp/models/WorldModel.hpp"

#include "mxrp/base/PairStream.hpp"
#include "mxrp/base/Pair.hpp"
#include "mxrp/base/units/Angles.hpp"
#include "mxrp/base/units/Frequencies.hpp"
#include "mxrp/base/units/Powers.hpp"

namespace oe {
namespace models {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Jammer, "Jammer")
EMPTY_SERIALIZER(Jammer)

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

EMPTY_DELETEDATA(Jammer)

//------------------------------------------------------------------------------
// transmit() -- send jam emissions
//------------------------------------------------------------------------------
void Jammer::transmit(const double)
{
    // Send the emission to the other player
    if ( !areEmissionsDisabled() && isTransmitting() ) {
        const auto em = new Emission();
        em->setFrequency(getFrequency());
        const double p = getPeakPower();
        em->setPower(p);
        em->setTransmitLoss(getRfTransmitLoss());
        em->setMaxRangeNM(getRange());
        em->setBandwidth(getBandwidth());
        em->setTransmitter(this);
        em->setReturnRequest(false);
        em->setECM(Emission::ECM_NOISE);
        getAntenna()->rfTransmit(em);
        em->unref();
    }
}

}
}
