
#ifndef __mixr_base_ubf_IAction_HPP__
#define __mixr_base_ubf_IAction_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {

namespace base {
class IComponent;

namespace ubf {

//------------------------------------------------------------------------------
// Class: IAction
// Description:
//    Interface class for all Actions. They are responsible for
//    their own execution.
//------------------------------------------------------------------------------
// Factory name: IAction
//------------------------------------------------------------------------------
class IAction : public base::Object
{
   DECLARE_SUBCLASS(IAction, base::Object)

public:
   IAction();

   int getVote() const;
   void setVote(const int);

   // Execute the behavior
   virtual bool execute(base::IComponent* actor)=0;

private:
   int vote{};
};

inline void IAction::setVote(const int x)      { vote = x; return; }
inline int IAction::getVote() const            { return vote; }

}
}
}

#endif

