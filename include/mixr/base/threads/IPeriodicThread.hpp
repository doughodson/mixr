
#ifndef __mixr_base_IPeriodicThread_HPP__
#define __mixr_base_IPeriodicThread_HPP__

#include "mixr/base/threads/IThread.hpp"
#include "mixr/base/Statistic.hpp"

namespace mixr {
namespace base {
class IComponent;

//------------------------------------------------------------------------------
// Class: IPeriodicThread
//
// Description: Interface that defines a periodic thread that calls the user supplied function
//              at a fixed rate. Derived classes define the work
//              function (i.e., userFunc()), which will be called at a fixed
//              rate of 'rate' Hz until the parent component is shutdown.
//              A value of 1.0/rate is passed to userFunc() as the delta time
//              parameter.
//------------------------------------------------------------------------------
class IPeriodicThread : public IThread
{
public:
   IPeriodicThread(IComponent* const parent, const double rate);
   IPeriodicThread(const IPeriodicThread&) = delete;
   IPeriodicThread& operator=(const IPeriodicThread&) = delete;

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

