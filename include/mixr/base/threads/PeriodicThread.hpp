
#ifndef __mixr_base_PeriodicThread_HPP__
#define __mixr_base_PeriodicThread_HPP__

#include "mixr/base/threads/IThread.hpp"
#include "mixr/base/Statistic.hpp"

namespace mixr {
namespace base {
class IComponent;

//------------------------------------------------------------------------------
// Class: PeriodicThread
//
// Description: Periodic thread that calls the user supplied function
//              at a fixed rate. Derived classes define the work
//              function (i.e., userFunc()), which will be called at a fixed
//              rate of 'rate' Hz until the parent component is shutdown.
//              A value of 1.0/rate is passed to userFunc() as the delta time
//              parameter.
//------------------------------------------------------------------------------
class PeriodicThread : public IThread
{
public:
   PeriodicThread(IComponent* const parent, const double rate);
   PeriodicThread(const PeriodicThread&) = delete;
   PeriodicThread& operator=(const PeriodicThread&) = delete;

   double getRate() const;                // Update rate (must be greater than zero)
   int getTotalFrameCount() const;        // Total frame count

   // Busted (overrun) frames statistics; overrun frames time (seconds)
   // (Windows only)
   const Statistic& getBustedFrameStats() const;

   // Variable delta time flag (windows only).
   // If false (default), delta time is always passed as one over the update rate;
   // If true and there's a frame overrun then a delta time adjusted for the overrun
   // is used.
   bool isVariableDeltaTimeEnabled() const;
   bool setVariableDeltaTimeFlag(const bool enable);

private:
   unsigned long mainThreadFunc() final;

   // user defined work function
   virtual unsigned long userFunc(const double dt) =0;

   double rate {};         // Loop rate (hz); until our parent shuts down
   Statistic bfStats {};   // Busted (overrun) frame statistics (windows only)
   int tcnt {};            // total frame count
   bool vdtFlg {};         // Variable delta time flag
};

}
}

#endif

