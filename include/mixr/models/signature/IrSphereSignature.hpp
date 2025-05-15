
#ifndef __mixr_models_common_IrSphereSignature_HPP__
#define __mixr_models_common_IrSphereSignature_HPP__

#include "mixr/models/signature/IIrShapeSignature.hpp"

namespace mixr {
namespace base { class IArea; class ILength; class INumber; }
namespace models {
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrSphereSignature
//   Description: Concrete IR shape
//
// Factory name: IrSphereSignature
// Slots:
//   radius    <INumber>   ! Radius of sphere (default is 10)
//   radius    <ILength>
//------------------------------------------------------------------------------
class IrSphereSignature final: public IIrShapeSignature
{
   DECLARE_SUBCLASS(IrSphereSignature, IIrShapeSignature)

public:

   IrSphereSignature();

   double getArea() override;

   double getRadius()              { return radius; }

private:
   double radius{10.0};

private:
   // slot table helper methods
   bool setSlotIrSphereRadius(const base::INumber* const);
   bool setSlotIrSphereRadius(const mixr::base::ILength* const);
};

}
}

#endif
