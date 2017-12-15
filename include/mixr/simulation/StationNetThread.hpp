
#ifndef __mixr_simulation_StationNetThread_H__
#define __mixr_simulation_StationNetThread_H__

#include "mixr/base/concurrent/PeriodicTask.hpp"

namespace mixr {
namespace simulation {

// ---
// Interoperability Networks thread
// ---
class NetThread : public base::PeriodicTask
{
   public: NetThread(base::Component* const parent, const double priority, const double rate);
   private: virtual unsigned long userFunc(const double dt) override;
};

}
}



#endif
