
#include "mixr/base/units/Masses.hpp"

#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Mass() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Mass, "AbstractMass")
EMPTY_SLOTTABLE(Mass)
EMPTY_DELETEDATA(Mass)

Mass::Mass()
{
    STANDARD_CONSTRUCTOR()
}

Mass::Mass(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Mass::copyData(const Mass& org, const bool)
{
    BaseClass::copyData(org);
    val = fromMass(org.toMass());;
}

//==============================================================================
// KiloGrams() --
//==============================================================================
IMPLEMENT_SUBCLASS(KiloGrams, "KiloGrams")
EMPTY_SLOTTABLE(KiloGrams)
EMPTY_COPYDATA(KiloGrams)
EMPTY_DELETEDATA(KiloGrams)

KiloGrams::KiloGrams() : Mass()
{
    STANDARD_CONSTRUCTOR()
}

KiloGrams::KiloGrams(const double value) : Mass(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloGrams::KiloGrams(const Mass& value) : Mass()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Grams() --
//==============================================================================
IMPLEMENT_SUBCLASS(Grams, "Grams")
EMPTY_SLOTTABLE(Grams)
EMPTY_COPYDATA(Grams)
EMPTY_DELETEDATA(Grams)

Grams::Grams() : Mass()
{
    STANDARD_CONSTRUCTOR()
}

Grams::Grams(const double value) : Mass(value)
{
    STANDARD_CONSTRUCTOR()
}

Grams::Grams(const Mass& value) : Mass()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Slugs() --
//==============================================================================
IMPLEMENT_SUBCLASS(Slugs, "Slugs")
EMPTY_SLOTTABLE(Slugs)
EMPTY_COPYDATA(Slugs)
EMPTY_DELETEDATA(Slugs)

Slugs::Slugs() : Mass()
{
    STANDARD_CONSTRUCTOR()
}

Slugs::Slugs(const double value) : Mass(value)
{
    STANDARD_CONSTRUCTOR()
}

Slugs::Slugs(const Mass& value) : Mass()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
