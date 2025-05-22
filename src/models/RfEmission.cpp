
#include "mixr/models/RfEmission.hpp"

#include "mixr/models/system/IRfSystem.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(RfEmission, "RfEmission")
EMPTY_SLOTTABLE(RfEmission)

RfEmission::RfEmission()
{
    STANDARD_CONSTRUCTOR()
}

void RfEmission::copyData(const RfEmission& org, const bool)
{
    BaseClass::copyData(org);

    // Copy the data
    freq = org.freq;
    lambda = org.lambda;
    pw = org.pw;
    power = org.power;
    polar = org.polar;
    bw = org.bw;
    gain = org.gain;
    prf = org.prf;
    pulses = org.pulses;
    lossRng = org.lossRng;
    lossAtmos = org.lossAtmos;
    lossXmit = org.lossXmit;
    rcs = org.rcs;

    const IRfSystem* mm = org.transmitter;
    setTransmitter( const_cast<IRfSystem*>(static_cast<const IRfSystem*>(mm)) );

    ecmFlag = org.ecmFlag;
}

void RfEmission::deleteData()
{
   clear();
}

//------------------------------------------------------------------------------
// clear() -- clears out the emissions
//------------------------------------------------------------------------------
void RfEmission::clear()
{
   BaseClass::clear();
   setTransmitter(nullptr);
}

//------------------------------------------------------------------------------
// Sets the range to the target
//------------------------------------------------------------------------------
void RfEmission::setRange(const double r)
{
   BaseClass::setRange(r);

   if (r > 1.0) lossRng = static_cast<double>(1.0/(4.0 * base::PI * r * r));
   else lossRng = 1.0;
}

//------------------------------------------------------------------------------
// setTransmitter() -- Sets the pointer to the source
//------------------------------------------------------------------------------
void RfEmission::setTransmitter(IRfSystem* const t)
{
   //if (transmitter != nullptr) {
   //   transmitter->unref();
   //}
   transmitter = t;
   //if (transmitter != nullptr) {
   //   transmitter->ref();
   //}
}

}
}
