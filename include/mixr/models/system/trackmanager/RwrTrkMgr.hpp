
#ifndef __mixr_models_common_RwrTrkMgr_HPP__
#define __mixr_models_common_RwrTrkMgr_HPP__

#include "mixr/models/system/trackmanager/TrackManager.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: RwrTrkMgr
//
// Description: RADAR Warning Receiver (RWR) Track Manager
// Factory name: RwrTrkMgr
//
//------------------------------------------------------------------------------
class RwrTrkMgr : public TrackManager
{
   DECLARE_SUBCLASS(RwrTrkMgr, TrackManager)

public:
   RwrTrkMgr();
protected:
   void processTrackList(const double dt) override;

private:
   void initData();

   // Used by processTrackList()
   bool** report2TrackMatch {};                 // Report/Track matched matrix
   unsigned int* reportNumMatches {};           // Number of matches for each report
   unsigned int* trackNumMatches {};            // Number of matcher for each track
};

}
}

#endif
