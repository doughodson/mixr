#include "mixr/models/signature/IIrShapeSignature.hpp"

#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/system/IrSensor.hpp"
#include "mixr/models/signature/IIrSignature.hpp"
#include "mixr/models/IrQueryMsg.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/units/areas.hpp"
#include "mixr/base/units/lengths.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_ABSTRACT_SUBCLASS(IIrShapeSignature, "IIrShapeSignature")

BEGIN_SLOTTABLE(IIrShapeSignature)
   "area",
END_SLOTTABLE(IIrShapeSignature)

BEGIN_SLOT_MAP(IIrShapeSignature)
   ON_SLOT(1, setSlotIrShapeArea, base::IArea)
END_SLOT_MAP()

EMPTY_DELETEDATA(IIrShapeSignature)

IIrShapeSignature::IIrShapeSignature()
{
   STANDARD_CONSTRUCTOR()
}

void IIrShapeSignature::copyData(const IIrShapeSignature& org, const bool)
{
   BaseClass::copyData(org);
   area = org.area;
}

bool IIrShapeSignature::setSlotIrShapeArea(const mixr::base::IArea* const a)
{
   if (a != nullptr) {
      area = a->getValueInSquareMeters();
   }
   return true;
}

double IIrShapeSignature::getArea()
{
   return area;
}

double IIrShapeSignature::getReflectorAreaInFieldOfView(const IrQueryMsg* const msg)
{
   const double angleOffBoresight{msg->getAngleOffBoresight()};
   const double maxAngle{msg->getSendingSensor()->getIFOVTheta()};
   if (angleOffBoresight > maxAngle) return 0;
   return getArea();
}

}
}
