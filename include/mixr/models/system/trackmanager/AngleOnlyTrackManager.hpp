
#ifndef __mixr_models_AngleOnlyTrackManager_H__
#define __mixr_models_AngleOnlyTrackManager_H__

#include "mixr/models/system/trackmanager/TrackManager.hpp"
#include "mixr/base/safe_queue.hpp"
#include "mixr/base/util/constants.hpp"

namespace mixr {
namespace base { class Number; }
namespace models {
class IrQueryMsg;
class Track;

//------------------------------------------------------------------------------
// Class: AngleOnlyTrackManager
// Description: Abstract class for Angle Only Track Managers
//
// Factory name: AngleOnlyTrackManager
// Slots:
//    azimuthBin    <Number>   ! Azimuth Bin (default: PI)
//    elevationBin  <Number>   ! Elevation Bin (default: PI)
//
//------------------------------------------------------------------------------
class AngleOnlyTrackManager : public TrackManager
{
   DECLARE_SUBCLASS(AngleOnlyTrackManager, TrackManager)

public:
   AngleOnlyTrackManager();

   virtual void newReport(IrQueryMsg* q, double snDbl);

   void clearTracksAndQueues() override;
   bool addTrack(Track* const t) override;

protected:
   virtual IrQueryMsg* getQuery(double* const sn);                     // Get the next 'new' report from the queue

   bool shutdownNotification() override;

   // Prediction parameters
   double azimuthBin {base::PI};     // Azimuth Bin
   double elevationBin {base::PI};   // Elevation Bin
   double oneMinusAlpha {};          // 1 - Alpha parameter
   double oneMinusBeta {1.0};        // 1 - Beta parameter

private:
   base::safe_queue<IrQueryMsg*> queryQueue;  // Emission input queue (used with the
                                              //   TrackManager::queueLock semaphore)
private:
   // slot table helper methods
   bool setSlotAzimuthBin(const base::Number* const);
   bool setSlotElevationBin(const base::Number* const);
   bool setSlotAlpha(const base::Number* const) override;
   bool setSlotBeta(const base::Number* const) override;
};

}
}

#endif
