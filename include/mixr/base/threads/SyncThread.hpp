
#ifndef __mixr_base_SyncTask_H__
#define __mixr_base_SyncTask_H__

#include "mixr/base/threads/AbstractThread.hpp"

namespace mixr {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: SyncThread
//
// Description: Abstract sync (i.e., start/complete loop) thread;
//
//    User's will need to derive a class from this class and define the
//    work function, userFunc(), which is called once after each 'start' signal,
//    signalStart().  This class will then signal 'completed' after each return
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

   // User defined work function
   private: virtual unsigned long userFunc() =0;

protected:
   void waitForStart();
   void signalCompleted();

   bool configThread() override;

private:
   bool createSignals();
   void closeSignals();

   unsigned long mainThreadFunc() final;

   // Implementation dependent
   void* startSig {};      // Start signal
   void* completedSig {};  // completed signal
};

}
}

#endif
