
#ifndef __mixr_models_common_IIrShapeSignature_HPP__
#define __mixr_models_common_IIrShapeSignature_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace base { class IArea; class ILength; class INumber; }
namespace models {
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IIrShapeSignature
// Description: Interface for infrared shapes
//
// Factory name: IIrShapeSignature
//------------------------------------------------------------------------------
class IIrShapeSignature : public base::IObject
{
   DECLARE_SUBCLASS(IIrShapeSignature, base::IObject)

public:
   IIrShapeSignature();

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
