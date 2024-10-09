
//------------------------------------------------------------------------------
// Classes: IrShape, IrSphere, IrBox
//------------------------------------------------------------------------------
#include "mixr/models/IrShapes.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/IrSensor.hpp"
#include "mixr/models/signature/IrSignature.hpp"
#include "mixr/models/IrQueryMsg.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/units/areas.hpp"
#include "mixr/base/units/lengths.hpp"

#include <cmath>

namespace mixr {
namespace models {

//==============================================================================
// Class: IrShape (Abstract)
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(IrShape, "IrShape")

BEGIN_SLOTTABLE(IrShape)
   "area",
END_SLOTTABLE(IrShape)

BEGIN_SLOT_MAP(IrShape)
   ON_SLOT(1, setSlotIrShapeArea, base::Area)
END_SLOT_MAP()

EMPTY_DELETEDATA(IrShape)

IrShape::IrShape()
{
   STANDARD_CONSTRUCTOR()
}

void IrShape::copyData(const IrShape& org, const bool)
{
   BaseClass::copyData(org);
   area = org.area;
}

bool IrShape::setSlotIrShapeArea(const mixr::base::Area* const a)
{
   if (a != nullptr) {
      area = a->getValueInSquareMeters();
   }
   return true;
}

double IrShape::getArea()
{
   return area;
}

double IrShape::getReflectorAreaInFieldOfView(const IrQueryMsg* const msg)
{
   const double angleOffBoresight{msg->getAngleOffBoresight()};
   const double maxAngle{msg->getSendingSensor()->getIFOVTheta()};
   if (angleOffBoresight > maxAngle) return 0;
   return getArea();
}

//==============================================================================
// Class: IrSphere
//==============================================================================
IMPLEMENT_SUBCLASS(IrSphere, "IrSphere")
EMPTY_DELETEDATA(IrSphere)

BEGIN_SLOTTABLE(IrSphere)
   "radius",
END_SLOTTABLE(IrSphere)

BEGIN_SLOT_MAP(IrSphere)
   ON_SLOT(1, setSlotIrSphereRadius, base::Number)
   ON_SLOT(1, setSlotIrSphereRadius, base::Length)
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

bool IrSphere::setSlotIrSphereRadius(const mixr::base::Number* const x)
{
   double value{};

   if (x != nullptr) {
      value = x->asDouble();
   }
   radius = value;
   return true;
}

bool IrSphere::setSlotIrSphereRadius(const mixr::base::Length* const x)
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


//==============================================================================
// Class: IrBox
//==============================================================================
IMPLEMENT_SUBCLASS(IrBox, "IrBox")
EMPTY_DELETEDATA(IrBox)

BEGIN_SLOTTABLE(IrBox)
   "x",
   "y",
   "z",
END_SLOTTABLE(IrBox)

BEGIN_SLOT_MAP(IrBox)
   ON_SLOT(1, setSlotIrBoxX, base::Length)
   ON_SLOT(2, setSlotIrBoxY, base::Length)
   ON_SLOT(3, setSlotIrBoxZ, base::Length)
END_SLOT_MAP()

IrBox::IrBox()
{
   STANDARD_CONSTRUCTOR()
}

void IrBox::copyData(const IrBox& org, const bool)
{
   BaseClass::copyData(org);
   x = org.x;
   y = org.y;
   z = org.z;
}

bool IrBox::setSlotIrBoxX(const mixr::base::Length* const d)
{
   if (d != nullptr) {
      x = d->getValueInMeters();
   }
   return true;
}

bool IrBox::setSlotIrBoxY(const mixr::base::Length* const d)
{
   if (d != nullptr) {
      y = d->getValueInMeters();
   }
   return true;
}

bool IrBox::setSlotIrBoxZ(const mixr::base::Length* const d)
{
   if (d != nullptr) {
      z = d->getValueInMeters();
   }
   return true;
}

double IrBox::getArea()
{
   return static_cast<double>((x*y * 2) + (2*x*z) + (2*y*z));
}

double IrBox::getReflectorAreaInFieldOfView(const IrQueryMsg* const msg)
{
   double area{};
#if 1
   //retrieve the angle off of the gimbal boresight
   const double angleOffBoresight{msg->getAngleOffBoresight()};

   //retrieve the planar angle of the IFOV of the IR sensor
   const double maxAngle{msg->getSendingSensor()->getIFOVTheta()};

   //If the angle to the target is outside the IFOV, then return an area of 0.
   //This will cause the target to be discarded from further IR signature processing.
   if (angleOffBoresight > maxAngle){
      //std::cout << angleOffBoresight * base::Angle::R2DCC << " " << msg->getGimbalAzimuth() * base::Angle::R2DCC << " " << msg->getGimbalElevation() * base::Angle::R2DCC << " DETECTED" << std::endl;
      return area;
   }

   //std::cout << angleOffBoresight * base::Angle::R2DCC << " " << msg->getGimbalAzimuth() * base::Angle::R2DCC << " " << msg->getGimbalElevation() * base::Angle::R2DCC << " DETECTED" << std::endl;

   //Calculate the projected area of the box.
   const base::Vec3d targetAoiVector{msg->getAoiVector()};

   // front/back of the box to the sensor
   // x, z are members of this object corresponding to width and height.
   area += std::fabs(targetAoiVector[0]) * x * z;

   // side/side of the box to the sensor
   // y,z are members of this object corresponding to length and height.
   area += std::fabs(targetAoiVector[1]) * y * z;

   // top/bottom of the box to the sensor.
   // x,y is a member of this object corresponding to width and length.
   area += std::fabs(targetAoiVector[2]) * x * y;

   return area;

#else
   const double angleOffBoresight = msg->getAngleOffBoresight();
   const double maxAngle{msg->getSendingSensor()->getIFOVTheta()};

   if (angleOffBoresight > maxAngle)
      return area;
   else {

   const Player* sensorPlatform{msg->getOwnship()};
   const Player* target{msg->getTarget()};
   base::Vec3d targetVelocityVector{target->getVelocity()};
   base::Vec3d targetPosition{target->getPosition()};
   base::Vec3d sensorPosition{sensorPlatform->getPosition()};
   base::Vec3d directionToSensor{sensorPosition - targetPosition};
   base::Vec3d normalizedDirectionToSensor = directionToSensor;
   normalizedDirectionToSensor.normalize();
   base::Vec3d normalizedTargetVelocityVector{targetVelocityVector};
   normalizedTargetVelocityVector.normalize();
   base::Vec3d targetXVector;
   targetXVector[0] = normalizedTargetVelocityVector[0]; // x
   double frontToSensor{targetXVector * normalizedDirectionToSensor};
   if (frontToSensor < 0) frontToSensor = -frontToSensor;
      area += frontToSensor * x * z; // x, z are members of this object corresponding to
                                     // width and height.

   base::Vec3d targetYVector;
   targetYVector[1] = normalizedTargetVelocityVector[1]; // y
   double sideToSensor{targetYVector * normalizedDirectionToSensor};
   if (sideToSensor < 0) sideToSensor = -sideToSensor;
      area += sideToSensor * y * z; // y,z are members of this object corresponding to
                                    // length and height.

   base::Vec3d targetZVector;
   targetZVector[2] = normalizedTargetVelocityVector[2]; // y
   double topToSensor{targetZVector * normalizedDirectionToSensor};
   if (topToSensor < 0) topToSensor = -topToSensor;
      area += topToSensor * x * y; // x,y is a member of this object corresponding to
                                   // width and length.
   return area;
#endif

}

}
}
