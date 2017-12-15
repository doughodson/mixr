
#include "mixr/base/concurrent/SyncTask.hpp"

#include "mixr/base/Component.hpp"
#include <iostream>

namespace mixr {
namespace base {

SyncTask::SyncTask(Component* const p, const double pri) : Thread(p, pri)
{
}

SyncTask::~SyncTask()
{
   closeSignals();
}

//-----------------------------------------------------------------------------
// Configure thread
//-----------------------------------------------------------------------------
bool SyncTask::configThread()
{
//   bool ok{BaseClass::configThread()};
   bool ok{true};                           // added DDH

   // Create the signals
   if (ok) ok = createSignals();

   if (!ok) {
      std::cerr << "SyncTask(" << this << ")::configThread() -- ERROR: Did NOT create the signals!" << std::endl;
   }

   return ok;
}

//-----------------------------------------------------------------------------
// Our main thread function
//-----------------------------------------------------------------------------
unsigned long SyncTask::mainThreadFunc()
{
   unsigned long rtn{};

   // Configure this thread
   bool ok{configThread()};

//   if ( getParent()->isMessageEnabled(MSG_INFO) ) {
      std::cout << "SyncTask(" << this << ")::mainThreadFunc(): thread handle = " << getThreadHandle() << std::endl;
//   }

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
bool SyncTask::terminate()
{
   signalCompleted();
//   return BaseClass::terminate();    commented out ddh
   return true;
}

}
}
