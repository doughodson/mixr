
#include "mixr/base/threads/ISyncThread.hpp"

#include "mixr/base/IObject.hpp"
#include "mixr/base/IComponent.hpp"
#include <iostream>

namespace mixr {
namespace base {

ISyncThread::ISyncThread(IComponent* const p) : IThread(p)
{
}

ISyncThread::~ISyncThread()
{
   closeSignals();
}

//-----------------------------------------------------------------------------
// Configure thread
//-----------------------------------------------------------------------------
bool ISyncThread::configThread()
{
   bool ok{IThread::configThread()};

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
unsigned long ISyncThread::mainThreadFunc()
{
   unsigned long rtn{};

   // Configure this thread
   bool ok{configThread()};

   if ( getParent()->isMessageEnabled(IObject::MSG_INFO) ) {
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
bool ISyncThread::terminate()
{
   signalCompleted();
   return IThread::terminate();
}

}
}
