
#ifndef __mixr_simulation_StationBgPeriodicThread_H__
#define __mixr_simulation_StationBgPeriodicThread_H__

#include "mixr/base/threads/PeriodicThread.hpp"

namespace mixr {
namespace simulation {

// ---
// Background thread
// ---
class StationBgPeriodicThread final : public base::PeriodicThread
{
   public: StationBgPeriodicThread(base::Component* const parent, const double rate);
   private: unsigned long userFunc(const double dt) final;
};

}
}



#endif
