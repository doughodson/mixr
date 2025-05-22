
#include "mixr/models/signature/RfSphereSignature.hpp"

#include "mixr/models/RfEmission.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/lengths.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(RfSphereSignature, "RfSphereSignature")
EMPTY_DELETEDATA(RfSphereSignature)

BEGIN_SLOTTABLE(RfSphereSignature)
    "radius",       // 1 Radius of the sphere
END_SLOTTABLE(RfSphereSignature)

BEGIN_SLOT_MAP(RfSphereSignature)
    ON_SLOT(1, setSlotRadius, base::INumber)
    ON_SLOT(1, setSlotRadius, base::ILength)
END_SLOT_MAP()

RfSphereSignature::RfSphereSignature()
{
    STANDARD_CONSTRUCTOR()
    setRadius(0);
}

RfSphereSignature::RfSphereSignature(const double r)
{
    STANDARD_CONSTRUCTOR()
    setRadius(r);
}

void RfSphereSignature::copyData(const RfSphereSignature& org, const bool)
{
    BaseClass::copyData(org);
    setRadius(org.radius);
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double RfSphereSignature::getRCS(const RfEmission* const)
{
    return rcs;
}

//------------------------------------------------------------------------------
// setRadiusFromSlot() -- Set the radius from Slot table
//------------------------------------------------------------------------------
bool RfSphereSignature::setSlotRadius(base::INumber* const x)
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
      std::cerr << "RfSphereSignature::setRadius: invalid radius; must be greater than or equal to zero!" << std::endl;
   }
   return ok;
}

bool RfSphereSignature::setSlotRadius(base::ILength* const x)
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
      std::cerr << "RfSphereSignature::setRadius: invalid radius; must be greater than or equal to zero!" << std::endl;
   }
   return ok;
}

}
}
