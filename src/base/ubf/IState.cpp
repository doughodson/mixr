
#include "mixr/base/ubf/IState.hpp"

#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/IComponent.hpp"

namespace mixr {
namespace base {
namespace ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(IState, "IState")
EMPTY_SLOTTABLE(IState)
EMPTY_CONSTRUCTOR(IState)
EMPTY_DELETEDATA(IState)
EMPTY_COPYDATA(IState)

void IState::updateGlobalState()
{
   // Update all my children
   base::PairStream* subcomponents{getComponents()};
   if (subcomponents != nullptr) {
      if (isComponentSelected()) {
         // When we've selected only one
         if (getSelectedComponent() != nullptr) {
            const auto state = dynamic_cast<IState*>(getSelectedComponent());
            if (state != nullptr)
               state->updateGlobalState();
         }
      } else {
         // When we should update them all
         base::List::Item* item{subcomponents->getFirstItem()};
         while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto obj = static_cast<base::IComponent*>(pair->object());
            const auto state = dynamic_cast<IState*>(obj);
            if (state != nullptr)
               state->updateGlobalState();
            item = item->getNext();
         }
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }
}

void IState::updateState(const base::IComponent* const actor)
{
   // Update all my children
   base::PairStream* subcomponents{getComponents()};
   if (subcomponents != nullptr) {
      if (isComponentSelected()) {
         // When we've selected only one
         if (getSelectedComponent() != nullptr) {
            const auto state = dynamic_cast<IState*>(getSelectedComponent());
            if (state != nullptr)
               state->updateState(actor);
         }
      } else {
         // When we should update them all
         base::List::Item* item{subcomponents->getFirstItem()};
         while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto obj = static_cast<base::IComponent*>(pair->object());
            const auto state = dynamic_cast<IState*>(obj);
            if (state != nullptr)
               state->updateState(actor);
            item = item->getNext();
         }
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }
}


const IState* IState::getUbfStateByType(const std::type_info& type) const
{
   const IState* p{this};
   if ( !p->isClassType(type) ) {
      const base::Pair* pair{findByType(type)};
      if (pair != nullptr) {
         p = dynamic_cast<const IState*>( pair->object() );
      }
   }
   return p;
}

}
}
}
