
#ifndef __mixr_base_ISyncTask_HPP__
#define __mixr_base_ISyncTask_HPP__

#include "mixr/base/threads/IThread.hpp"

namespace mixr {
namespace base {
class IComponent;

//------------------------------------------------------------------------------
// Class: ISyncThread
//
// Description: Interface that defines a synchronized (i.e., start/complete loop) thread;
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
class ISyncThread : public IThread
{
public:
   ISyncThread(IComponent* const parent);
   ISyncThread(const ISyncThread&) = delete;
   ISyncThread& operator=(const ISyncThread&) = delete;
   ~ISyncThread();

   void signalStart();
   void waitForCompleted();
   static void waitForAllCompleted(ISyncThread** threads, const int num);

   //Returns the index of the first thread that is completed, or -1 if an error
   static int waitForAnyCompleted(ISyncThread** threads, const int num);

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
