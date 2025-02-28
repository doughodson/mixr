
#ifndef __mixr_simulation_StationBgPeriodicThread_HPP__
#define __mixr_simulation_StationBgPeriodicThread_HPP__

#include "mixr/base/threads/IPeriodicThread.hpp"

namespace mixr {
namespace simulation {

// ---
// Background thread
// ---
class StationBgPeriodicThread final : public base::IPeriodicThread
{
   public: StationBgPeriodicThread(base::IComponent* const parent, const double rate);
   private: unsigned long userFunc(const double dt) final;
};

}
}



#endif
