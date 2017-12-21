
#ifndef __mixr_models_IrShape_H__
#define __mixr_models_IrShape_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base { class Number; }
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
   bool setSlotIrShapeArea(const base::Number* const);
};

//------------------------------------------------------------------------------
// Class: IrSphere
//
// Factory name: IrSphere
// Slots:
//   radius    <Number>   ! Radius of sphere (default is 10)
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
};

//------------------------------------------------------------------------------
// Class: IrBox
//
// Factory name: IrBox
// Slots:
//   x        < Number>         ! x dimension  (default is 10)
//   y        < Number>         ! y dimension
//   z        < Number>         ! z dimension
//------------------------------------------------------------------------------
class IrBox : public IrShape
{
   DECLARE_SUBCLASS(IrBox, IrShape)

public:

   IrBox();

   double getArea() override;
   double getReflectorAreaInFieldOfView(const IrQueryMsg* const) override;

private:
   double x{10.0}, y{10.0}, z{10.0};

private:
   bool setSlotIrBoxX(const base::Number* const);
   bool setSlotIrBoxY(const base::Number* const);
   bool setSlotIrBoxZ(const base::Number* const);
};

}
}

#endif
