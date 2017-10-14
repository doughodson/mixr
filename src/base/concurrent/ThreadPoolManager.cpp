
#include "mixr/base/concurrent/ThreadPoolManager.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(ThreadPoolManager, "ThreadPoolManager")
EMPTY_SLOTTABLE(ThreadPoolManager)
EMPTY_CONSTRUCTOR(ThreadPoolManager)
EMPTY_COPYDATA(ThreadPoolManager)
EMPTY_DELETEDATA(ThreadPoolManager)

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
