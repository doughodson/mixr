
#include "mixr/models/SensorMsg.hpp"

#include "mixr/models/player/Player.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SensorMsg, "SensorMsg")
EMPTY_SLOTTABLE(SensorMsg)

SensorMsg::SensorMsg()
{
    STANDARD_CONSTRUCTOR()

    initData();
}

void SensorMsg::initData()
{
    losO2T.set(0,0,0);
    losT2O.set(0,0,0);
    aoi.set(0,0,0);
}

void SensorMsg::copyData(const SensorMsg& org, const bool cc)
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

    const Player* oo{org.ownship};
    setOwnship( const_cast<Player*>(static_cast<const Player*>(oo)) );

    const Gimbal* aa{org.gimbal};
    setGimbal( const_cast<Gimbal*>(static_cast<const Gimbal*>(aa)) );

    const Player* pp{org.target};
    setTarget( const_cast<Player*>(static_cast<const Player*>(pp)) );

    const base::Object* msg{org.dataMsg};
    setDataMessage( const_cast<base::Object*>(static_cast<const base::Object*>(msg)) );

    returnReq = org.returnReq;
    localOnly = org.localOnly;
}

void SensorMsg::deleteData()
{
   clear();
}

//------------------------------------------------------------------------------
// clear() -- clears out the emissions
//------------------------------------------------------------------------------
void SensorMsg::clear()
{
   setOwnship(nullptr);
   setGimbal(nullptr);
   setTarget(nullptr);
   setDataMessage(nullptr);
}
//------------------------------------------------------------------------------
// Sets the range to the target
//------------------------------------------------------------------------------
void SensorMsg::setRange(const double r)
{
   rng = r;
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------

double SensorMsg::getRangeRateFPS() const
{
   return getRangeRate() * base::length::M2FT;
}

double SensorMsg::getRangeRateKts() const
{
   return getRangeRate() * base::length::M2NM * 3600.0f;
}

//------------------------------------------------------------------------------
// setOwnship() -- Sets the pointer to the originator (ownship)
//------------------------------------------------------------------------------
void SensorMsg::setOwnship(Player* const p)
{
   ownship = p;
}

//------------------------------------------------------------------------------
// setGimbal() -- Sets the pointer to the transmitting gimbal (seeker/antenna)
//------------------------------------------------------------------------------
void SensorMsg::setGimbal(Gimbal* const a)
{
   gimbal = a;
}

//------------------------------------------------------------------------------
// setTarget() -- Sets the pointer to the target
//------------------------------------------------------------------------------
void SensorMsg::setTarget(Player* const p)
{
   target = p;
}

//------------------------------------------------------------------------------
// setDataMessage() -- Sets a pointer to an optional data message
//------------------------------------------------------------------------------
void SensorMsg::setDataMessage(base::Object* const msg)
{
   dataMsg = msg;
}

}
}
