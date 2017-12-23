
#ifndef __mixr_models_AirAngleOnlyTrkMgrPT_H__
#define __mixr_models_AirAngleOnlyTrkMgrPT_H__

#include "mixr/models/system/trackmanager/AirAngleOnlyTrkMgr.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: AirAngleOnlyTrkMgrPT
//
// Description: AirAngleOnlyTrkMgr that uses perceived-truth az, el,
// and (depending on usePerceivedPosVel) pos, vel from IR reports;
// perceived truth in that some reports are IrSensor's merged reports from two or more targets,
// azimuth, elevation, position and velocity may all be "merged" values.
//
// usePerceivedPosVel determines whether track's potentially merged position and velocity are set in the
// track, which would mean that missile would use perceived truth in its flyout, potentially flying
// towards a merged track centroid instead of toward an actual target.
//
// otherwise, uses track target position, which is a ground truth target position for one
// of the entities in the merged track
//------------------------------------------------------------------------------
class AirAngleOnlyTrkMgrPT : public AirAngleOnlyTrkMgr
{
   DECLARE_SUBCLASS(AirAngleOnlyTrkMgrPT, AirAngleOnlyTrkMgr)
public:
   AirAngleOnlyTrkMgrPT();

protected:
   void processTrackList(const double dt) override;

   virtual void updateTrackAges(const double dt);
   virtual void removeAgedTracks();
private:
   bool usePerceivedPosVel{true};
};

}
}

#endif
