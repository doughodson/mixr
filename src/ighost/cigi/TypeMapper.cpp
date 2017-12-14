
#include "mixr/ighost/cigi/TypeMapper.hpp"

#include "mixr/models/player/Player.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace cigi {

IMPLEMENT_SUBCLASS(TypeMapper, "CigiTypeMapper")

BEGIN_SLOTTABLE(TypeMapper)
   "factoryName",       // 1) Reference factory name
   "typeName",          // 2) Reference type name
   "modelTypeId",       // 3) IG entity type ID number
END_SLOTTABLE(TypeMapper)

BEGIN_SLOT_MAP(TypeMapper)
    ON_SLOT(1, setSlotRefFactoryName, base::Identifier)
    ON_SLOT(2, setSlotRefTypeName,    base::String)
    ON_SLOT(3, setSlotTypeId,         base::Number)
END_SLOT_MAP()

TypeMapper::TypeMapper()
{
   STANDARD_CONSTRUCTOR()
}

void TypeMapper::copyData(const TypeMapper& org, const bool)
{
   BaseClass::copyData(org);

   setTypeId( org.typeId );
   setSlotRefFactoryName( org.refFactoryName );
   setSlotRefTypeName( org.refTypeName );
}

void TypeMapper::deleteData()
{
   setTypeId( 0 );
   setSlotRefFactoryName( nullptr );
   setSlotRefTypeName( nullptr );
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the IG entity type number
bool TypeMapper::setTypeId(const int newType)
{
   typeId = newType;
   return true;
}

// Sets the IG entity type number
bool TypeMapper::setSlotTypeId(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int i{msg->getInt()};
      if (i >= 0) {
         ok = setTypeId(i);
      }
   }
   return ok;
}

// Sets the player's factory name
bool TypeMapper::setSlotRefFactoryName(const base::Identifier* const msg)
{
   refFactoryName = msg;
   return true;
}

// Sets the player's type name
bool TypeMapper::setSlotRefTypeName(const base::String* const msg)
{
   refTypeName = msg;
   return true;
}

//------------------------------------------------------------------------------
// isMatchingPlayerType() -- Returns true if the factory & type names match
//------------------------------------------------------------------------------
bool TypeMapper::isMatchingPlayerType(const models::Player* const p) const
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
