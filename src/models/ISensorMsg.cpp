
#include "mixr/models/ISensorMsg.hpp"

#include "mixr/models/player/IPlayer.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ISensorMsg, "ISensorMsg")
EMPTY_SLOTTABLE(ISensorMsg)

ISensorMsg::ISensorMsg()
{
    STANDARD_CONSTRUCTOR()

    initData();
}

void ISensorMsg::initData()
{
    losO2T.set(0,0,0);
    losT2O.set(0,0,0);
    aoi.set(0,0,0);
}

void ISensorMsg::copyData(const ISensorMsg& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();

    // copy the data
    maxRng = org.maxRng;
    rng = org.rng;
    rngRate = org.rngRate;
    gaz = org.gaz;
    gel = org.gel;
    iaz = org.iaz;
    iel = org.iel;
    aoi = org.aoi;
    losO2T = org.losO2T;
    losT2O = org.losT2O;

    const IPlayer* oo{org.ownship};
    setOwnship( const_cast<IPlayer*>(static_cast<const IPlayer*>(oo)) );

    const IGimbal* aa{org.gimbal};
    setGimbal( const_cast<IGimbal*>(static_cast<const IGimbal*>(aa)) );

    const IPlayer* pp{org.target};
    setTarget( const_cast<IPlayer*>(static_cast<const IPlayer*>(pp)) );

    const base::IObject* msg{org.dataMsg};
    setDataMessage( const_cast<base::IObject*>(static_cast<const base::IObject*>(msg)) );

    returnReq = org.returnReq;
    localOnly = org.localOnly;
}

void ISensorMsg::deleteData()
{
   clear();
}

//------------------------------------------------------------------------------
// clear() -- clears out the emissions
//------------------------------------------------------------------------------
void ISensorMsg::clear()
{
   setOwnship(nullptr);
   setGimbal(nullptr);
   setTarget(nullptr);
   setDataMessage(nullptr);
}
//------------------------------------------------------------------------------
// Sets the range to the target
//------------------------------------------------------------------------------
void ISensorMsg::setRange(const double r)
{
   rng = r;
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------

double ISensorMsg::getRangeRateFPS() const
{
   return getRangeRate() * base::length::M2FT;
}

double ISensorMsg::getRangeRateKts() const
{
   return getRangeRate() * base::length::M2NM * 3600.0f;
}

//------------------------------------------------------------------------------
// setOwnship() -- Sets the pointer to the originator (ownship)
//------------------------------------------------------------------------------
void ISensorMsg::setOwnship(IPlayer* const p)
{
   ownship = p;
}

//------------------------------------------------------------------------------
// setGimbal() -- Sets the pointer to the transmitting gimbal (seeker/antenna)
//------------------------------------------------------------------------------
void ISensorMsg::setGimbal(IGimbal* const a)
{
   gimbal = a;
}

//------------------------------------------------------------------------------
// setTarget() -- Sets the pointer to the target
//------------------------------------------------------------------------------
void ISensorMsg::setTarget(IPlayer* const p)
{
   target = p;
}

//------------------------------------------------------------------------------
// setDataMessage() -- Sets a pointer to an optional data message
//------------------------------------------------------------------------------
void ISensorMsg::setDataMessage(base::IObject* const msg)
{
   dataMsg = msg;
}

}
}
