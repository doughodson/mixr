
#ifndef __mixr_base_AbstractThread_H__
#define __mixr_base_AbstractThread_H__

#include "mixr/base/Referenced.hpp"
#include "mixr/base/util/platform_api.hpp"

#include <cstddef>

namespace mixr {
namespace base {
class Component;

//------------------------------------------------------------------------------
// Class: AbstractThread
//
// Description: Abstract thread class - defines common functionality needed
//              by more specific (althrough still abstract) thread classes.
//              Thread objects and their parent objects are ref()'d as child
//              threads start, and are unref()'d when the thread terminates.
//              This prevents the objects from being deleted before the thread
//              terminates.
//
//    On Windows applications, the process is converted to a "High priority
//    class" process during the creation of the first Thread.
//
//    The constructor requires several parameters, and because of the nature of
//    threads, this class can not be copied or assigned.
//
//
// Thread priority mapping: range [ 0.0 ... 1.0 ] (default: 0.5 -> Normal)
//
//       Priority       Windows(HIGH_PRIORITY_CLASS)              Linux
//    -------------     ------------------------------      --------------------
//        1.0           THREAD_PRIORITY_TIME_CRITICAL(15)   SCHED_FIFO( max FIFO pri )
//    [ 0.9, 1.0 )      THREAD_PRIORITY_HIGHEST(2)          SCHED_FIFO( 90% FIFO pri )
//    [ 0.8, 0.9 )      THREAD_PRIORITY_HIGHEST(2)          SCHED_FIFO( 80% FIFO pri )
//    [ 0.7, 0.8 )      THREAD_PRIORITY_ABOVE_NORMAL(1)     SCHED_FIFO( 70% FIFO pri )
//    [ 0.6, 0.7 )      THREAD_PRIORITY_ABOVE_NORMAL(1)     SCHED_FIFO( 60% FIFO pri )
//    [ 0.5, 0.6 )      THREAD_PRIORITY_NORMAL(0)           SCHED_FIFO( 50% FIFO pri )
//    [ 0.4, 0.5 )      THREAD_PRIORITY_NORMAL(0)           SCHED_FIFO( 40% FIFO pri )
//    [ 0.3, 0.4 )      THREAD_PRIORITY_BELOW_NORMAL(-1)    SCHED_FIFO( 30% FIFO pri )
//    [ 0.2, 0.3 )      THREAD_PRIORITY_BELOW_NORMAL(-1)    SCHED_FIFO( 20% FIFO pri )
//    [ 0.1, 0.2 )      THREAD_PRIORITY_LOWEST(-2)          SCHED_FIFO( 10% FIFO pri )
//    ( 0.0, 0.1 )      THREAD_PRIORITY_LOWEST(-2)          SCHED_FIFO( min FIFO pri )
//        0.0           THREAD_PRIORITY_IDLE(-15)           SCHED_OTHER(0)
//
//    Windows Only: the default process priority class is 'HIGH_PRIORITY_CLASS'.  To use
//    the Windows highest process priority class, 'REALTIME_PRIORITY_CLASS', define the
//    macro 'USE_REALTIME_PRI_CLASS' in the file 'src/base/windows/Thread.cxx', which
//    will set REALTIME_PRIORITY_CLASS class and use the thread priorities below.
//
//            Priority        Windows(REALTIME_PRIORITY_CLASS)
//          -------------     --------------------------------
//              1.0           THREAD_PRIORITY_TIME_CRITICAL(15)
//          [ 0.9, 1.0 )                (5)
//          [ 0.8, 0.9 )                (4)
//          [ 0.7, 0.8 )                (3)
//          [ 0.6, 0.7 )      THREAD_PRIORITY_HIGHEST(2)
//          ( 0.5, 0.6 )      THREAD_PRIORITY_ABOVE_NORMAL(1)
//              0.5           THREAD_PRIORITY_NORMAL(0)
//          [ 0.4, 0.5 )      THREAD_PRIORITY_BELOW_NORMAL(-1)
//          [ 0.3, 0.4 )      THREAD_PRIORITY_LOWEST(-2)
//          [ 0.2, 0.3 )               (-3)
//          [ 0.1, 0.2 )               (-4)
//          ( 0.0, 0.1 )               (-5)
//              0.0           THREAD_PRIORITY_IDLE(-15)
//
//------------------------------------------------------------------------------
class AbstractThread : public Referenced
{
public:
   AbstractThread(Component* const parent);
   AbstractThread(const AbstractThread&) = delete;
   AbstractThread& operator=(const AbstractThread&) = delete;
   ~AbstractThread();

   bool isTerminated() const;

   // thread stack size in bytes (or zero if using the default stack size)
   std::size_t getStackSize() const;

   // starts thread execution with a given priority
   bool start(const double priority);

   // Terminate the child thread
   virtual bool terminate();

   // Set the thread's stack size (zero to use the system default size)
   // -- set before creating the thread --
   bool setStackSize(const std::size_t size);

   // number of processors assigned to this process
   static int getNumProcessors();

protected:
   Component* getParent();
   const void* getThreadHandle() const;

   virtual void setTerminated(); // Indicate that the thread has terminated natually

   virtual bool configThread();  // called by the child thread

private:
   bool createThread();
   void closeThread();

   // Our main thread function
   virtual unsigned long mainThreadFunc() =0;

   // The static thread function ---
   #if defined(WIN32)
      static DWORD WINAPI staticThreadFunc(LPVOID lpParam);
   #else
      static void* staticThreadFunc(void* lpParam);
   #endif

private:
   Component* parent{};        // Our parent component that created us.
   double priority{};          // Thread priority (0->lowest, 1->highest)
   bool killed{};              // Are we terminated?
   std::size_t stackSize{};    // Stack size in bytes (zero to use the system default stack size)

   // Implementation dependent
   void* theThread{};          // thread handle
};

}
}

#endif

