
#include "mixr/base/units/Forces.hpp"

#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Force() --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Force, "AbstractForce")
EMPTY_SLOTTABLE(Force)
EMPTY_DELETEDATA(Force)

Force::Force()
{
    STANDARD_CONSTRUCTOR()
}

Force::Force(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Force::copyData(const Force& org, const bool)
{
   BaseClass::copyData(org);
   val = fromForce(org.toForce());;
}

//==============================================================================
// Newtons() --
//==============================================================================
IMPLEMENT_SUBCLASS(Newtons, "Newtons")
EMPTY_SLOTTABLE(Newtons)
EMPTY_COPYDATA(Newtons)
EMPTY_DELETEDATA(Newtons)

Newtons::Newtons() : Force()
{
    STANDARD_CONSTRUCTOR()
}

Newtons::Newtons(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

Newtons::Newtons(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Kilonewtons() --
//==============================================================================
IMPLEMENT_SUBCLASS(KiloNewtons, "KiloNewtons")
EMPTY_SLOTTABLE(KiloNewtons)
EMPTY_COPYDATA(KiloNewtons)
EMPTY_DELETEDATA(KiloNewtons)

KiloNewtons::KiloNewtons() : Force()
{
    STANDARD_CONSTRUCTOR()
}

KiloNewtons::KiloNewtons(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloNewtons::KiloNewtons(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// PoundForces() --
//==============================================================================
IMPLEMENT_SUBCLASS(PoundForces, "PoundForces")
EMPTY_SLOTTABLE(PoundForces)
EMPTY_COPYDATA(PoundForces)
EMPTY_DELETEDATA(PoundForces)

PoundForces::PoundForces() : Force()
{
    STANDARD_CONSTRUCTOR()
}

PoundForces::PoundForces(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

PoundForces::PoundForces(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// Poundals() --
//==============================================================================
IMPLEMENT_SUBCLASS(Poundals, "Poundals")
EMPTY_SLOTTABLE(Poundals)
EMPTY_COPYDATA(Poundals)
EMPTY_DELETEDATA(Poundals)

Poundals::Poundals() : Force()
{
    STANDARD_CONSTRUCTOR()
}

Poundals::Poundals(const double value) : Force(value)
{
    STANDARD_CONSTRUCTOR()
}

Poundals::Poundals(const Force& value) : Force()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}

