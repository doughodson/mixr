
#ifndef __mixr_simulation_StationTcThread_H__
#define __mixr_simulation_StationTcThread_H__

#include "mixr/base/concurrent/PeriodicTask.hpp"

namespace mixr {
namespace simulation {

// ---
// Time-critical thread
// ---
class TcThread : public base::PeriodicTask
{
   DECLARE_SUBCLASS(TcThread, base::PeriodicTask)
   public: TcThread(base::Component* const parent, const double priority, const double rate);
   private: virtual unsigned long userFunc(const double dt) override;
};

}
}

#endif
