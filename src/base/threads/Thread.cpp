
#include "mixr/base/threads/Thread.hpp"

#include "mixr/base/Object.hpp"
#include "mixr/base/Component.hpp"
#include <iostream>

namespace mixr {
namespace base {

Thread::Thread(Component* const p) : AbstractThread(p)
{
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long Thread::mainThreadFunc()
{
   unsigned long rtn {};

   // Configure this thread
   configThread();

   if ( getParent()->isMessageEnabled(Object::MSG_INFO) ) {
      std::cout << "Thread (" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
   }

   // Call the user function only once
   rtn = this->userFunc();

   return rtn;
}

}
}

