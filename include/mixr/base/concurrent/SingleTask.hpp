
#ifndef __mixr_base_SingleTask_H__
#define __mixr_base_SingleTask_H__

#include "mixr/base/concurrent/Thread.hpp"

namespace mixr {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: SingleTask
// Description: Abstract single task thread class
//
//    User's will need to derive a class from this class and define the
//    work function, userFunc(), which is only called once.
//------------------------------------------------------------------------------
class SingleTask : public Thread
{
public:
   SingleTask(Component* const parent, const double priority);

   // User defined work function
   private: virtual unsigned long userFunc() =0;

private:
   // Thread class function
   virtual unsigned long mainThreadFunc() override;
};

}
}

#endif

