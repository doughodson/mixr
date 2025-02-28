
#ifndef __mixr_ighost_cigi3_IgThread_HPP__
#define __mixr_ighost_cigi3_IgThread_HPP__

#include "mixr/base/threads/IOneShotThread.hpp"

namespace mixr {
namespace cigi {

class IgThread final : public base::IOneShotThread
{
   public: IgThread(base::IComponent* const parent);
   private: unsigned long userFunc() final;
};

}
}

#endif
