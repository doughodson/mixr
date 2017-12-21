
#ifndef __mixr_base_ubf_AbstractAction_H__
#define __mixr_base_ubf_AbstractAction_H__

#include "mixr/base/Object.hpp"

namespace mixr {

namespace base {
class Component;

namespace ubf {

//------------------------------------------------------------------------------
// Class: AbstractAction
//
// Description:
//    Abstract base class for all Actions. They are responsible for
//    their own execution.
//
// Factory name: UbfAction
//------------------------------------------------------------------------------
class AbstractAction : public base::Object
{
   DECLARE_SUBCLASS(AbstractAction, base::Object)

public:
   AbstractAction();

   int getVote() const;
   void setVote(const int x);

   // Execute the behavior
   virtual bool execute(base::Component* actor)=0;

private:
   int vote{};
};

inline void AbstractAction::setVote(const int x)      { vote = x; return; }
inline int AbstractAction::getVote() const            { return vote; }

}
}
}

#endif

