
#include "mixr/base/colors/Cmy.hpp"

#include "mixr/base/numeric/Float.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Cmy, "cmy")
EMPTY_DELETEDATA(Cmy)

BEGIN_SLOTTABLE(Cmy)
    "cyan",     // 1: ... Cyan component, range(0.0 to 1.0)
    "magenta",  // 2: ... Magenta component, range(0.0 to 1.0)
    "yellow"    // 3: ... Yellow component, range(0.0 to 1.0)
END_SLOTTABLE(Cmy)

BEGIN_SLOT_MAP(Cmy)
    ON_SLOT(1, setSlotCyan,    Number)
    ON_SLOT(2, setSlotMagenta, Number)
    ON_SLOT(3, setSlotYellow,  Number)
END_SLOT_MAP()

Cmy::Cmy(const double c, const double m, const double y)
{
   STANDARD_CONSTRUCTOR()
   cmy[CYAN]    = c;     // set the values
   cmy[MAGENTA] = m;
   cmy[YELLOW]  = y;
   cmy2rgb(color,cmy);   // set the rgb values
}

Cmy::Cmy()
{
   STANDARD_CONSTRUCTOR()
   cmy[CYAN]    = 0.0f;  // default to black
   cmy[MAGENTA] = 0.0f;
   cmy[YELLOW]  = 0.0f;
   cmy2rgb(color,cmy);   // set the rgb values
}

void Cmy::copyData(const Cmy& org, const bool)
{
   BaseClass::copyData(org);
   cmy = org.cmy;
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------
double Cmy::cyan() const
{
    return cmy[CYAN];
}

double Cmy::magenta() const
{
    return cmy[MAGENTA];
}

double Cmy::yellow() const
{
    return cmy[YELLOW];
}

void Cmy::getCMY(Vec3d& hhh) const
{
    hhh.set(cmy[CYAN],cmy[MAGENTA],cmy[YELLOW]);
}

//------------------------------------------------------------------------------
// setSlotCyan() -- set the cyan value
//------------------------------------------------------------------------------
bool Cmy::setSlotCyan(const Number* const msg)
{
    if (msg == nullptr) return false;
    double value{msg->getReal()};
    bool ok{value >= 0 && value <= 1};
    if (ok) {
        cmy[CYAN] = value;
        cmy2rgb(color,cmy);
    } else {
        std::cerr << "Cmy::setCyan: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMagenta() -- set the magenta value
//------------------------------------------------------------------------------
bool Cmy::setSlotMagenta(const Number* const msg)
{
    if (msg == nullptr) return false;
    const double value{msg->getReal()};
    const bool ok{value >= 0 && value <= 1};
    if (ok) {
        cmy[MAGENTA] = value;
        cmy2rgb(color,cmy);
    } else {
        std::cerr << "Cmy::setMagenta: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotYellow() -- set the yellow value
//------------------------------------------------------------------------------
bool Cmy::setSlotYellow(const Number* const msg)
{
    if (msg == nullptr) return false;
    const double value{msg->getReal()};
    const bool ok{value >= 0 && value <= 1};
    if (ok) {
        cmy[YELLOW] = value;
        cmy2rgb(color,cmy);
    } else {
        std::cerr << "Cmy::setYellow: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// cmy2rgb() -- converts a Cyan, Magenta, Yellow (CMY) color to a
//              Red, Green, Blue (RGB) value.
//
// This code is based on '/usr/people/4Dgifts/iristools/libgutil/colormod.c'
//------------------------------------------------------------------------------
void Cmy::cmy2rgb(Vec4d& rgb, const Vec3d& cmy)
{
    rgb[RED]   = 1 - cmy[CYAN];
    rgb[GREEN] = 1 - cmy[MAGENTA];
    rgb[BLUE]  = 1 - cmy[YELLOW];
    rgb[ALPHA] = defaultAlpha;
}

//------------------------------------------------------------------------------
// rgb2cmy -- converts a Red, Green, Blue (RGB) color to a
//              Cyan, Magenta, Yellow (CMY) value.
//
// This code is based on '/usr/people/4Dgifts/iristools/libgutil/colormod.c'
//------------------------------------------------------------------------------
void Cmy::rgb2cmy(Vec3d& cmy, const Vec4d& rgb)
{
    cmy[CYAN]    = 1 - rgb[RED];
    cmy[MAGENTA] = 1 - rgb[GREEN];
    cmy[YELLOW]  = 1 - rgb[BLUE];
}

}
}
