
#ifndef __mixr_models_RfTrack_HPP__
#define __mixr_models_RfTrack_HPP__

#include "mixr/models/track/ITrack.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"

#include "mixr/base/units/util/angle_utils.hpp"
#include "mixr/base/units/util/length_utils.hpp"

#include <array>

namespace mixr {
namespace models {
class RfEmission;
class IPlayer;

//------------------------------------------------------------------------------
// Class: RfTrack
// Factory name: RfTrack
// Description: Generic R/F Target Track (onboard sensors)
//------------------------------------------------------------------------------
class RfTrack : public ITrack
{
   DECLARE_SUBCLASS(RfTrack, ITrack)

public:
   RfTrack();

   // Signal strength and last emission for onboard R/F sensor tracks
   double getAvgSignal() const                     { return avgSig; }
   double getMaxSignal() const                     { return maxSig; }
   int getNumSignals() const                       { return nSig; }
   const RfEmission* getLastEmission() const       { return lastEM; }
   virtual bool setSignal(const double snDbl, const RfEmission* const em);

   // Missile warning (from an RWR)
   bool isMissileWarning() const                   { return mslWarn; }
   virtual bool setMissileWarning(const bool);

   void clear() override;

   virtual bool setLastEmission(const RfEmission* const);

protected:
   const RfEmission* lastEM{};      // Last emission

};

}
}

#endif

