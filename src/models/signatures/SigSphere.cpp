
#include "mixr/models/signatures/SigSphere.hpp"

#include "mixr/models/Emission.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/Distances.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SigSphere, "SigSphere")
EMPTY_DELETEDATA(SigSphere)

BEGIN_SLOTTABLE(SigSphere)
    "radius",       // 1 Radius of the sphere
END_SLOTTABLE(SigSphere)

BEGIN_SLOT_MAP(SigSphere)
    ON_SLOT(1, setSlotRadius, base::Number)
END_SLOT_MAP()

SigSphere::SigSphere()
{
    STANDARD_CONSTRUCTOR()
    setRadius(0);
}

SigSphere::SigSphere(const double r)
{
    STANDARD_CONSTRUCTOR()
    setRadius(r);
}

void SigSphere::copyData(const SigSphere& org, const bool)
{
    BaseClass::copyData(org);
    setRadius(org.radius);
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigSphere::getRCS(const Emission* const)
{
    return rcs;
}

//------------------------------------------------------------------------------
// setRadiusFromSlot() -- Set the radius from Slot table
//------------------------------------------------------------------------------
bool SigSphere::setSlotRadius(base::Number* const num)
{
    bool ok{};
    double r{-1.0};

    const auto d = dynamic_cast<base::Distance*>(num);
    if (d != nullptr) {
        // Has distance units and we need meters
        base::Meters meters;
        r = meters.convert(*d);
    } else if (num != nullptr) {
        // Just a Number
        r = num->getReal();
    }

    if (r >= 0.0) { setRadius(r); ok = true; }
    else { std::cerr << "SigSphere::setRadius: invalid radius; must be greater than or equal to zero!" << std::endl; }
    return ok;
}

}
}
