
#ifndef __mixr_models_IrTrack_HPP__
#define __mixr_models_IrTrack_HPP__

#include "mixr/models/track/ITrack.hpp"

#include "mixr/base/IList.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"

#include "mixr/base/qty/util/angle_utils.hpp"
#include "mixr/base/qty/util/length_utils.hpp"

#include <array>

namespace mixr {
namespace models {
class IrQueryMsg;
class IPlayer;

//------------------------------------------------------------------------------
// Class: IrTrack
// Factory name: IrTrack
// Description: Generic IR Target Track (onboard sensors)
//------------------------------------------------------------------------------
class IrTrack : public ITrack
{
   DECLARE_SUBCLASS(IrTrack, ITrack)

public:
   IrTrack();

   // Signal strength and last emission for onboard IR sensor tracks
   double getAvgSignal() const                                             { return avgSig; }
   double getMaxSignal() const                                             { return maxSig; }
   int getNumSignals() const                                               { return nSig; }
   const IrQueryMsg* getLastQuery() const                                  { return lastQuery; }
   virtual bool setSignal(const double snDbl, const IrQueryMsg* const);
   bool setPosition(const base::Vec3d&) override;

   // Missile warning (from an RWR)
   bool isMissileWarning() const                                           { return mslWarn; }
   virtual bool setMissileWarning(const bool);

   void clear() override;

//protected:
   virtual bool setLastQuery(const IrQueryMsg* const);

protected:
   const IrQueryMsg* lastQuery {}; // Last query

};

}
}

#endif

