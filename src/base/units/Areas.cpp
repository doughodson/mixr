
//------------------------------------------------------------------------------
// Area.cpp: implementation of the Area classes          
// Square - Meters, Inches, Yards, Miles, KiloMeters, CentiMeters, MilliMeters,
// DecibelMeters, Feet
//------------------------------------------------------------------------------

#include "mixr/base/units/Areas.hpp"

#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Area()
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Area, "AbstractArea")
EMPTY_SLOTTABLE(Area)
EMPTY_DELETEDATA(Area)

Area::Area()
{
   STANDARD_CONSTRUCTOR()
}

Area::Area(const double value) : Number(value) 
{
   STANDARD_CONSTRUCTOR()
}

void Area::copyData(const Area& org, const bool)
{
   BaseClass::copyData(org);
   val = fromArea(org.toArea());;
}

//==============================================================================
// SquareMeters() --
//==============================================================================
IMPLEMENT_SUBCLASS(SquareMeters, "SquareMeters")
EMPTY_SLOTTABLE(SquareMeters)
EMPTY_COPYDATA(SquareMeters)
EMPTY_DELETEDATA(SquareMeters)

SquareMeters::SquareMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareMeters::SquareMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareMeters::SquareMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}


//==============================================================================
// SquareFeet() --
//==============================================================================
IMPLEMENT_SUBCLASS(SquareFeet, "SquareFeet")
EMPTY_SLOTTABLE(SquareFeet)
EMPTY_COPYDATA(SquareFeet)
EMPTY_DELETEDATA(SquareFeet)

SquareFeet::SquareFeet() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareFeet::SquareFeet(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareFeet::SquareFeet(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// SquareInches() --
//==============================================================================
IMPLEMENT_SUBCLASS(SquareInches, "SquareInches")
EMPTY_SLOTTABLE(SquareInches)
EMPTY_COPYDATA(SquareInches)
EMPTY_DELETEDATA(SquareInches)

SquareInches::SquareInches() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareInches::SquareInches(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareInches::SquareInches(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// SquareYards() --
//==============================================================================
IMPLEMENT_SUBCLASS(SquareYards, "SquareYards")
EMPTY_SLOTTABLE(SquareYards)
EMPTY_COPYDATA(SquareYards)
EMPTY_DELETEDATA(SquareYards)

SquareYards::SquareYards() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareYards::SquareYards(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareYards::SquareYards(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// SquareMiles() --
//==============================================================================
IMPLEMENT_SUBCLASS(SquareMiles, "SquareMiles")
EMPTY_SLOTTABLE(SquareMiles)
EMPTY_COPYDATA(SquareMiles)
EMPTY_DELETEDATA(SquareMiles)

SquareMiles::SquareMiles() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareMiles::SquareMiles(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareMiles::SquareMiles(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// SquareCentiMeters() --
//==============================================================================
IMPLEMENT_SUBCLASS(SquareCentiMeters, "SquareCentiMeters")
EMPTY_SLOTTABLE(SquareCentiMeters)
EMPTY_COPYDATA(SquareCentiMeters)
EMPTY_DELETEDATA(SquareCentiMeters)

SquareCentiMeters::SquareCentiMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareCentiMeters::SquareCentiMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareCentiMeters::SquareCentiMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// SquareMilliMeters() --
//==============================================================================
IMPLEMENT_SUBCLASS(SquareMilliMeters, "SquareMilliMeters")
EMPTY_SLOTTABLE(SquareMilliMeters)
EMPTY_COPYDATA(SquareMilliMeters)
EMPTY_DELETEDATA(SquareMilliMeters)

SquareMilliMeters::SquareMilliMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareMilliMeters::SquareMilliMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareMilliMeters::SquareMilliMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// SquareKiloMeters() --
//==============================================================================
IMPLEMENT_SUBCLASS(SquareKiloMeters, "SquareKiloMeters")
EMPTY_SLOTTABLE(SquareKiloMeters)
EMPTY_COPYDATA(SquareKiloMeters)
EMPTY_DELETEDATA(SquareKiloMeters)

SquareKiloMeters::SquareKiloMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

SquareKiloMeters::SquareKiloMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

SquareKiloMeters::SquareKiloMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// DecibelSquareMeters() --
//==============================================================================
IMPLEMENT_SUBCLASS(DecibelSquareMeters, "DecibelSquareMeters")
EMPTY_SLOTTABLE(DecibelSquareMeters)
EMPTY_COPYDATA(DecibelSquareMeters)
EMPTY_DELETEDATA(DecibelSquareMeters)

DecibelSquareMeters::DecibelSquareMeters() : Area()
{
    STANDARD_CONSTRUCTOR()
}

DecibelSquareMeters::DecibelSquareMeters(const double value) : Area(value)
{
    STANDARD_CONSTRUCTOR()
}

DecibelSquareMeters::DecibelSquareMeters(const Area& value) : Area()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
