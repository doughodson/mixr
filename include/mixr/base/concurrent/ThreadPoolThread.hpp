
#include "mixr/base/concurrent/SyncTask.hpp"

namespace mixr {
namespace base {
class Object;
class Component;
class ThreadPool;
class ThreadPoolManager;

//------------------------------------------------------------------------------
// Class: ThreadPoolThread
//
// Description: The implementation of ThreadSyncTask used for the ThreadPool
//------------------------------------------------------------------------------
class ThreadPoolThread : public SyncTask
{
   friend class ThreadPool;

public:
   ThreadPoolThread(Component* const parent, ThreadPool* const pool, ThreadPoolManager* const mgr, const double priority, Object* const obj);
   ThreadPoolThread(const ThreadPoolThread&) = delete;
   ThreadPoolThread& operator=(const ThreadPoolThread&) = delete;
   ~ThreadPoolThread();

protected:
   Object* getPersistentObj();
   void setCurrentObj(Object* const);

private:
   virtual unsigned long userFunc() override;

private:
   ThreadPool* threadPool {};
   ThreadPoolManager* manager {};
   Object* persistentObj {};      // The persistent callback object for this thread
   Object* currentObj {};         // The active callback object for this thread
};

}
}
