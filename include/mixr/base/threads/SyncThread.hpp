
#ifndef __mixr_base_SyncTask_H__
#define __mixr_base_SyncTask_H__

#include "mixr/base/threads/AbstractThread.hpp"

namespace mixr {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: SyncThread
//
// Description: Abstract synchronized (i.e., start/complete loop) thread;
//              multiple calls to the user defined work function with
//              start/complete signals.
//
//    Derived classes need to define the work function (i.e., userFunc()),
//    which is called once after each 'start' signal (i.e., signalStart()).
//    This class will then signal 'completed' after each return
//    from the userFunc(). Users can use waitForCompleted() to wait for the
//    'completed' signal, or use the static function waitForAllCompleted() to
//    wait for several sync task threads.  Loop will end with the shutdown of
//    the parent.
//------------------------------------------------------------------------------
class SyncThread : public AbstractThread
{
public:
   SyncThread(Component* const parent);
   SyncThread(const SyncThread&) = delete;
   SyncThread& operator=(const SyncThread&) = delete;
   ~SyncThread();

   void signalStart();
   void waitForCompleted();
   static void waitForAllCompleted(SyncThread** threads, const int num);

   //Returns the index of the first thread that is completed, or -1 if an error
   static int waitForAnyCompleted(SyncThread** threads, const int num);

   bool terminate() override;

protected:
   void waitForStart();
   void signalCompleted();

   bool configThread() override;

private:
   unsigned long mainThreadFunc() final;

   // user defined work function
   virtual unsigned long userFunc() =0;

   bool createSignals();
   void closeSignals();

   // Implementation dependent
   void* startSig {};      // Start signal
   void* completedSig {};  // completed signal
};

}
}

#endif
