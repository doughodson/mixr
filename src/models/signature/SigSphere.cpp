
#include "mixr/models/signature/SigSphere.hpp"

#include "mixr/models/Emission.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/lengths.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SigSphere, "SigSphere")
EMPTY_DELETEDATA(SigSphere)

BEGIN_SLOTTABLE(SigSphere)
    "radius",       // 1 Radius of the sphere
END_SLOTTABLE(SigSphere)

BEGIN_SLOT_MAP(SigSphere)
    ON_SLOT(1, setSlotRadius, base::Number)
    ON_SLOT(1, setSlotRadius, base::ILength)
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
bool SigSphere::setSlotRadius(base::Number* const x)
{
   bool ok{};
   double r{-1.0};

   if (x != nullptr) {
      // Just a Number
      r = x->asDouble();
   }

   if (r >= 0.0) {
      setRadius(r);
      ok = true;
   } else {
      std::cerr << "SigSphere::setRadius: invalid radius; must be greater than or equal to zero!" << std::endl;
   }
   return ok;
}

bool SigSphere::setSlotRadius(base::ILength* const x)
{
   bool ok{};
   double r{ -1.0 };

   if (x != nullptr) {
      // we need meters
      r = x->getValueInMeters();
   }

   if (r >= 0.0) {
      setRadius(r);
      ok = true;
   } else {
      std::cerr << "SigSphere::setRadius: invalid radius; must be greater than or equal to zero!" << std::endl;
   }
   return ok;
}

}
}
