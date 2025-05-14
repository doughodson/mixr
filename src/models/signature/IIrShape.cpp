#include "mixr/models/signature/IIrShape.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/IrSensor.hpp"
#include "mixr/models/signature/IrSignature.hpp"
#include "mixr/models/IrQueryMsg.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/units/areas.hpp"
#include "mixr/base/units/lengths.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_ABSTRACT_SUBCLASS(IIrShape, "IIrShape")

BEGIN_SLOTTABLE(IIrShape)
   "area",
END_SLOTTABLE(IIrShape)

BEGIN_SLOT_MAP(IIrShape)
   ON_SLOT(1, setSlotIrShapeArea, base::IArea)
END_SLOT_MAP()

EMPTY_DELETEDATA(IIrShape)

IIrShape::IIrShape()
{
   STANDARD_CONSTRUCTOR()
}

void IIrShape::copyData(const IIrShape& org, const bool)
{
   BaseClass::copyData(org);
   area = org.area;
}

bool IIrShape::setSlotIrShapeArea(const mixr::base::IArea* const a)
{
   if (a != nullptr) {
      area = a->getValueInSquareMeters();
   }
   return true;
}

double IIrShape::getArea()
{
   return area;
}

double IIrShape::getReflectorAreaInFieldOfView(const IrQueryMsg* const msg)
{
   const double angleOffBoresight{msg->getAngleOffBoresight()};
   const double maxAngle{msg->getSendingSensor()->getIFOVTheta()};
   if (angleOffBoresight > maxAngle) return 0;
   return getArea();
}

}
}
