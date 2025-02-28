
#ifndef __mixr_base_IOneShotThread_HPP__
#define __mixr_base_IOneShotThread_HPP__

#include "mixr/base/threads/IThread.hpp"

namespace mixr {
namespace base {
class IComponent;

//------------------------------------------------------------------------------
// Class: IOneShotThread
//
// Description: Interface thread that executes a single task. Derived classes
//              need to define the work function (i.e., userFunc()), which is
//              called once. (Note: This class was named 'One Shot' to avoid
//              confusion with a new class to be introduced called 'Thread' that
//              will use std::thread functionality.)
//------------------------------------------------------------------------------
class IOneShotThread : public IThread
{
public:
   IOneShotThread(IComponent* const parent);
   IOneShotThread(const IOneShotThread&) = delete;
   IOneShotThread& operator=(const IOneShotThread&) = delete;

private:
   unsigned long mainThreadFunc() final;

   // user defined work function
   virtual unsigned long userFunc() =0;
};

}
}

#endif

