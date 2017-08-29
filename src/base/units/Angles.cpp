
#include "mixr/base/units/Angles.hpp"

#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Angle --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Angle, "AbstractAngle")
EMPTY_SLOTTABLE(Angle)
EMPTY_DELETEDATA(Angle)

Angle::Angle() : Number()
{
   STANDARD_CONSTRUCTOR()
}

Angle::Angle(const double value) : Number(value)
{
   STANDARD_CONSTRUCTOR()
}

void Angle::copyData(const Angle& org, const bool)
{
   BaseClass::copyData(org);
   val = fromAngle(org.toAngle());;
}

//==============================================================================
// Degrees --
//==============================================================================
IMPLEMENT_SUBCLASS(Degrees, "Degrees")
EMPTY_SLOTTABLE(Degrees)
EMPTY_COPYDATA(Degrees)
EMPTY_DELETEDATA(Degrees)

Degrees::Degrees() : Angle()
{
    STANDARD_CONSTRUCTOR()
}

Degrees::Degrees(const double value) : Angle(value)
{
    STANDARD_CONSTRUCTOR()
}

Degrees::Degrees(const Angle& value) : Angle()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Radians --
//==============================================================================
IMPLEMENT_SUBCLASS(Radians, "Radians")
EMPTY_SLOTTABLE(Radians)
EMPTY_COPYDATA(Radians)
EMPTY_DELETEDATA(Radians)

Radians::Radians() : Angle()
{
    STANDARD_CONSTRUCTOR()
}

Radians::Radians(const double value) : Angle(value)
{
    STANDARD_CONSTRUCTOR()
}

Radians::Radians(const Angle& value) : Angle()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Semicircles --
//==============================================================================
IMPLEMENT_SUBCLASS(Semicircles, "Semicircles")
EMPTY_SLOTTABLE(Semicircles)
EMPTY_COPYDATA(Semicircles)
EMPTY_DELETEDATA(Semicircles)

Semicircles::Semicircles() : Angle()
{
    STANDARD_CONSTRUCTOR()
}

Semicircles::Semicircles(const double value) : Angle(value)
{
    STANDARD_CONSTRUCTOR()
}

Semicircles::Semicircles(const Angle& value) : Angle()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
