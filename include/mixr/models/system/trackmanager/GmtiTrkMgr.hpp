
#ifndef __mixr_models_common_GmtiTrkMgr_HPP__
#define __mixr_models_common_GmtiTrkMgr_HPP__

#include "mixr/models/system/trackmanager/ITrackMgr.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: GmtiTrkMgr
//
// Description: Very simple Ground Moving Target Indication (GMTI) Track Manager
// Factory name: GmtiTrkMgr
//
//------------------------------------------------------------------------------
class GmtiTrkMgr : public ITrackMgr
{
   DECLARE_SUBCLASS(GmtiTrkMgr, ITrackMgr)

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
