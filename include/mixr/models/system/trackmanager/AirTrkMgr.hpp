
#ifndef __mixr_models_AirTrkMgr_H__
#define __mixr_models_AirTrkMgr_H__

#include "mixr/models/system/trackmanager/TrackManager.hpp"

#include "mixr/base/units/distance_utils.hpp"

namespace mixr {
namespace base { class Number; }
namespace models {

//------------------------------------------------------------------------------
// Class: AirTrkMgr
//
// Description: Track Manager for A/A modes (e.g., TWS, ACM, SST)
// Factory name: AirTrkMgr
// Slots:
//   positionGate   <base::Number>  ! Position Gate (meters) (default: 2.0f * NM2M)
//   rangeGate      <base::Number>  ! Range Gate (meters) (default: 500.0f)
//   velocityGate   <base::Number>  ! Velocity Gate (m/s) (default: 10.0f)
//
//------------------------------------------------------------------------------
class AirTrkMgr : public TrackManager
{
   DECLARE_SUBCLASS(AirTrkMgr, TrackManager)

public:
   AirTrkMgr();

   double getPosGate()     { return posGate;}
   double getRngGate()     { return rngGate;}
   double getVelGate()     { return velGate;}

protected:
   void processTrackList(const double dt) override;

private:
   void initData();

   // Prediction parameters
   double posGate {2.0 * base::distance::NM2M};   // Position Gate (meters)
   double rngGate {500.0};   // Range Gate (meters)
   double velGate {10.0};    // Velocity Gate (m/s)

   // Used by processTrackList()
   bool** report2TrackMatch {};           // Report/Track matched matrix
   unsigned int* reportNumMatches {};     // Number of matches for each report
   unsigned int* trackNumMatches {};      // Number of matcher for each track

private:
   // slot table helper methods
   bool setSlotPositionGate(const base::Number* const);
   bool setSlotRangeGate(const base::Number* const);
   bool setSlotVelocityGate(const base::Number* const);
};

}
}

#endif
