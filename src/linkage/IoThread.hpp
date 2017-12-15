
#ifndef __mixr_linkage_IoThread_H__
#define __mixr_linkage_IoThread_H__

#include "mixr/base/concurrent/PeriodicTask.hpp"

namespace mixr {
namespace base { class Component; }
namespace linkage {

class IoThread : public base::PeriodicTask
{
   public: IoThread(base::Component* const parent, const double priority, const double rate);
   private: virtual unsigned long userFunc(const double dt) override;
};

}
}

#endif
