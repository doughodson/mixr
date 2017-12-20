
#include "mixr/base/threads/AbstractThread.hpp"

#include "mixr/base/Component.hpp"
#include <iostream>

namespace mixr {
namespace base {

AbstractThread::AbstractThread(Component* const p) : parent(p)
{
}

AbstractThread::~AbstractThread()
{
   if (!isTerminated()) {
      terminate();
   }
   closeThread();
}

//------------------------------------------------------------------------------
// Create the thread
//------------------------------------------------------------------------------
bool AbstractThread::start(const double _priority)
{
   bool ok{true};

   // Check for a parent pointer
   if (parent == nullptr) {
      std::cerr << "AbstractThread(" << this << ")::start() -- ERROR: Parent pointer is not set!" << std::endl;
      ok = false;
   }

   // Check for valid priority
   priority = _priority;
   if (priority < 0.0f || priority > 1.0f) {
      std::cerr << "AbstractThread(" << this << ")::start() -- ERROR: Invalid priority! Use [ 0.0 ... 1.0 ]" << std::endl;
      ok = false;
   }

   // Create the thread
   if (ok) ok = createThread();

   if (!ok) {
      std::cerr << "AbstractThread(" << this << ")::start() -- ERROR: Did NOT create the thread!" << std::endl;
   }

   killed = !ok;

   return ok;
}

//-----------------------------------------------------------------------------
// Get functions
//-----------------------------------------------------------------------------
bool AbstractThread::isTerminated() const
{
   return killed;
}

Component* AbstractThread::getParent()
{
   return parent;
}

const void* AbstractThread::getThreadHandle() const
{
   return theThread;
}

// thread stack size in bytes (or zero if using the default stack size)
std::size_t AbstractThread::getStackSize() const
{
   return stackSize;
}

//-----------------------------------------------------------------------------
// Set functions
//-----------------------------------------------------------------------------

// Set the thread's stack size in bytes (or zero to use the system default size)
bool AbstractThread::setStackSize(const std::size_t size)
{
   stackSize = size;
   return true;
}

// Set the terminated flag
void AbstractThread::setTerminated()
{
   killed = true;
}

}
}
