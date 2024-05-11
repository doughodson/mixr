
#ifndef __mixr_models_common_AirAngleOnlyTrkMgr_HPP__
#define __mixr_models_common_AirAngleOnlyTrkMgr_HPP__

#include "mixr/models/common/system/trackmanager/AngleOnlyTrackManager.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: AirAngleOnlyTrkMgr
//
// Description: Track Manager for A/A modes (e.g., TWS, ACM, SST)
// Factory name: AirAngleOnlyTrkMgr
//
//------------------------------------------------------------------------------
class AirAngleOnlyTrkMgr : public AngleOnlyTrackManager
{
   DECLARE_SUBCLASS(AirAngleOnlyTrkMgr, AngleOnlyTrackManager)

public:
   AirAngleOnlyTrkMgr();

protected:
   void processTrackList(const double dt) override;
};

}
}

#endif
