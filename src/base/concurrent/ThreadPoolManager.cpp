
#include "mixr/base/Object.hpp"

#include "mixr/base/concurrent/ThreadPoolManager.hpp"

namespace mixr {
namespace base {

ThreadPoolManager::ThreadPoolManager()
{
}

//------------------------------------------------------------------------------
// Default callback implementations (do nothing)
//------------------------------------------------------------------------------

Object* ThreadPoolManager::initialize()
{
   return nullptr;
}

void ThreadPoolManager::prepare(Object* const)
{
}

void ThreadPoolManager::execute(Object* const)
{
}

void ThreadPoolManager::execute(Object* const, Object*)
{
}

void ThreadPoolManager::destroy(Object* const)
{
}

}
}
