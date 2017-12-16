
#include "mixr/base/concurrent/SingleTask.hpp"

#include "mixr/base/Object.hpp"
#include "mixr/base/Component.hpp"
#include <iostream>

namespace mixr {
namespace base {

SingleTask::SingleTask(Component* const p, const double pri) : Thread(p, pri)
{
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long SingleTask::mainThreadFunc()
{
   unsigned long rtn {};

   // Configure this thread
   configThread();

   if ( getParent()->isMessageEnabled(Object::MSG_INFO) ) {
      std::cout << "SingleTask(" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
   }

   // Call the user function only once
   rtn = this->userFunc();

   return rtn;
}

}
}

