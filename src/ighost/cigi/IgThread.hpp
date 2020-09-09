
#ifndef __mixr_ighost_cigi3_IgThread_HPP__
#define __mixr_ighost_cigi3_IgThread_HPP__

#include "mixr/base/threads/OneShotThread.hpp"

namespace mixr {
namespace cigi {

class IgThread final : public base::OneShotThread
{
   public: IgThread(base::Component* const parent);
   private: unsigned long userFunc() final;
};

}
}

#endif
