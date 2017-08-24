
#include "mixr/base/units/Powers.hpp"
#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Power() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Power, "Power")
EMPTY_SLOTTABLE(Power)
EMPTY_DELETEDATA(Power)

Power::Power()
{
   STANDARD_CONSTRUCTOR()
}

Power::Power(const double value) : Number(value)
{
   STANDARD_CONSTRUCTOR()
}

void Power::copyData(const Power& org, const bool)
{
   BaseClass::copyData(org);
   val = fromPower(org.toPower());;
}

//==============================================================================
// KiloWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(KiloWatts, "KiloWatts")
EMPTY_COPYDATA(KiloWatts)
EMPTY_DELETEDATA(KiloWatts)

KiloWatts::KiloWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

KiloWatts::KiloWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloWatts::KiloWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// Watts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Watts, "Watts")
EMPTY_COPYDATA(Watts)
EMPTY_DELETEDATA(Watts)

Watts::Watts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

Watts::Watts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

Watts::Watts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// Horsepower() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Horsepower, "Horsepower")
EMPTY_COPYDATA(Horsepower)
EMPTY_DELETEDATA(Horsepower)

Horsepower::Horsepower() : Power()
{
    STANDARD_CONSTRUCTOR()
}


Horsepower::Horsepower(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

Horsepower::Horsepower(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// DecibelWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DecibelWatts, "DecibelWatts")
EMPTY_COPYDATA(DecibelWatts)
EMPTY_DELETEDATA(DecibelWatts)

DecibelWatts::DecibelWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

DecibelWatts::DecibelWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelWatts::DecibelWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// MilliWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MilliWatts, "MilliWatts")
EMPTY_COPYDATA(MilliWatts)
EMPTY_DELETEDATA(MilliWatts)

MilliWatts::MilliWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

MilliWatts::MilliWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

MilliWatts::MilliWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

//==============================================================================
// DecibelMilliWatts() --
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DecibelMilliWatts, "DecibelMilliWatts")
EMPTY_COPYDATA(DecibelMilliWatts)
EMPTY_DELETEDATA(DecibelMilliWatts)

DecibelMilliWatts::DecibelMilliWatts() : Power()
{
    STANDARD_CONSTRUCTOR()
}

DecibelMilliWatts::DecibelMilliWatts(const double value) : Power(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelMilliWatts::DecibelMilliWatts(const Power& org) : Power()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(org,true);
}

}
}
