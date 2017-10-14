
#include "mixr/base/colors/Cie.hpp"

#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/MonitorMetrics.hpp"
#include <cstdio>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Cie, "cie")

BEGIN_SLOTTABLE(Cie)
    "luminance", // 1: ... Luminance component, range(0.0 to 1.0)
    "x",         // 2: ... X component, range(0.0 to 1.0)
    "y",         // 3: ... Y component, range(0.0 to 1.0)
    "monitor"    // 4: ... Characteristics of intended display
END_SLOTTABLE(Cie)

BEGIN_SLOT_MAP(Cie)
    ON_SLOT(1, setSlotLuminance, Number)
    ON_SLOT(2, setSlotX, Number)
    ON_SLOT(3, setSlotY, Number)
    ON_SLOT(4, setSlotMonitor, MonitorMetrics)
END_SLOT_MAP()

Cie::Cie(const MonitorMetrics*, const double l, const double x, const double y)
{
   STANDARD_CONSTRUCTOR()
   cie[LUMINANCE] = l;   // set the values
   cie[X]         = x;
   cie[Y]         = y;
   cie2rgb(color,cie,monitor);   // set the rgb values
}

Cie::Cie()
{
   STANDARD_CONSTRUCTOR()
   cie[LUMINANCE] = 0.0f;  // default to black
   cie[X]         = 0.0f;
   cie[Y]         = 0.0f;
   monitor = new MonitorMetrics(); // use a default monitor configuration
   cie2rgb(color,cie,monitor);   // set the rgb values
}

void Cie::copyData(const Cie& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) monitor = nullptr;

   cie = org.cie;
   monitor = org.monitor;
}

void Cie::deleteData()
{
   monitor = nullptr;
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------
double Cie::luminance() const
{
    return cie[LUMINANCE];
}

double Cie::x() const
{
    return cie[X];
}

double Cie::y() const
{
    return cie[Y];
}

void Cie::getCIE(Vec3d& hhh) const
{
    hhh.set(cie[LUMINANCE],cie[X],cie[Y]);
}

//------------------------------------------------------------------------------
// setSlotLuminance() -- set the luminance value
//------------------------------------------------------------------------------
bool Cie::setSlotLuminance(const Number* const msg)
{
    if (msg == nullptr) return false;
    const double value = msg->getReal();
    const bool ok = (value >= 0 && value <= 1);
    if (ok) {
        cie[LUMINANCE] = value;
        cie2rgb(color,cie,monitor);
    } else {
        std::cerr << "Cie::setLuminance: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotX() -- set the X value
//------------------------------------------------------------------------------
bool Cie::setSlotX(const Number* const msg)
{
    if (msg == nullptr) return false;
    const double value = msg->getReal();
    const bool ok = (value >= 0 && value <= 1);
    if (ok) {
        cie[X] = value;
        cie2rgb(color,cie,monitor);
    } else {
        std::cerr << "Cie::setX: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotY() -- set the X value
//------------------------------------------------------------------------------
bool Cie::setSlotY(const Number* const msg)
{
    if (msg == nullptr) return false;
    const double value = msg->getReal();
    const bool ok = (value >= 0 && value <= 1);
    if (ok) {
        cie[Y] = value;
        cie2rgb(color,cie,monitor);
    } else {
        std::cerr << "Cie::setY: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMonitor() -- set the monitor parameters
//------------------------------------------------------------------------------
bool Cie::setSlotMonitor(const MonitorMetrics* const msg)
{
    if (msg == nullptr) return false;
    monitor = msg;
    cie2rgb(color, cie, monitor);
    return true;
}

//------------------------------------------------------------------------------
// cie2rgb() -- converts a CIE color to a Red, Green, Blue (RGB) value.
//------------------------------------------------------------------------------
void Cie::cie2rgb(Vec4d& rgb, const Vec3d& cie, const MonitorMetrics* m)
{
   if ( m == nullptr )
      return;

   m->cie2rgb(rgb, cie);
}

}
}
