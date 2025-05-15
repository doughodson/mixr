
#ifndef __mixr_models_common_IrBoxSignature_HPP__
#define __mixr_models_common_IrBoxSignature_HPP__

#include "mixr/models/signature/IIrShapeSignature.hpp"

namespace mixr {
namespace base { class IArea; class ILength; class INumber; }
namespace models {
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrBoxSignature
//   Description: Concrete IR shape
//
// Factory name: IrBoxSignature
// Slots:
//   x      <base::ILength>         ! x dimension (default is 10 meters)
//   y      <base::ILength>         ! y dimension (default is 10 meters)
//   z      <base::ILength>         ! z dimension (default is 10 meters)
//------------------------------------------------------------------------------
class IrBoxSignature final: public IIrShapeSignature
{
   DECLARE_SUBCLASS(IrBoxSignature, IIrShapeSignature)

public:

   IrBoxSignature();

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
