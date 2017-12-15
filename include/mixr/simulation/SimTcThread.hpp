
#ifndef __mixr_simulation_SimTcThread_H__
#define __mixr_simulation_SimTcThread_H__

#include "mixr/base/concurrent/SyncTask.hpp"

namespace mixr {
namespace base { class Component; class PairStream; }
namespace simulation {

//------------------------------------------------------------------------------
// Class: SimTcThread
// Description: Time critical thread
//------------------------------------------------------------------------------
class SimTcThread : public base::SyncTask
{
public:
   SimTcThread(base::Component* const parent, const double priority);

   // Parent thread signals start to this child thread with these parameters.
   void start(
      base::PairStream* const pl0,
      const double dt0,
      const unsigned int idx0,
      const unsigned int n0
   );

private:
   // SyncTask class function -- our userFunc()
   unsigned long userFunc() override;

private:
   base::PairStream* pl0{};
   double dt0{};
   unsigned int idx0{};
   unsigned int n0{};
};

}
}

#endif
