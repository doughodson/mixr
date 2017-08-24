
#include "mixr/interop/common/Ntm.hpp"

#include "mixr/models/player/Player.hpp"

namespace mixr {
namespace interop {

IMPLEMENT_ABSTRACT_SUBCLASS(Ntm, "Ntm")

BEGIN_SLOTTABLE(Ntm)
   "template",       // 1) Template player
END_SLOTTABLE(Ntm)

BEGIN_SLOT_MAP(Ntm)
    ON_SLOT(1, setSlotTemplatePlayer, models::Player)
END_SLOT_MAP()

Ntm::Ntm()
{
   STANDARD_CONSTRUCTOR()
}

void Ntm::copyData(const Ntm& org, const bool)
{
   BaseClass::copyData(org);

   setSlotTemplatePlayer( org.tPlayer );
}

void Ntm::deleteData()
{
   setSlotTemplatePlayer( nullptr );
}

//------------------------------------------------------------------------------
// Set Slot functions
//------------------------------------------------------------------------------

// Sets the template player
bool Ntm::setSlotTemplatePlayer(const models::Player* const msg)
{
   tPlayer = msg;
   return true;
}

}
}
