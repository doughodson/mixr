
#ifndef __mixr_models_GmtiTrkMgr_H__
#define __mixr_models_GmtiTrkMgr_H__

#include "mixr/models/system/trackmanager/TrackManager.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: GmtiTrkMgr
//
// Description: Very simple Ground Moving Target Indication (GMTI) Track Manager
// Factory name: GmtiTrkMgr
//
//------------------------------------------------------------------------------
class GmtiTrkMgr : public TrackManager
{
   DECLARE_SUBCLASS(GmtiTrkMgr, TrackManager)

public:
   GmtiTrkMgr();
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
