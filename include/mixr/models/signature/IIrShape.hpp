
#ifndef __mixr_models_common_IIrShape_HPP__
#define __mixr_models_common_IIrShape_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace base { class IArea; class ILength; class INumber; }
namespace models {
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IIrShape
// Description: Base class for infrared shape
// Factory name: IrShape
//------------------------------------------------------------------------------
class IIrShape : public base::IObject
{
   DECLARE_SUBCLASS(IIrShape, base::IObject)

public:
   IIrShape();

   virtual double getArea();            // returns the effective area as a number in square meters

   virtual double getReflectorAreaInFieldOfView(const IrQueryMsg* const);

private:
   double area{};

private:
   // slot table helper methods
   bool setSlotIrShapeArea(const base::IArea* const);
};

}
}

#endif
