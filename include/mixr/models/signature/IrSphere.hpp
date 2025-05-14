
#ifndef __mixr_models_common_IrSphere_HPP__
#define __mixr_models_common_IrSphere_HPP__

#include "mixr/models/signature/IIrShape.hpp"

namespace mixr {
namespace base { class IArea; class ILength; class INumber; }
namespace models {
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IIrSphere
//   Description: Interace for all IR shapes
//
// Factory name: IIrSphere
// Slots:
//   radius    <INumber>   ! Radius of sphere (default is 10)
//   radius    <ILength>
//------------------------------------------------------------------------------
class IrSphere final: public IIrShape
{
   DECLARE_SUBCLASS(IrSphere, IIrShape)

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

}
}

#endif
