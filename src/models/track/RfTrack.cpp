
#include "mixr/models/track/RfTrack.hpp"

#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/RfEmission.hpp"
#include "mixr/models/ISensorMsg.hpp"

#include "mixr/base/qty/util/angle_utils.hpp"
#include "mixr/base/qty/util/length_utils.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(RfTrack, "RfTrack")
EMPTY_SLOTTABLE(RfTrack)

RfTrack::RfTrack() : lastEM(nullptr)
{
    STANDARD_CONSTRUCTOR()

    clear();
}

void RfTrack::copyData(const RfTrack& org, const bool)
{
    BaseClass::copyData(org);

    setLastEmission( org.lastEM );

    mslWarn = org.mslWarn;

    avgSig = org.avgSig;
    maxSig = org.maxSig;
    nSig = org.nSig;
    iSig = org.iSig;
    for (int i = 0; i < MAX_SIG; i++) {
       lastSN[i] = org.lastSN[i];
    }
}

void RfTrack::deleteData()
{
    setLastEmission(nullptr);
}

//------------------------------------------------------------------------------
// setSignal() -- set the track's last signal
//------------------------------------------------------------------------------
bool RfTrack::setSignal(const double snDbl, const RfEmission* const em)
{
    // Save the emission
    setLastEmission(em);

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

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setLastEmission() -- set the last emission pointer
bool RfTrack::setLastEmission(const RfEmission* const em)
{
   if (lastEM != nullptr) lastEM->unref();
   lastEM = em;
   if (lastEM != nullptr) lastEM->ref();
   return true;
}

// Set missile warning flag
bool RfTrack::setMissileWarning(const bool b)
{
   mslWarn = b;
   return true;
}

//------------------------------------------------------------------------------
// clear() -- clear the track
//------------------------------------------------------------------------------
void RfTrack::clear()
{
   mslWarn = false;

   setLastEmission(nullptr);
   avgSig = 0;
   maxSig = 0;
   nSig = 0;
   iSig = 0;

   trackClass = RANGE_AND_ANGLE;

   BaseClass::clear();
}

}
}
