
#include "mixr/base/units/Volumes.hpp"
#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Volume() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Volume, "AbstractVolume")
EMPTY_SLOTTABLE(Volume)
EMPTY_DELETEDATA(Volume)

Volume::Volume()
{
    STANDARD_CONSTRUCTOR()
}

Volume::Volume(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Volume::copyData(const Volume& org, const bool)
{
   BaseClass::copyData(org);
   val = fromVolume(org.toVolume());;
}

//==============================================================================
// CubicMeters() --
//==============================================================================
IMPLEMENT_SUBCLASS(CubicMeters, "CubicMeters")
EMPTY_SLOTTABLE(CubicMeters)
EMPTY_COPYDATA(CubicMeters)
EMPTY_DELETEDATA(CubicMeters)

CubicMeters::CubicMeters() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicMeters::CubicMeters(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicMeters::CubicMeters(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}
    
//==============================================================================
// CubicFeet() --
//==============================================================================
IMPLEMENT_SUBCLASS(CubicFeet, "CubicFeet")
EMPTY_SLOTTABLE(CubicFeet)
EMPTY_COPYDATA(CubicFeet)
EMPTY_DELETEDATA(CubicFeet)

CubicFeet::CubicFeet() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicFeet::CubicFeet(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicFeet::CubicFeet(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// CubicInches() --
//==============================================================================
IMPLEMENT_SUBCLASS(CubicInches, "CubicInches")
EMPTY_SLOTTABLE(CubicInches)
EMPTY_COPYDATA(CubicInches)
EMPTY_DELETEDATA(CubicInches)

CubicInches::CubicInches() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

CubicInches::CubicInches(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
CubicInches::CubicInches(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Liters() --
//==============================================================================
IMPLEMENT_SUBCLASS(Liters, "Liters")
EMPTY_SLOTTABLE(Liters)
EMPTY_COPYDATA(Liters)
EMPTY_DELETEDATA(Liters)

Liters::Liters() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

Liters::Liters(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
Liters::Liters(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

