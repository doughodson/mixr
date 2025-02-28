
#ifndef __mixr_linkage_IoPeriodicThread_HPP__
#define __mixr_linkage_IoPeriodicThread_HPP__

#include "mixr/base/threads/IPeriodicThread.hpp"

namespace mixr {
namespace base { class IComponent; }
namespace linkage {

class IoPeriodicThread final : public base::IPeriodicThread
{
   public: IoPeriodicThread(base::IComponent* const parent, const double rate);
   private: unsigned long userFunc(const double dt) final;
};

}
}

#endif
