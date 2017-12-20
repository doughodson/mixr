
#include "mixr/base/threads/SyncThread.hpp"

#include "mixr/base/Object.hpp"
#include "mixr/base/Component.hpp"
#include <iostream>

namespace mixr {
namespace base {

SyncThread::SyncThread(Component* const p) : AbstractThread(p)
{
}

SyncThread::~SyncThread()
{
   closeSignals();
}

//-----------------------------------------------------------------------------
// Configure thread
//-----------------------------------------------------------------------------
bool SyncThread::configThread()
{
   bool ok{AbstractThread::configThread()};

   // Create the signals
   if (ok) ok = createSignals();

   if (!ok) {
      std::cerr << "SyncThread(" << this << ")::configThread() -- ERROR: Did NOT create the signals!" << std::endl;
   }

   return ok;
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long SyncThread::mainThreadFunc()
{
   unsigned long rtn{};

   // Configure this thread
   bool ok{configThread()};

   if ( getParent()->isMessageEnabled(Object::MSG_INFO) ) {
      std::cout << "SyncThread(" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
   }

   // Main start-complete loop ...
   while ( ok && getParent()->isNotShutdown() ) {

      // Wait for the start signal
      waitForStart();

      // Just in case we've been shutdown while we were waiting
      if (getParent()->isShutdown()) {
         signalCompleted();
         break;
      }

      // User defined tasks
      this->userFunc();

      // Signal that we've completed
      signalCompleted();
   }

   return rtn;
}

//-----------------------------------------------------------------------------
// Terminate the thread
//-----------------------------------------------------------------------------
bool SyncThread::terminate()
{
   signalCompleted();
   return AbstractThread::terminate();
}

}
}
