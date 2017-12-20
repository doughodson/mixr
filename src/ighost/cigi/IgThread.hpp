
#ifndef __mixr_ighost_cigi_IgThread_H__
#define __mixr_ighost_cigi_IgThread_H__

#include "mixr/base/threads/Thread.hpp"

namespace mixr {
namespace cigi {

class IgThread final : public base::Thread
{
   public: IgThread(base::Component* const parent);
   private: unsigned long userFunc() final;
};

}
}

#endif
