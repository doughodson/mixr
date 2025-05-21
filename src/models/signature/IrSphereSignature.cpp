
#include "mixr/models/signature/IrSphereSignature.hpp"

#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/system/IrSensor.hpp"
#include "mixr/models/IrQueryMsg.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/units/areas.hpp"
#include "mixr/base/units/lengths.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IrSphereSignature, "IrSphereSignature")
EMPTY_DELETEDATA(IrSphereSignature)

BEGIN_SLOTTABLE(IrSphereSignature)
   "radius",
END_SLOTTABLE(IrSphereSignature)

BEGIN_SLOT_MAP(IrSphereSignature)
   ON_SLOT(1, setSlotIrSphereRadius, base::INumber)
   ON_SLOT(1, setSlotIrSphereRadius, base::ILength)
END_SLOT_MAP()

IrSphereSignature::IrSphereSignature()
{
   STANDARD_CONSTRUCTOR()
}

void IrSphereSignature::copyData(const IrSphereSignature& org, const bool)
{
   BaseClass::copyData(org);
   radius = org.radius;
}

bool IrSphereSignature::setSlotIrSphereRadius(const mixr::base::INumber* const x)
{
   double value{};

   if (x != nullptr) {
      value = x->asDouble();
   }
   radius = value;
   return true;
}

bool IrSphereSignature::setSlotIrSphereRadius(const mixr::base::ILength* const x)
{
   double value{};

   if (x != nullptr) {
      value = x->getValueInMeters();
   }
   radius = value;
   return true;
}

double IrSphereSignature::getArea()
{
   //return (double) (radius * radius * 4.0 * PI);
   //Calculates the projected area of a sphere (projected area is a flat circle)
   return static_cast<double>(radius * radius * base::PI);
}

}
}
