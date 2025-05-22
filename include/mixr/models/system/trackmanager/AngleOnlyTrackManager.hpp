
#ifndef __mixr_models_common_AngleOnlyTrackManager_HPP__
#define __mixr_models_common_AngleOnlyTrackManager_HPP__

#include "mixr/models/system/trackmanager/ITrackMgr.hpp"
#include "mixr/base/safe_queue.hpp"
#include "mixr/base/util/constants.hpp"

namespace mixr {
namespace base { class IAngle; class INumber; }
namespace models {
class IrQueryMsg;
class ITrack;

//------------------------------------------------------------------------------
// Class: AngleOnlyTrackManager
// Description: Interface class for Angle Only Track Managers
//
// Factory name: AngleOnlyTrackManager
// Slots:
//    azimuthBin    <base::IAngle>   ! Azimuth Bin (default: PI)
//    elevationBin  <base::IAngle>   ! Elevation Bin (default: PI)
//------------------------------------------------------------------------------
class AngleOnlyTrackManager : public ITrackMgr
{
   DECLARE_SUBCLASS(AngleOnlyTrackManager, ITrackMgr)

public:
   AngleOnlyTrackManager();

   virtual void newReport(IrQueryMsg* q, double snDbl);

   void clearTracksAndQueues() override;
   bool addTrack(ITrack* const t) override;

protected:
   virtual IrQueryMsg* getQuery(double* const sn);                     // Get the next 'new' report from the queue

   bool shutdownNotification() override;

   // Prediction parameters
   double azimuthBin{base::PI};     // Azimuth Bin
   double elevationBin{base::PI};   // Elevation Bin
   double oneMinusAlpha{};          // 1 - Alpha parameter
   double oneMinusBeta{1.0};        // 1 - Beta parameter

private:
   base::safe_queue<IrQueryMsg*> queryQueue;  // Emission input queue (used with the
                                              //   TrackManager::queueLock semaphore)
private:
   // slot table helper methods
   bool setSlotAzimuthBin(const base::IAngle* const);
   bool setSlotElevationBin(const base::IAngle* const);
   bool setSlotAlpha(const base::INumber* const) override;
   bool setSlotBeta(const base::INumber* const) override;
};

}
}

#endif
