
#include "mixr/base/Object.hpp"
#include "mixr/base/Component.hpp"

#include "mixr/base/concurrent/ThreadPoolThread.hpp"

#include "mixr/base/concurrent/ThreadPool.hpp"
#include "mixr/base/concurrent/ThreadPoolManager.hpp"

namespace mixr {
namespace base {

ThreadPoolThread::ThreadPoolThread(Component* const parent, ThreadPool* const pool, ThreadPoolManager* const mgr, const double priority, Object* const obj)
   : SyncTask(parent, priority), threadPool(pool), manager(mgr), persistentObj(obj), currentObj(nullptr)
{
}

unsigned long ThreadPoolThread::userFunc()
{
   //Execute the thread callback methods
   if (manager != nullptr) {
      manager->execute(persistentObj);
      manager->execute(persistentObj, currentObj);
   }

   //Clear the current callback object because we're done with it
   currentObj = nullptr;

   //Add the thread back to the pool
   threadPool->threadAvailable(this);

   return 0;
}

/*
void ThreadPoolThread::deleteData()
{
   if (manager != nullptr)
      manager->destroy(persistentObj);
}
*/

Object* ThreadPoolThread::getPersistentObj()
{
   return persistentObj;
}

void ThreadPoolThread::setCurrentObj(Object* const obj)
{
   currentObj = obj;
}

}
}
