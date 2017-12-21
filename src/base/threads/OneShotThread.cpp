
#include "mixr/base/threads/OneShotThread.hpp"

#include "mixr/base/Object.hpp"
#include "mixr/base/Component.hpp"
#include <iostream>

namespace mixr {
namespace base {

OneShotThread::OneShotThread(Component* const p) : AbstractThread(p)
{
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long OneShotThread::mainThreadFunc()
{
   unsigned long rtn {};

   // Configure this thread
   configThread();

   if ( getParent()->isMessageEnabled(Object::MSG_INFO) ) {
      std::cout << "OneShotThread (" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
   }

   // Call the user function only once
   rtn = this->userFunc();

   return rtn;
}

}
}

