
#include "mixr/models/system/IExternalStore.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/String.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IExternalStore, "IExternalStore")

BEGIN_SLOTTABLE(IExternalStore)
   "type",          // 1) Type of external store string
   "jettisonable"   // 2) External store can be jettisoned (default: true)
END_SLOTTABLE(IExternalStore)

BEGIN_SLOT_MAP(IExternalStore)
   ON_SLOT( 1, setSlotType,         base::String)
   ON_SLOT( 2, setSlotJettisonable, base::Boolean )
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(IExternalStore)
   ON_EVENT( JETTISON_EVENT, onJettisonEvent)
END_EVENT_HANDLER()

IExternalStore::IExternalStore()
{
   STANDARD_CONSTRUCTOR()
}

void IExternalStore::copyData(const IExternalStore& org, const bool)
{
   BaseClass::copyData(org);

   {
      const base::String* p{};
      if (org.type != nullptr) p = org.type->clone();
      setSlotType( p );
      if (p != nullptr) p->unref();
   }

   canJettison  = org.canJettison;
   jettisoned = org.jettisoned;
}

void IExternalStore::deleteData()
{
   type = nullptr;
}

//------------------------------------------------------------------------------
// Reset
//------------------------------------------------------------------------------
void IExternalStore::reset()
{
   BaseClass::reset();

   jettisoned = false;
}

//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------
bool IExternalStore::onJettisonEvent()
{
   bool ok{};
   if (canJettison && !jettisoned) {
      // If we can be jettisoned and we haven't already been ...
      jettisoned =  true;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Get the type string
const base::String* IExternalStore::getType() const
{
   return type;
}

// True if the weapon can be jettisioned
bool IExternalStore::isJettisonable() const
{
   return canJettison;
}

// True if the weapon has been jettisioned
bool IExternalStore::isJettisoned() const
{
   return jettisoned;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the jettision enable flag
bool IExternalStore::setJettisonable(const bool f)
{
   canJettison = f;
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// Set type string
bool IExternalStore::setSlotType(const base::String* const msg)
{
   type = msg;
   return true;
}

// jettisonable: weapon can be jettisoned
bool IExternalStore::setSlotJettisonable(base::Boolean* const p)
{
   setJettisonable( p->asBool() );
   return true;
}

}
}
