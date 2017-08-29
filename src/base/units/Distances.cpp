//------------------------------------------------------------------------------
// Distance, Meters, CentiMeters, KiloMeters, Inches, Feet, NauticalMiles,
// StatuteMiles 
//------------------------------------------------------------------------------
#include "mixr/base/units/Distances.hpp"
#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Distance --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Distance, "AbstractDistance")
EMPTY_SLOTTABLE(Distance)
EMPTY_DELETEDATA(Distance)

Distance::Distance() : Number()
{
    STANDARD_CONSTRUCTOR()
}

Distance::Distance(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Distance::copyData(const Distance& org, const bool)
{
   BaseClass::copyData(org);
   val = fromDistance(org.toDistance());;
}

//==============================================================================
// Meters --
//==============================================================================
IMPLEMENT_SUBCLASS(Meters, "Meters")
EMPTY_SLOTTABLE(Meters)
EMPTY_COPYDATA(Meters)
EMPTY_DELETEDATA(Meters)

Meters::Meters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Meters::Meters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Meters::Meters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// CentiMeters --
//==============================================================================
IMPLEMENT_SUBCLASS(CentiMeters, "CentiMeters")
EMPTY_SLOTTABLE(CentiMeters)
EMPTY_COPYDATA(CentiMeters)
EMPTY_DELETEDATA(CentiMeters)

CentiMeters::CentiMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

CentiMeters::CentiMeters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

CentiMeters::CentiMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// MicroMeters --
//==============================================================================
IMPLEMENT_SUBCLASS(MicroMeters, "MicroMeters")
EMPTY_SLOTTABLE(MicroMeters)
EMPTY_COPYDATA(MicroMeters)
EMPTY_DELETEDATA(MicroMeters)

MicroMeters::MicroMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

MicroMeters::MicroMeters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

MicroMeters::MicroMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Microns -- Same as Micrometers
//==============================================================================
IMPLEMENT_SUBCLASS(Microns, "Microns")
EMPTY_SLOTTABLE(Microns)
EMPTY_COPYDATA(Microns)
EMPTY_DELETEDATA(Microns)

Microns::Microns() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Microns::Microns(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Microns::Microns(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// KiloMeters --
//==============================================================================
IMPLEMENT_SUBCLASS(KiloMeters, "KiloMeters")
EMPTY_SLOTTABLE(KiloMeters)
EMPTY_COPYDATA(KiloMeters)
EMPTY_DELETEDATA(KiloMeters)

KiloMeters::KiloMeters() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

KiloMeters::KiloMeters(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloMeters::KiloMeters(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Inches --
//==============================================================================
IMPLEMENT_SUBCLASS(Inches, "Inches")
EMPTY_SLOTTABLE(Inches)
EMPTY_COPYDATA(Inches)
EMPTY_DELETEDATA(Inches)

Inches::Inches() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Inches::Inches(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Inches::Inches(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Feet --
//==============================================================================
IMPLEMENT_SUBCLASS(Feet, "Feet")
EMPTY_SLOTTABLE(Feet)
EMPTY_COPYDATA(Feet)
EMPTY_DELETEDATA(Feet)

Feet::Feet() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Feet::Feet(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Feet::Feet(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// NauticalMiles --
//==============================================================================
IMPLEMENT_SUBCLASS(NauticalMiles, "NauticalMiles")
EMPTY_SLOTTABLE(NauticalMiles)
EMPTY_COPYDATA(NauticalMiles)
EMPTY_DELETEDATA(NauticalMiles)

NauticalMiles::NauticalMiles() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

NauticalMiles::NauticalMiles(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

NauticalMiles::NauticalMiles(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// StatuteMiles --
//==============================================================================
IMPLEMENT_SUBCLASS(StatuteMiles, "StatuteMiles")
EMPTY_SLOTTABLE(StatuteMiles)
EMPTY_COPYDATA(StatuteMiles)
EMPTY_DELETEDATA(StatuteMiles)

StatuteMiles::StatuteMiles() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

StatuteMiles::StatuteMiles(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

StatuteMiles::StatuteMiles(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
