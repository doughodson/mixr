
#ifndef __mixr_simulation_StationTcPeriodicThread_H__
#define __mixr_simulation_StationTcPeriodicThread_H__

#include "mixr/base/threads/PeriodicThread.hpp"

namespace mixr {
namespace simulation {

// ---
// Time-critical thread
// ---
class StationTcPeriodicThread final : public base::PeriodicThread
{
   public: StationTcPeriodicThread(base::Component* const parent, const double rate);
   private: unsigned long userFunc(const double dt) final;
};

}
}

#endif
