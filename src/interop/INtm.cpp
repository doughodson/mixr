
#include "mixr/interop/INtm.hpp"

#include "mixr/models/player/Player.hpp"

namespace mixr {
namespace interop {

IMPLEMENT_ABSTRACT_SUBCLASS(INtm, "INtm")

BEGIN_SLOTTABLE(INtm)
   "template",       // 1) Template player
END_SLOTTABLE(INtm)

BEGIN_SLOT_MAP(INtm)
    ON_SLOT(1, setSlotTemplatePlayer, models::Player)
END_SLOT_MAP()

INtm::INtm()
{
   STANDARD_CONSTRUCTOR()
}

void INtm::copyData(const INtm& org, const bool)
{
   BaseClass::copyData(org);

   setSlotTemplatePlayer( org.tPlayer );
}

void INtm::deleteData()
{
   setSlotTemplatePlayer( nullptr );
}

//------------------------------------------------------------------------------
// Set Slot functions
//------------------------------------------------------------------------------

// Sets the template player
bool INtm::setSlotTemplatePlayer(const models::Player* const msg)
{
   tPlayer = msg;
   return true;
}

}
}
