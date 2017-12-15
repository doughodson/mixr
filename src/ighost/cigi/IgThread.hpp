
#ifndef __mixr_ighost_cigi_IgThread_H__
#define __mixr_ighost_cigi_IgThread_H__

#include "mixr/base/concurrent/SingleTask.hpp"

namespace mixr {
namespace cigi {

class IgThread : public base::SingleTask
{
   public: IgThread(base::Component* const parent, const double priority);
   private: unsigned long userFunc() override;
};

}
}

#endif
