
#ifndef __mixr_models_common_IrBox_HPP__
#define __mixr_models_common_IrBox_HPP__

#include "mixr/models/signature/IIrShape.hpp"

namespace mixr {
namespace base { class IArea; class ILength; class INumber; }
namespace models {
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrBox
//
// Factory name: IrBox
// Slots:
//   x      <base::ILength>         ! x dimension (default is 10 meters)
//   y      <base::ILength>         ! y dimension (default is 10 meters)
//   z      <base::ILength>         ! z dimension (default is 10 meters)
//------------------------------------------------------------------------------
class IrBox final: public IIrShape
{
   DECLARE_SUBCLASS(IrBox, IIrShape)

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
