
#include "mixr/models/player/space/ISpaceVehicle.hpp"
#include "mixr/models/dynamics/ISpaceDynamics.hpp"

#include "mixr/base/qty/lengths.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ISpaceVehicle, "ISpaceVehicle")
EMPTY_SLOTTABLE(ISpaceVehicle)
EMPTY_COPYDATA(ISpaceVehicle)
EMPTY_DELETEDATA(ISpaceVehicle)

ISpaceVehicle::ISpaceVehicle()
{
   STANDARD_CONSTRUCTOR()

   static base::String generic("GenericSpaceVehicle");
   setType_old(&generic);
   setType("GenericSpaceVehicle");
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int ISpaceVehicle::getMajorType() const
{
   return SPACE_VEHICLE;
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------

double ISpaceVehicle::getFuelWt() const
{
   double value{};
   if (getDynamicsModel() != nullptr) {
      value = getDynamicsModel()->getFuelWt();
   }
   return value;
}

double ISpaceVehicle::getFuelWtMax() const
{
   double value{};
   if (getDynamicsModel() != nullptr) {
      value = getDynamicsModel()->getFuelWtMax();
   }
   return value;
}

double ISpaceVehicle::getGrossWeight() const
{
   double value{};
   if (getDynamicsModel() != nullptr) {
      value = getDynamicsModel()->getGrossWeight();
   }
   return value;
}

//------------------------------------------------------------------------------
// setControlStickYawInput(yaw) -- Yaw inputs: normalized
//          yaw:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool ISpaceVehicle::setControlStickYawInput(const double yaw)
{
   bool ok{};
   const auto model = dynamic_cast<ISpaceDynamics*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setControlStickYawInput(yaw);
   }
   return ok;
}

//------------------------------------------------------------------------------
// setTranslateXInput(transx) -- Translate X inputs: normalized
//          transx:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool ISpaceVehicle::setTranslateXInput(const double transx)
{
   bool ok{};
   const auto model = dynamic_cast<ISpaceDynamics*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setTranslateXInput(transx);
   }
   return ok;
}

//------------------------------------------------------------------------------
// setTranslateYInput(transy) -- Translate Y inputs: normalized
//          transy:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool ISpaceVehicle::setTranslateYInput(const double transy)
{
   bool ok{};
   const auto model = dynamic_cast<ISpaceDynamics*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setTranslateYInput(transy);
   }
   return ok;
}

//------------------------------------------------------------------------------
// setTranslateZInput(transz) -- Translate Z inputs: normalized
//          transz:  -1.0 -> maz left;  0.0 -> center;  1.0 -> maz right
//------------------------------------------------------------------------------
bool ISpaceVehicle::setTranslateZInput(const double transz)
{
   bool ok{};
   const auto model = dynamic_cast<ISpaceDynamics*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setTranslateZInput(transz);
   }
   return ok;
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int ISpaceVehicle::getNumberOfEngines() const
{
   int n{};
   if (getDynamicsModel() != nullptr) {
      n = getDynamicsModel()->getNumberOfEngines();
   }
   return n;
}

int ISpaceVehicle::getEngThrust(double* const data, const int max) const
{
   int n{};
   if (getDynamicsModel() != nullptr) {
      n = getDynamicsModel()->getEngThrust(data, max);
   }
   return n;
}

}
}
