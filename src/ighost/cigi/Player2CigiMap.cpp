
#include "mixr/ighost/cigi/Player2CigiMap.hpp"

#include "mixr/models/player/Player.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace cigi {

IMPLEMENT_SUBCLASS(Player2CigiMap, "Player2CigiMap")

BEGIN_SLOTTABLE(Player2CigiMap)
   "factoryName",       // 1) Reference factory name
   "typeName",          // 2) Reference type name
   "entityId",          // 3) IG entity type ID number
END_SLOTTABLE(Player2CigiMap)

BEGIN_SLOT_MAP(Player2CigiMap)
    ON_SLOT(1, setSlotRefFactoryName, base::Identifier)
    ON_SLOT(2, setSlotRefTypeName,    base::String)
    ON_SLOT(3, setSlotEntityId,       base::Number)
END_SLOT_MAP()

Player2CigiMap::Player2CigiMap()
{
   STANDARD_CONSTRUCTOR()
}

void Player2CigiMap::copyData(const Player2CigiMap& org, const bool)
{
   BaseClass::copyData(org);

   setEntityId( org.entityId );
   setSlotRefFactoryName( org.refFactoryName );
   setSlotRefTypeName( org.refTypeName );
}

void Player2CigiMap::deleteData()
{
   setEntityId( 0 );
   setSlotRefFactoryName( nullptr );
   setSlotRefTypeName( nullptr );
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the IG entity type number
bool Player2CigiMap::setEntityId(const int id)
{
   entityId = id;
   return true;
}

// Sets the IG entity type number
bool Player2CigiMap::setSlotEntityId(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int i{msg->getInt()};
      if (i >= 0) {
         ok = setEntityId(i);
      }
   }
   return ok;
}

// Sets the player's factory name
bool Player2CigiMap::setSlotRefFactoryName(const base::Identifier* const msg)
{
   refFactoryName = msg;
   return true;
}

// Sets the player's type name
bool Player2CigiMap::setSlotRefTypeName(const base::String* const msg)
{
   refTypeName = msg;
   return true;
}

//------------------------------------------------------------------------------
// isMatchingPlayerType() -- Returns true if the factory & type names match
//------------------------------------------------------------------------------
bool Player2CigiMap::isMatchingPlayerType(const models::Player* const p) const
{
   bool match{};
   if (p != nullptr && refFactoryName != nullptr) {
      // first match the factory name --
      if (p->isFactoryName( *refFactoryName ) ) {

         // The factory names match!

         // By default we match, unless we have both type names to match,
         match = true;

         // Do we have both type names?
         const base::String* ptype{p->getType()};
         if ( refTypeName != nullptr && ptype != nullptr) {

            // Then compare at most the length of our reference type name ...
            match = std::strncmp( ptype->getString(), refTypeName->getString(), refTypeName->len() ) == 0;

         }
      }
   }
   return match;
}

}
}
