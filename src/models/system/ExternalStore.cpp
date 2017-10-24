
#include "mixr/models/system/ExternalStore.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/String.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ExternalStore, "ExternalStore")

BEGIN_SLOTTABLE(ExternalStore)
   "type",        // 1) Type of external store string
   "jettisonable" // 2) External store can be jettisoned (default: true)
END_SLOTTABLE(ExternalStore)

BEGIN_SLOT_MAP(ExternalStore)
   ON_SLOT( 1, setSlotType,         base::String)
   ON_SLOT( 2, setSlotJettisonable, base::Number )
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(ExternalStore)
   ON_EVENT( JETTISON_EVENT, onJettisonEvent)
END_EVENT_HANDLER()

ExternalStore::ExternalStore()
{
   STANDARD_CONSTRUCTOR()
}

void ExternalStore::copyData(const ExternalStore& org, const bool)
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

void ExternalStore::deleteData()
{
   type = nullptr;
}

//------------------------------------------------------------------------------
// Reset
//------------------------------------------------------------------------------
void ExternalStore::reset()
{
   BaseClass::reset();

   jettisoned = false;
}

//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------
bool ExternalStore::onJettisonEvent()
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
const base::String* ExternalStore::getType() const
{
   return type;
}

// True if the weapon can be jettisioned
bool ExternalStore::isJettisonable() const
{
   return canJettison;
}

// True if the weapon has been jettisioned
bool ExternalStore::isJettisoned() const
{
   return jettisoned;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the jettision enable flag
bool ExternalStore::setJettisonable(const bool f)
{
   canJettison = f;
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

// Set type string
bool ExternalStore::setSlotType(const base::String* const msg)
{
   type = msg;
   return true;
}

// jettisonable: weapon can be jettisoned
bool ExternalStore::setSlotJettisonable(base::Number* const p)
{
   setJettisonable( p->getBoolean() );
   return true;
}

}
}
