
#include "mixr/base/threads/IThread.hpp"

#include "mixr/base/IComponent.hpp"
#include <iostream>

namespace mixr {
namespace base {

IThread::IThread(IComponent* const p) : parent(p)
{
}

IThread::~IThread()
{
   if (!isTerminated()) {
      terminate();
   }
   closeThread();
}

//------------------------------------------------------------------------------
// Create the thread
//------------------------------------------------------------------------------
bool IThread::start(const double _priority)
{
   bool ok{true};

   // Check for a parent pointer
   if (parent == nullptr) {
      std::cerr << "IThread(" << this << ")::start() -- ERROR: Parent pointer is not set!" << std::endl;
      ok = false;
   }

   // Check for valid priority
   priority = _priority;
   if (priority < 0.0f || priority > 1.0f) {
      std::cerr << "IThread(" << this << ")::start() -- ERROR: Invalid priority! Use [ 0.0 ... 1.0 ]" << std::endl;
      ok = false;
   }

   // Create the thread
   if (ok) ok = createThread();

   if (!ok) {
      std::cerr << "IThread(" << this << ")::start() -- ERROR: Did NOT create the thread!" << std::endl;
   }

   killed = !ok;

   return ok;
}

//-----------------------------------------------------------------------------
// Get functions
//-----------------------------------------------------------------------------
bool IThread::isTerminated() const
{
   return killed;
}

IComponent* IThread::getParent()
{
   return parent;
}

const void* IThread::getThreadHandle() const
{
   return theThread;
}

// thread stack size in bytes (or zero if using the default stack size)
std::size_t IThread::getStackSize() const
{
   return stackSize;
}

//-----------------------------------------------------------------------------
// Set functions
//-----------------------------------------------------------------------------

// Set the thread's stack size in bytes (or zero to use the system default size)
bool IThread::setStackSize(const std::size_t size)
{
   stackSize = size;
   return true;
}

// Set the terminated flag
void IThread::setTerminated()
{
   killed = true;
}

}
}
