
#include "mixr/base/units/Energies.hpp"

#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Energy() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Energy, "AbstractEnergy")
EMPTY_SLOTTABLE(Energy)
EMPTY_DELETEDATA(Energy)

Energy::Energy()
{
    STANDARD_CONSTRUCTOR()
}

Energy::Energy(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Energy::copyData(const Energy& org, const bool)
{
   BaseClass::copyData(org);
   val = fromEnergy(org.toEnergy());;
}

//==============================================================================
// KiloWattHours() --
//==============================================================================
IMPLEMENT_SUBCLASS(KiloWattHours, "KiloWattHours")
EMPTY_SLOTTABLE(KiloWattHours)
EMPTY_COPYDATA(KiloWattHours)
EMPTY_DELETEDATA(KiloWattHours)

KiloWattHours::KiloWattHours() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

KiloWattHours::KiloWattHours(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloWattHours::KiloWattHours(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// BTUs() --
//==============================================================================
IMPLEMENT_SUBCLASS(BTUs, "BTUs")
EMPTY_SLOTTABLE(BTUs)
EMPTY_COPYDATA(BTUs)
EMPTY_DELETEDATA(BTUs)

BTUs::BTUs() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

BTUs::BTUs(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

BTUs::BTUs(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Calories() --
//==============================================================================
IMPLEMENT_SUBCLASS(Calories, "Calories")
EMPTY_SLOTTABLE(Calories)
EMPTY_COPYDATA(Calories)
EMPTY_DELETEDATA(Calories)

Calories::Calories() : Energy()
{
    STANDARD_CONSTRUCTOR()
}


Calories::Calories(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

Calories::Calories(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Joules() --
//==============================================================================
IMPLEMENT_SUBCLASS(Joules, "Joules")
EMPTY_SLOTTABLE(Joules)
EMPTY_COPYDATA(Joules)
EMPTY_DELETEDATA(Joules)

Joules::Joules() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

Joules::Joules(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

Joules::Joules(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// FootPounds() --
//==============================================================================
IMPLEMENT_SUBCLASS(FootPounds, "FootPounds")
EMPTY_SLOTTABLE(FootPounds)
EMPTY_COPYDATA(FootPounds)
EMPTY_DELETEDATA(FootPounds)

FootPounds::FootPounds() : Energy()
{
    STANDARD_CONSTRUCTOR()
}

FootPounds::FootPounds(const double value) : Energy(value)
{
    STANDARD_CONSTRUCTOR()
}

FootPounds::FootPounds(const Energy& value) : Energy()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

