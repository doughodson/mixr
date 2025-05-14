
#include "mixr/models/signature/IrSphere.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/IrSensor.hpp"
#include "mixr/models/IrQueryMsg.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/units/areas.hpp"
#include "mixr/base/units/lengths.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IrSphere, "IrSphere")
EMPTY_DELETEDATA(IrSphere)

BEGIN_SLOTTABLE(IrSphere)
   "radius",
END_SLOTTABLE(IrSphere)

BEGIN_SLOT_MAP(IrSphere)
   ON_SLOT(1, setSlotIrSphereRadius, base::INumber)
   ON_SLOT(1, setSlotIrSphereRadius, base::ILength)
END_SLOT_MAP()

IrSphere::IrSphere()
{
   STANDARD_CONSTRUCTOR()
}

void IrSphere::copyData(const IrSphere& org, const bool)
{
   BaseClass::copyData(org);
   radius = org.radius;
}

bool IrSphere::setSlotIrSphereRadius(const mixr::base::INumber* const x)
{
   double value{};

   if (x != nullptr) {
      value = x->asDouble();
   }
   radius = value;
   return true;
}

bool IrSphere::setSlotIrSphereRadius(const mixr::base::ILength* const x)
{
   double value{};

   if (x != nullptr) {
      value = x->getValueInMeters();
   }
   radius = value;
   return true;
}

double IrSphere::getArea()
{
   //return (double) (radius * radius * 4.0 * PI);
   //Calculates the projected area of a sphere (projected area is a flat circle)
   return static_cast<double>(radius * radius * base::PI);
}

}
}
