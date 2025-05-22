
#include "mixr/models/track/IrTrack.hpp"

#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/IrQueryMsg.hpp"
#include "mixr/models/ISensorMsg.hpp"

#include "mixr/base/units/util/angle_utils.hpp"
#include "mixr/base/units/util/length_utils.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IrTrack,"IrTrack")
EMPTY_SLOTTABLE(IrTrack)

IrTrack::IrTrack() : lastQuery(nullptr)
{
    STANDARD_CONSTRUCTOR()

    clear();
}

void IrTrack::copyData(const IrTrack& org, const bool)
{
    BaseClass::copyData(org);

    setLastQuery( org.lastQuery );

    mslWarn = org.mslWarn;

    avgSig = org.avgSig;
    maxSig = org.maxSig;
    nSig = org.nSig;
    iSig = org.iSig;
    for (int i = 0; i < MAX_SIG; i++) {
       lastSN[i] = org.lastSN[i];
    }
}

void IrTrack::deleteData()
{
    setLastQuery(nullptr);
}

//------------------------------------------------------------------------------
// setSignal() -- set the track's last signal
//------------------------------------------------------------------------------
bool IrTrack::setSignal(const double snDbl, const IrQueryMsg* const q)
{
    // Save the emission
    setLastQuery(q);

    // Save the signal
    lastSN[iSig++] = snDbl;
    if (iSig >= MAX_SIG) iSig = 0;
    if (nSig < MAX_SIG) nSig++;

    // Compute average signal
    double sum{};
    double avg{};
    double maxs{};
    if (nSig > 0) {
        for (int i = 0; i < nSig; i++) {
            sum += lastSN[i];
            if (lastSN[i] > maxs) maxs = lastSN[i];
        }
        avg = sum / static_cast<double>(nSig);
    }
    avgSig = avg;
    maxSig = maxs;

    return true;
}

// setPosition() -- set track's position vector - but do not set rel az or el
bool IrTrack::setPosition(const base::Vec3d& p)
{
   // set position vector
   pos = p;

   // compute ranges
   double gndRng2{pos.x()*pos.x() + pos.y()*pos.y()};
   gndRng = std::sqrt(gndRng2);
   rng = std::sqrt(gndRng2 +  pos.z()*pos.z());

   // compute angles
   taz = std::atan2(pos.y(),pos.x());

   // Set LOS unit vector
   if (rng > 0) los.set( pos.x()/rng, pos.y()/rng, pos.z()/rng );
   else los.set(0,0,0);

   return true;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setLastQuery() -- set the last emission pointer
bool IrTrack::setLastQuery(const IrQueryMsg* const q)
{
   if (lastQuery != nullptr) lastQuery->unref();
   lastQuery = q;
   if (lastQuery != nullptr) lastQuery->ref();
   return true;
}

// Set missile warning flag
bool IrTrack::setMissileWarning(const bool b)
{
   mslWarn = b;
   return true;
}

//------------------------------------------------------------------------------
// clear() -- clear the track
//------------------------------------------------------------------------------
void IrTrack::clear()
{
   mslWarn = false;

   setLastQuery(nullptr);
   avgSig = 0;
   maxSig = 0;
   nSig = 0;
   iSig = 0;

   trackClass = ANGLE_ONLY;

   BaseClass::clear();
}

}
}
