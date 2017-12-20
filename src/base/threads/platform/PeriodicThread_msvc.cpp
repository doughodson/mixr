
#include "mixr/base/threads/PeriodicThread.hpp"

#include "mixr/base/Object.hpp"
#include "mixr/base/Component.hpp"
#include "mixr/base/util/math_utils.hpp"
#include "mixr/base/util/system_utils.hpp"
#include <iostream>

//-----------------------------------------------------------------
// Define 'USE_REALTIME_PRI_CLASS' to use 'REALTIME_PRIORITY_CLASS'
// The default is HIGH_PRIORITY_CLASS
//-----------------------------------------------------------------
//#define USE_REALTIME_PRI_CLASS

namespace mixr {
namespace base {

// max number of processors we'll allow
static const int MAX_CPUS{32};

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long PeriodicThread::mainThreadFunc()
{
   unsigned long rtn{};

   // Configure this thread
   bool ok{configThread()};

   if ( getParent()->isMessageEnabled(Object::MSG_INFO) ) {
      std::cout << "PeriodicThread(" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
      std::cout << "PeriodicThread(" << this << ")::mainThreadFunc(): Parent = " << getParent() << std::endl;
      std::cout << "PeriodicThread(" << this << ")::mainThreadFunc(): Starting main loop ..." << std::endl;
   }

   // All of the real work is done by ...
   if (ok) {
      double refTime{};                                    // Reference time
      const double startTime{getComputerTime()};           // Computer's time of day (sec) run started
      double dt{1.0/static_cast<double>(getRate())};

      while (!getParent()->isShutdown()) {

         // ---
         // User defined tasks
         // ---
         this->userFunc(dt);

         // ---
         // Wait for the start of the next frame
         // ---
         {
            // Update reference time at the start of next frame
            refTime = (refTime + dt);

            // Current computer time
            const double time{getComputerTime()};

            // Actual run time
            const double t0{(time - startTime)};

            // How long should we sleep for
            const double st{refTime - t0};
            const auto sleepFor = static_cast<int>(st*1000.0);

            // wait for the next frame
            if (sleepFor > 0) Sleep(sleepFor);

            // Compute next delta time and update frame stats
            dt = 1.0/static_cast<double>(getRate());
            if (st < 0) {
               const double overrun{-st};
               bfStats.sigma(overrun);
               if (vdtFlg) {
                  // adjust for overrun
                  dt += overrun;
                  refTime += overrun;
               }
            }
            tcnt++;
         }
      }
   }

   if (getParent()->isMessageEnabled(Object::MSG_INFO) ) {
      std::cout << "PeriodicThread(" << this << ")::mainThreadFunc(): ... end of main loop." << std::endl;
   }

   return rtn;
}

}
}
