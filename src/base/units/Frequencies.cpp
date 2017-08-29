//------------------------------------------------------------------------------
// Frequencies, Hertz
//------------------------------------------------------------------------------
#include "mixr/base/units/Frequencies.hpp"

#include <iostream>

namespace mixr {
namespace base {

//==============================================================================
// Frequency --
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(Frequency, "AbstractFrequency")
EMPTY_SLOTTABLE(Frequency)
EMPTY_DELETEDATA(Frequency)

Frequency::Frequency() : Number()
{
    STANDARD_CONSTRUCTOR()
}

Frequency::Frequency(const double value) : Number(value)
{
    STANDARD_CONSTRUCTOR()
}

void Frequency::copyData(const Frequency& org, const bool)
{
   BaseClass::copyData(org);
   val = fromFrequency(org.toFrequency());;
}

//==============================================================================
// Hertz --
//==============================================================================
IMPLEMENT_SUBCLASS(Hertz, "Hertz")
EMPTY_SLOTTABLE(Hertz)
EMPTY_COPYDATA(Hertz)
EMPTY_DELETEDATA(Hertz)

Hertz::Hertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

Hertz::Hertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

Hertz::Hertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// KiloHertz --
//==============================================================================
IMPLEMENT_SUBCLASS(KiloHertz, "KiloHertz")
EMPTY_SLOTTABLE(KiloHertz)
EMPTY_COPYDATA(KiloHertz)
EMPTY_DELETEDATA(KiloHertz)

KiloHertz::KiloHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

KiloHertz::KiloHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloHertz::KiloHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// MegaHertz --
//==============================================================================
IMPLEMENT_SUBCLASS(MegaHertz, "MegaHertz")
EMPTY_SLOTTABLE(MegaHertz)
EMPTY_COPYDATA(MegaHertz)
EMPTY_DELETEDATA(MegaHertz)

MegaHertz::MegaHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

MegaHertz::MegaHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

MegaHertz::MegaHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// GigaHertz --
//==============================================================================
IMPLEMENT_SUBCLASS(GigaHertz, "GigaHertz")
EMPTY_SLOTTABLE(GigaHertz)
EMPTY_COPYDATA(GigaHertz)
EMPTY_DELETEDATA(GigaHertz)

GigaHertz::GigaHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

GigaHertz::GigaHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

GigaHertz::GigaHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

//==============================================================================
// TeraHertz --
//==============================================================================
IMPLEMENT_SUBCLASS(TeraHertz, "TeraHertz")
EMPTY_SLOTTABLE(TeraHertz)
EMPTY_COPYDATA(TeraHertz)
EMPTY_DELETEDATA(TeraHertz)

TeraHertz::TeraHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

TeraHertz::TeraHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

TeraHertz::TeraHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
