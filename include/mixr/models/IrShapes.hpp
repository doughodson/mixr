
#ifndef __mixr_models_common_IrShape_HPP__
#define __mixr_models_common_IrShape_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base { class IArea; class ILength; class INumber; }
namespace models {
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrShape
// Description: Base class for infrared shape
// Factory name: IrShape
//------------------------------------------------------------------------------
class IrShape : public base::Object
{
   DECLARE_SUBCLASS(IrShape, base::Object)

public:
   IrShape();

   virtual double getArea();            // returns the effective area as a number in square meters

   virtual double getReflectorAreaInFieldOfView(const IrQueryMsg* const);

private:
   double area{};

private:
   // slot table helper methods
   bool setSlotIrShapeArea(const base::IArea* const);
};

//------------------------------------------------------------------------------
// Class: IrSphere
//
// Factory name: IrSphere
// Slots:
//   radius    <INumber>   ! Radius of sphere (default is 10)
//   radius    <ILength>
//------------------------------------------------------------------------------
class IrSphere : public IrShape
{
   DECLARE_SUBCLASS(IrSphere, IrShape)

public:

   IrSphere();

   double getArea() override;

   double getRadius()              { return radius; }

private:
   double radius{10.0};

private:
   // slot table helper methods
   bool setSlotIrSphereRadius(const base::INumber* const);
   bool setSlotIrSphereRadius(const mixr::base::ILength* const);
};

//------------------------------------------------------------------------------
// Class: IrBox
//
// Factory name: IrBox
// Slots:
//   x      <base::ILength>         ! x dimension (default is 10 meters)
//   y      <base::ILength>         ! y dimension (default is 10 meters)
//   z      <base::ILength>         ! z dimension (default is 10 meters)
//------------------------------------------------------------------------------
class IrBox : public IrShape
{
   DECLARE_SUBCLASS(IrBox, IrShape)

public:

   IrBox();

   double getArea() override;
   double getReflectorAreaInFieldOfView(const IrQueryMsg* const) override;

private:
   double x{10.0};
   double y{10.0};
   double z{10.0};

private:
   bool setSlotIrBoxX(const base::ILength* const);
   bool setSlotIrBoxY(const base::ILength* const);
   bool setSlotIrBoxZ(const base::ILength* const);
};

}
}

#endif
