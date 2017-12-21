
#ifndef __mixr_ighost_cigi_TypeMapper_H__
#define __mixr_ighost_cigi_TypeMapper_H__

#include "mixr/base/Object.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class Identifier; class Number; class String; }
namespace models { class Player; }
namespace cigi {

//------------------------------------------------------------------------------
// Class: TypeMapper
//
// Description: Cigi Type-Mapper --
//              Maps player's factory name and type string to Cigi IG entity type ID.
//
// Factory name: CigiTypeMapper
// Slots:
//     factoryName   <Identifier>     ! Reference factory name (default: 0)
//     typeName      <String>         ! Reference type name (default: 0)
//     modelTypeId   <Number>         ! Entity type ID number (int) (default: 0)
//
// Examples:
//    ( Otm  Aircraft  "B-1B"          203 )
//    ( Otm  Aircraft  "F-14"          309 )
//    ( Otm  Aircraft     modelTypeId: 302 )       // Default aircraft type
//
//
// Notes:
//   1) If typeName isn't given, only the factoryName will be used.
//
//
// Notes on comparing player types -- isMatchingPlayerType()
//
//    1) Derived classes and more specific type should be checked first
//       (see #2 & #5 below -- we get matches with the base classes and
//        the more general type names)
//
//    2) Factory names will also match base class factory names
//
//    3) Factory names are compared first and then the type names are compared
//
//    4) It's considered a match if the factory names match and no type name as given.
//
//    5) For type names, we compare at most the length of our type name ...
//          (e.g., Test player's "F-16C" would match our "F-16C"
//          (e.g., Test player's "F-16C" would match our general "F-16" type)
//          (e.g., Test player's general "F-16" type would not match our "F-16C")
//
//------------------------------------------------------------------------------
class TypeMapper : public base::Object
{
    DECLARE_SUBCLASS(TypeMapper, base::Object)

public:
    TypeMapper();

    int getTypeId() const                                    { return typeId; }          // IG entity type ID number
    bool setTypeId(const int newType);                                                   // Sets the IG entity type number

    const base::Identifier* getRefFactoryName() const        { return refFactoryName; }  // Reference factory name

    const base::String* getRefTypeName() const               { return refTypeName; }     // Reference type name

    // True if player's factory & type names match our reference factory and type names.
    virtual bool isMatchingPlayerType(const models::Player* const) const;

private:
    base::safe_ptr<const base::Identifier> refFactoryName;    // Reference factory name
    base::safe_ptr<const base::String> refTypeName;           // Reference type name (e.g., "F-16C", "T-71")
    int typeId{};                                             // IG entity type ID number

private:
    // slot table helper methods
    bool setSlotTypeId(const base::Number* const);              // Sets the IG entity type number
    bool setSlotRefFactoryName(const base::Identifier* const);  // Sets the Reference factory name
    bool setSlotRefTypeName(const base::String* const);         // Sets the Reference type name
};

}
}

#endif
