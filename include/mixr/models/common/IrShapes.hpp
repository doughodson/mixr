
#ifndef __mixr_models_common_IrShape_HPP__
#define __mixr_models_common_IrShape_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base { class Area; class Length; class Number; class Length; }
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
   bool setSlotIrShapeArea(const base::Area* const);
};

//------------------------------------------------------------------------------
// Class: IrSphere
//
// Factory name: IrSphere
// Slots:
//   radius    <Number>   ! Radius of sphere (default is 10)
//   radius    <Length>
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
   bool setSlotIrSphereRadius(const base::Number* const);
   bool setSlotIrSphereRadius(const mixr::base::Length* const);
};

//------------------------------------------------------------------------------
// Class: IrBox
//
// Factory name: IrBox
// Slots:
//   x      <base::Length>         ! x dimension (default is 10 meters)
//   y      <base::Length>         ! y dimension (default is 10 meters)
//   z      <base::Length>         ! z dimension (default is 10 meters)
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
   bool setSlotIrBoxX(const base::Length* const);
   bool setSlotIrBoxY(const base::Length* const);
   bool setSlotIrBoxZ(const base::Length* const);
};

}
}

#endif
