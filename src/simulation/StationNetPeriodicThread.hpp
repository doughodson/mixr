
#ifndef __mixr_simulation_StationNetPeriodicThread_H__
#define __mixr_simulation_StationNetPeriodicThread_H__

#include "mixr/base/threads/PeriodicThread.hpp"

namespace mixr {
namespace simulation {

// ---
// Interoperability Networks thread
// ---
class StationNetPeriodicThread final : public base::PeriodicThread
{
   public: StationNetPeriodicThread(base::Component* const parent, const double rate);
   private: unsigned long userFunc(const double dt) final;
};

}
}



#endif
